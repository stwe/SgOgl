#pragma once

#include <cstdint>
#include "Core.h"

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::buffer
{
    class SG_OGL_API Fbo
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Fbo() = delete;

        Fbo(Application* t_application, int32_t t_width, int32_t t_height);

        Fbo(const Fbo& t_other) = delete;
        Fbo(Fbo&& t_other) noexcept = delete;
        Fbo& operator=(const Fbo& t_other) = delete;
        Fbo& operator=(Fbo&& t_other) noexcept = delete;

        ~Fbo() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        uint32_t GetDepthTextureId() const;

        //-------------------------------------------------
        // Fbo
        //-------------------------------------------------

        void GenerateFbo();
        void BindFbo() const;
        static void UnbindFbo();

        //-------------------------------------------------
        // Fbo - Render Target
        //-------------------------------------------------

        void BindAsRenderTarget() const;
        void UnbindRenderTarget() const;

    protected:

    private:
        Application* m_application{ nullptr };

        uint32_t m_fboId{ 0 };

        uint32_t m_depthTexture{ 0 };

        int32_t m_width{ 0 };
        int32_t m_height{ 0 };

        //-------------------------------------------------
        // Attachment
        //-------------------------------------------------

        void Attach();

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp() const;
    };
}
