#include "Config.h"
#include "Core.h"
#include "SgException.h"
#include "tinyxml2.h"

bool sg::ogl::Config::ToBool(const std::string& t_value)
{
    SG_OGL_CORE_ASSERT(t_value == "0" || t_value == "1", "[Config::ToBool()] Assertion failed.")
    return t_value == "1";
}

void sg::ogl::Config::LoadOptions(const std::string& t_fileName, WindowOptions& t_windowOptions, ProjectionOptions& t_projectionOptions)
{
    SG_OGL_CORE_LOG_DEBUG("[Config::LoadOptions()] Loading options from {}.", t_fileName);

    tinyxml2::XMLDocument document;

    const auto result{ document.LoadFile(t_fileName.c_str()) };
    if (result != tinyxml2::XML_SUCCESS)
    {
        SG_OGL_EXCEPTION("[Config::LoadOptions()] A XMLError has occurred.");
    }

    const auto* root{ document.FirstChildElement("init") };
    if (root)
    {
        const auto* window{ root->FirstChildElement("window") };
        if (window)
        {
            t_windowOptions.title = window->FirstChildElement("title")->GetText();
            t_windowOptions.showTriangles = ToBool(window->FirstChildElement("showTriangles")->GetText());
            t_windowOptions.compatibleProfile = ToBool(window->FirstChildElement("compatibleProfile")->GetText());
            t_windowOptions.debugContext = ToBool(window->FirstChildElement("debugContext")->GetText());
            t_windowOptions.antialiasing = ToBool(window->FirstChildElement("antialiasing")->GetText());
            t_windowOptions.faceCulling = ToBool(window->FirstChildElement("faceCulling")->GetText());
            t_windowOptions.frustumCulling = ToBool(window->FirstChildElement("frustumCulling")->GetText());
            t_windowOptions.printFrameRate = ToBool(window->FirstChildElement("printFrameRate")->GetText());
        }
        else
        {
            SG_OGL_EXCEPTION("[Config::LoadOptions()] The <window> element could not be found.");
        }

        const auto* projection{ root->FirstChildElement("projection") };
        if (projection)
        {
            t_projectionOptions.fovDeg = static_cast<float>(std::strtod(projection->FirstChildElement("fovDeg")->GetText(), nullptr));
            t_projectionOptions.width = std::strtol(projection->FirstChildElement("width")->GetText(), nullptr, 10);
            t_projectionOptions.height = std::strtol(projection->FirstChildElement("height")->GetText(), nullptr, 10);
            t_projectionOptions.nearPlane = static_cast<float>(std::strtod(projection->FirstChildElement("near")->GetText(), nullptr));
            t_projectionOptions.farPlane = static_cast<float>(std::strtod(projection->FirstChildElement("far")->GetText(), nullptr));
        }
        else
        {
            SG_OGL_EXCEPTION("[Config::LoadOptions()] The <projection> element could not be found.");
        }
    }
    else
    {
        SG_OGL_EXCEPTION("[Config::LoadOptions()] The <init> element could not be found.");
    }
}
