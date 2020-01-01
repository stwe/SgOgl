// This file is part of the SgOgl package.
// 
// Filename: GBufferFbo.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <cstdint>

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::buffer
{
    class GBufferFbo
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        GBufferFbo() = delete;

        explicit GBufferFbo(Application* t_application);

        GBufferFbo(const GBufferFbo& t_other) = delete;
        GBufferFbo(GBufferFbo&& t_other) noexcept = delete;
        GBufferFbo& operator=(const GBufferFbo& t_other) = delete;
        GBufferFbo& operator=(GBufferFbo&& t_other) noexcept = delete;

        ~GBufferFbo() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] uint32_t GetFboId() const;

        [[nodiscard]] uint32_t GetPositionTextureId() const;
        [[nodiscard]] uint32_t GetNormalTextureId() const;
        [[nodiscard]] uint32_t GetAlbedoSpecTextureId() const;

        //-------------------------------------------------
        // Fbo
        //-------------------------------------------------

        void BindFbo() const;
        static void UnbindFbo();
        void CopyDepthBufferToDefaultFramebuffer() const;

    protected:

    private:
        Application* m_application{ nullptr };

        int32_t m_width{ 0 };
        int32_t m_height{ 0 };

        uint32_t m_fboId{ 0 };

        uint32_t m_positionTextureId{ 0 };
        uint32_t m_normalTextureId{ 0 };
        uint32_t m_albedoSpecTextureId{ 0 };

        //-------------------------------------------------
        // Fbo
        //-------------------------------------------------

        void GenerateFbo();

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
