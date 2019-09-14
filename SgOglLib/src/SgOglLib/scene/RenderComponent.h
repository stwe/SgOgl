#pragma once

#include <memory>
#include "Component.h"
#include "Core.h"
#include "OpenGl.h"
#include "resource/ShaderProgram.h"

namespace sg::ogl::scene
{
    class SG_OGL_API RenderConfig
    {
    public:
        RenderConfig() = delete;

        explicit RenderConfig(resource::ShaderProgram& t_shaderProgram)
            : m_shaderProgram{ t_shaderProgram }
        {}

        RenderConfig(const RenderConfig& t_other) = delete;
        RenderConfig(RenderConfig&& t_other) noexcept = delete;
        RenderConfig& operator=(const RenderConfig& t_other) = delete;
        RenderConfig& operator=(RenderConfig&& t_other) noexcept = delete;

        virtual ~RenderConfig() noexcept = default;

        virtual void Enable() = 0;
        virtual void Disable() = 0;

        virtual void Init() {}

        resource::ShaderProgram& GetShaderProgram() { return m_shaderProgram; }
        const resource::ShaderProgram& GetShaderProgram() const { return m_shaderProgram; }

    protected:

    private:
        resource::ShaderProgram& m_shaderProgram;
    };

    class SG_OGL_API DefaultRenderConfig : public RenderConfig
    {
    public:
        explicit DefaultRenderConfig(resource::ShaderProgram& t_shaderProgram)
            : RenderConfig(t_shaderProgram)
        {}

        void Enable() override {}
        void Disable() override {}

        void Init() override
        {
            // todo
        }

    protected:

    private:

    };

    class SG_OGL_API SkyboxRenderConfig : public RenderConfig
    {
    public:
        explicit SkyboxRenderConfig(resource::ShaderProgram& t_shaderProgram)
            : RenderConfig(t_shaderProgram)
        {}

        void Enable() override
        {
            // save old depth func mode
            glGetIntegerv(GL_DEPTH_FUNC, &m_oldDepthFuncMode);

            // change depth func
            glDepthFunc(GL_LEQUAL);
        }

        void Disable() override
        {
            // restore old depth func mode
            glDepthFunc(m_oldDepthFuncMode);
        }

    protected:

    private:
        int32_t m_oldDepthFuncMode;
    };

    class SG_OGL_API RenderComponent : public Component
    {
    public:
        using RenderConfigSharedPtr = std::shared_ptr<RenderConfig>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        RenderComponent() = default;

        RenderComponent(const RenderComponent& t_other) = delete;
        RenderComponent(RenderComponent&& t_other) noexcept = delete;
        RenderComponent& operator=(const RenderComponent& t_other) = delete;
        RenderComponent& operator=(RenderComponent&& t_other) noexcept = delete;

        ~RenderComponent() noexcept = default;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        const RenderConfig& GetRenderConfig() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetRenderConfig(const RenderConfigSharedPtr& t_renderConfig);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Render() override;

    protected:

    private:
        RenderConfigSharedPtr m_renderConfig;
    };
}
