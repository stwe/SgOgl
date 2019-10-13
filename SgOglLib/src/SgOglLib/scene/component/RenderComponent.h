#pragma once

#include <memory>
#include "Core.h"
#include "scene/Component.h"

namespace sg::ogl::scene
{
    class RenderConfig;
}

namespace sg::ogl::scene::component
{
    class SG_OGL_API RenderComponent : public Component
    {
    public:
        using RenderConfigSharedPtr = std::shared_ptr<RenderConfig>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        RenderComponent();

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

        void Init() override {}
        void Input() override {}
        void Update() override {}
        void Render() override;

    protected:

    private:
        /**
         * @brief A RenderConfig object to prepare and finish the rendering process.
         */
        RenderConfigSharedPtr m_renderConfig;
    };
}
