#pragma once

#include <cstdint>
#include "Core.h"

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::buffer
{
    class SG_OGL_API WaterFbos
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        WaterFbos() = delete;

        explicit WaterFbos(Application* t_application);

        WaterFbos(const WaterFbos& t_other) = delete;
        WaterFbos(WaterFbos&& t_other) noexcept = delete;
        WaterFbos& operator=(const WaterFbos& t_other) = delete;
        WaterFbos& operator=(WaterFbos&& t_other) noexcept = delete;

        ~WaterFbos() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        uint32_t GetReflectionFboId() const;
        uint32_t GetReflectionColorTextureId() const;

        uint32_t GetRefractionFboId() const;
        uint32_t GetRefractionColorTextureId() const;
        uint32_t GetRefractionDepthTextureId() const;

        //-------------------------------------------------
        // Fbo
        //-------------------------------------------------

        static uint32_t GenerateAndBindFbo();
        static void BindFbo(uint32_t t_fboId);
        static void UnbindFbo();

        //-------------------------------------------------
        // Fbo - Render Target
        //-------------------------------------------------

        static void BindAsRenderTarget(uint32_t t_fbo, int32_t t_width, int32_t t_height);

        void BindReflectionFboAsRenderTarget() const;
        void BindRefractionFboAsRenderTarget() const;

        void UnbindRenderTarget() const;

    protected:

    private:
        Application* m_application{ nullptr };

        // geometry which is above the water surface
        int32_t m_reflectionWidth{ 0 };
        int32_t m_reflectionHeight{ 0 };
        uint32_t m_reflectionFboId{ 0 };
        uint32_t m_reflectionColorTextureId{ 0 };
        uint32_t m_reflectionDepthRenderBufferId{ 0 }; // write-only

        // geometry which is under the water surface
        int32_t m_refractionWidth{ 0 };
        int32_t m_refractionHeight{ 0 };
        uint32_t m_refractionFboId{ 0 };
        uint32_t m_refractionColorTextureId{ 0 };
        uint32_t m_refractionDepthTextureId{ 0 };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void InitReflectionFbo();
        void InitRefractionFbo();

        static uint32_t CreateColorTextureAttachment(int32_t t_width, int32_t t_height);
        static uint32_t CreateDepthTextureAttachment(int32_t t_width, int32_t t_height);
        static uint32_t CreateDepthRenderBufferAttachment(int32_t t_width, int32_t t_height);

        static void CheckFbo();

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp() const;
    };
}
