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
#include "resource/Mesh.h"

namespace sg::ogl::ecs::system
{
    class TextRenderSystem : public RenderSystem<resource::shaderprogram::TextShaderProgram>
    {
    public:
        using MeshSharedPtr = std::shared_ptr<resource::Mesh>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TextRenderSystem(scene::Scene* t_scene, std::string t_fontPath)
            : RenderSystem(t_scene)
            , m_fontPath{ std::move(t_fontPath) }
        {
            InitFreeType();
            CreateVao();
        }

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Update(double t_dt) override {}

        void Render() override {}

        //-------------------------------------------------
        // Custom
        //-------------------------------------------------

        void RenderText(const std::string& t_text, float t_xPos, const float t_yPos, const float t_scale, const glm::vec3& t_color)
        {
            PrepareRendering();

            auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram<resource::shaderprogram::TextShaderProgram>() };
            shaderProgram.Bind();

            shaderProgram.UpdateUniforms(*m_scene, t_color);



            glBindVertexArray(m_vao);

            // Iterate through all characters
            std::string::const_iterator c;
            for (c = t_text.begin(); c != t_text.end(); c++)
            {
                Character ch = m_characters[*c];

                float xpos = t_xPos + ch.bearing.x * t_scale;
                float ypos = t_yPos - (ch.size.y - ch.bearing.y) * t_scale;

                float w = ch.size.x * t_scale;
                float h = ch.size.y * t_scale;

                // Update VBO for each character
                GLfloat vertices[6][4] = {
                    { xpos,     ypos + h,   0.0, 0.0 },
                    { xpos,     ypos,       0.0, 1.0 },
                    { xpos + w, ypos,       1.0, 1.0 },

                    { xpos,     ypos + h,   0.0, 0.0 },
                    { xpos + w, ypos,       1.0, 1.0 },
                    { xpos + w, ypos + h,   1.0, 0.0 }
                };

                // Render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.textureId);

                // Update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // Render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                t_xPos += (ch.advance >> 6)* t_scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);



            resource::ShaderProgram::Unbind();

            FinishRendering();
        }

    protected:
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

    private:
        struct Character
        {
            uint32_t textureId;   // ID handle of the glyph texture.
            glm::ivec2 size;      // Size of glyph.
            glm::ivec2 bearing;   // Offset from baseline to left/top of glyph.
            signed long advance;  // Horizontal offset to advance to next glyph.
        };

        std::map<char, Character> m_characters;

        MeshSharedPtr m_textMesh;
        std::string m_fontPath;
        uint32_t m_texture{ 0 };
        uint32_t m_vao{ 0 };
        uint32_t m_vbo{ 0 };

        void InitFreeType()
        {
            FT_Library ft;

            // All functions return a value different than 0 whenever an error occurred.
            if (FT_Init_FreeType(&ft))
            {
                throw SG_OGL_EXCEPTION("[TextRenderSystem::Init()] Could not init FreeType Library.");
            }

            // Load font as face.
            FT_Face face;
            if (FT_New_Face(ft, m_fontPath.c_str(), 0, &face))
            {
                throw SG_OGL_EXCEPTION("[TextRenderSystem::Init()] Failed to load font.");
            }

            // Set size to load glyphs as.
            FT_Set_Pixel_Sizes(face, 0, 48);

            // Disable byte-alignment restriction.
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // Load first 128 characters of ASCII set.
            for (uint8_t c = 0; c < 128; ++c)
            {
                // Load character glyph.
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    throw SG_OGL_EXCEPTION("[TextRenderSystem::Init()] Failed to load Glyph.");
                }

                // Generate texture.
                glGenTextures(1, &m_texture);
                glBindTexture(GL_TEXTURE_2D, m_texture);
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
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                // Now store character for later use.
                Character character = {
                    m_texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
                };

                m_characters.insert(std::pair<char, Character>(c, character));
            }

            glBindTexture(GL_TEXTURE_2D, 0);

            // Destroy FreeType once we're finished.
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
        }

        void CreateVao()
        {
            glGenVertexArrays(1, &m_vao);
            glGenBuffers(1, &m_vbo);
            glBindVertexArray(m_vao);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    };
}
