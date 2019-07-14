#pragma once

#include <glm/mat4x4.hpp>
#include <string>
#include <memory>
#include "Core.h"

namespace sg::ogl::resource
{
    class Model;
    class ShaderManager;
    class TextureManager;
}

namespace sg::ogl::camera
{
    class LookAtCamera;
}

namespace sg::ogl::math
{
    struct Transform;
}

namespace sg::ogl::renderer
{
    class SG_OGL_API ModelRenderer
    {
    public:
        using ModelUniquePtr = std::unique_ptr<resource::Model>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        ModelRenderer() = delete;

        ModelRenderer(
            resource::ShaderManager& t_shaderManager,
            resource::TextureManager& t_textureManager,
            camera::LookAtCamera& t_camera,
            glm::mat4& t_projection
        );

        ModelRenderer(const ModelRenderer& t_other) = delete;
        ModelRenderer(ModelRenderer&& t_other) noexcept = delete;
        ModelRenderer& operator=(const ModelRenderer& t_other) = delete;
        ModelRenderer& operator=(ModelRenderer&& t_other) noexcept = delete;

        ~ModelRenderer() noexcept = default;

        //-------------------------------------------------
        // Render
        //-------------------------------------------------

        void Render(resource::Model& t_model, math::Transform& t_transform, const std::string& t_shaderProgramName) const;
        void RenderNormals(resource::Model& t_model, math::Transform& t_transform, const std::string& t_shaderProgramName, float t_normalLength) const;

    protected:

    private:
        resource::ShaderManager& m_shaderManager;
        resource::TextureManager& m_textureManager;
        camera::LookAtCamera& m_camera;
        glm::mat4 m_projectionMatrix;
    };
}
