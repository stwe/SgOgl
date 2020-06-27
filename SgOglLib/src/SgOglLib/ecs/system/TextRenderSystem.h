// This file is part of the SgOgl package.
// 
// Filename: TextRenderSystem.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "RenderSystem.h"
#include "resource/shaderprogram/TextShaderProgram.h"
#include "resource/ShaderManager.h"
#include "resource/TextureManager.h"
#include "resource/Mesh.h"
#include "ecs/component/Components.h"

namespace sg::ogl::ecs::system
{
    class TextRenderSystem : public RenderSystem<resource::shaderprogram::TextShaderProgram>
    {
    public:
        using MeshUniquedPtr = std::unique_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TextRenderSystem(scene::Scene* t_scene, std::string t_fontPath)
            : RenderSystem(t_scene)
            , m_fontPath{ std::move(t_fontPath) }
        {
            InitFreeType();
            CreateMesh();

            debugName = "TextRenderer";
        }

        TextRenderSystem(const int t_priority, scene::Scene* t_scene, std::string t_fontPath)
            : RenderSystem(t_priority, t_scene)
            , m_fontPath{ std::move(t_fontPath) }
        {
            InitFreeType();
            CreateMesh();

            debugName = "TextRenderer";
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override {}

        void Render() override
        {
            m_scene->GetApplicationContext()->registry.view<component::TextComponent>().each(
            [&](auto t_entity, auto& t_textComponent)
            {
                    RenderText(t_textComponent.text, t_textComponent.xPos, t_textComponent.yPos, t_textComponent.scale, t_textComponent.color);
            });
        }

        void PrepareRendering() override
        {
            OpenGl::EnableAlphaBlending();
            OpenGl::EnableFaceCulling();
        }

        void FinishRendering() override
        {
            OpenGl::DisableBlending();
            OpenGl::DisableFaceCulling();
        }

    protected:

    private:
        struct Character
        {
            uint32_t textureId;   // Id handle of the glyph texture.
            glm::ivec2 size;      // Size of glyph.
            glm::ivec2 bearing;   // Offset from baseline to left/top of glyph.
            signed long advance;  // Horizontal offset to advance to next glyph.
        };

        std::map<char, Character> m_characters;

        MeshUniquedPtr m_textMesh;
        std::string m_fontPath;

        uint32_t m_textureId{ 0 };
        uint32_t m_vboId{ 0 };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void InitFreeType()
        {
            FT_Library ft;

            // All functions return a value different than 0 whenever an error occurred.
            if (FT_Init_FreeType(&ft))
            {
                throw SG_OGL_EXCEPTION("[TextRenderSystem::InitFreeType()] Could not init FreeType Library.");
            }

            // Load font as face.
            FT_Face face;
            if (FT_New_Face(ft, m_fontPath.c_str(), 0, &face))
            {
                throw SG_OGL_EXCEPTION("[TextRenderSystem::InitFreeType()] Failed to load font.");
            }

            // Set size to load glyphs as.
            FT_Set_Pixel_Sizes(face, 0, 48);

            // Disable byte-alignment restriction.
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // Load first 128 characters of ASCII set.
            for (uint8_t c{ 0 }; c < 128; ++c)
            {
                // Load character glyph.
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    throw SG_OGL_EXCEPTION("[TextRenderSystem::InitFreeType()] Failed to load Glyph.");
                }

                // Generate texture.
                glGenTextures(1, &m_textureId);
                glBindTexture(GL_TEXTURE_2D, m_textureId);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );

                // Set texture options.
                resource::TextureManager::UseClampToEdgeWrapping();
                resource::TextureManager::UseBilinearFilter();

                // Now store character for later use.
                Character character = {
                    m_textureId,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
                };

                m_characters.insert(std::pair<char, Character>(c, character));
            }

            // Destroy FreeType once we're finished.
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
        }

        void CreateMesh()
        {
            // create an bind a new Vao
            m_textMesh = std::make_unique<resource::Mesh>();
            m_textMesh->GetVao().BindVao();

            // create a new Vbo
            m_vboId = buffer::Vbo::GenerateVbo();
            buffer::Vbo::InitEmpty(m_vboId, 6 * 4, GL_DYNAMIC_DRAW);
            buffer::Vbo::AddAttribute(m_vboId, 0, 4, 4, 0);

            // unbind Vao
            buffer::Vao::UnbindVao();

            // set draw count
            m_textMesh->GetVao().SetDrawCount(6);
        }

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void RenderText(const std::string& t_text, float t_xPos, const float t_yPos, const float t_scale, const glm::vec3& t_color)
        {
            // bind shader program
            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::TextShaderProgram>() };
            shaderProgram.Bind();

            // update uniforms
            shaderProgram.UpdateUniforms(*m_scene, t_color);

            // bind Vao
            m_textMesh->GetVao().BindVao();

            // iterate through all characters
            for (auto c = t_text.begin(); c != t_text.end(); ++c)
            {
                const auto ch{ m_characters[*c] };

                const auto xpos{ t_xPos + ch.bearing.x * t_scale };
                const auto ypos{ t_yPos - (ch.size.y - ch.bearing.y) * t_scale };

                const auto w{ ch.size.x * t_scale };
                const auto h{ ch.size.y * t_scale };

                // update Vbo for each character
                float vertices[6][4]{
                    { xpos,     ypos + h,   0.0, 0.0 },
                    { xpos,     ypos,       0.0, 1.0 },
                    { xpos + w, ypos,       1.0, 1.0 },

                    { xpos,     ypos + h,   0.0, 0.0 },
                    { xpos + w, ypos,       1.0, 1.0 },
                    { xpos + w, ypos + h,   1.0, 0.0 }
                };

                // bind a texture
                resource::TextureManager::BindForReading(ch.textureId, GL_TEXTURE0);

                // update content of Vbo memory
                buffer::Vbo::BindVbo(m_vboId);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                buffer::Vbo::UnbindVbo();

                // render
                m_textMesh->DrawPrimitives();

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                t_xPos += static_cast<float>(ch.advance >> 6) * t_scale;
            }

            // unbind Vao
            buffer::Vao::UnbindVao();

            // unbind texture
            resource::TextureManager::Unbind();

            // undbind shader program
            resource::ShaderProgram::Unbind();
        }
    };
}
