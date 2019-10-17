#pragma once

#include <string>
#include <utility>
#include "Core.h"

namespace sg::ogl::scene
{
    class SG_OGL_API RenderConfig
    {
    public:
        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        /**
         * @brief Directory name under which the shader files are stored.
         *        This will later be used to get the ShaderProgram from
         *        the ShaderManager.
         */
        std::string shaderFolder;

        /**
         * @brief Specifies what kind of primitives to render.
         */
        uint32_t drawMode{ GL_TRIANGLES };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        RenderConfig() = default;

        explicit RenderConfig(std::string t_shaderFolder)
            : shaderFolder{ std::move(t_shaderFolder) }
        {}

        RenderConfig(const RenderConfig& t_other) = delete;
        RenderConfig(RenderConfig&& t_other) noexcept = delete;
        RenderConfig& operator=(const RenderConfig& t_other) = delete;
        RenderConfig& operator=(RenderConfig&& t_other) noexcept = delete;

        virtual ~RenderConfig() noexcept = default;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        virtual void InitRendering() {}
        virtual void PrepareRendering() {}
        virtual void FinishRendering() {}

    protected:

    private:

    };
}
