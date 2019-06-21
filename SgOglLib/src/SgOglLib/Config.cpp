#include <tinyxml2.h>
#include "Config.h"
#include "Core.h"
#include "SgOglException.h"
#include "Log.h"

bool sg::ogl::Config::ToBool(const std::string& t_value)
{
    SG_OGL_CORE_ASSERT(t_value == "0" || t_value == "1", "[Config::ToBool()] Invalid value.")
    return t_value == "1";
}

void sg::ogl::Config::LoadOptions(const std::string& t_fileName, WindowOptions& t_windowOptions, ProjectionOptions& t_projectionOptions)
{
    SG_OGL_CORE_LOG_DEBUG("[Config::LoadOptions()] Loading options from {}.", t_fileName);

    tinyxml2::XMLDocument document;

    const auto result{ document.LoadFile(t_fileName.c_str()) };
    if (result != tinyxml2::XML_SUCCESS)
    {
        throw SG_OGL_EXCEPTION("[Config::LoadOptions()] Error while loading Xml-Config file " + t_fileName + ".");
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
            t_windowOptions.glMajor = std::strtol(window->FirstChildElement("glMajor")->GetText(), nullptr, 10);
            t_windowOptions.glMinor = std::strtol(window->FirstChildElement("glMinor")->GetText(), nullptr, 10);
        }
        else
        {
            throw SG_OGL_EXCEPTION("[Config::LoadOptions()] The <window> element could not be found.");
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
            throw SG_OGL_EXCEPTION("[Config::LoadOptions()] The <projection> element could not be found.");
        }
    }
    else
    {
        throw SG_OGL_EXCEPTION("[Config::LoadOptions()] The <init> element could not be found.");
    }
}
