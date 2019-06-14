#pragma once

#include <string>

namespace sg::ogl
{
    //-------------------------------------------------
    // Window options
    //-------------------------------------------------

    struct WindowOptions
    {
        std::string title{ "" };
        bool showTriangles{ false };
        bool compatibleProfile{ false };
        bool debugContext{ false };
        bool antialiasing{ false };
        bool faceCulling{ true };
        bool frustumCulling{ false };
        bool printFrameRate{ false };
        int glMajor{ 3 };
        int glMinor{ 3 };
    };

    //-------------------------------------------------
    // Projection options
    //-------------------------------------------------

    struct ProjectionOptions
    {
        float fovDeg{ 0.0f };
        int width{ 0 };
        int height{ 0 };
        float nearPlane{ 0.0f };
        float farPlane{ 0.0f };
    };

    //-------------------------------------------------
    // Config
    //-------------------------------------------------

    class Config
    {
    public:
        static bool ToBool(const std::string& t_value);
        static void LoadOptions(const std::string& t_fileName, WindowOptions& t_windowOptions, ProjectionOptions& t_projectionOptions);

    protected:

    private:

    };
}
