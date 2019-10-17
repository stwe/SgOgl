#pragma once

#include <cstdint>
#include "Core.h"

namespace sg::ogl::buffer
{
    class SG_OGL_API Fbo
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Fbo();

        Fbo(const Fbo& t_other) = delete;
        Fbo(Fbo&& t_other) noexcept = delete;
        Fbo& operator=(const Fbo& t_other) = delete;
        Fbo& operator=(Fbo&& t_other) noexcept = delete;

        ~Fbo() noexcept = default;

        //-------------------------------------------------
        // Fbo
        //-------------------------------------------------

        void GenerateFbo();
        void BindFbo() const;
        void UnbindFbo();
        void DeleteFbo() const;

        void BindAsRenderTarget() const;

        //-------------------------------------------------
        // Attachment
        //-------------------------------------------------

        void AttachTextures();

    protected:

    private:
        uint32_t m_fboId{ 0 };

        uint32_t m_colorTexture{ 0 };
        uint32_t m_depthTexture{ 0 };

        int32_t m_width{ 0 };
        int32_t m_height{ 0 };
    };
}
