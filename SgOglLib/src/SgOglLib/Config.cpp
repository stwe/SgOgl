// This file is part of the SgOgl package.
// 
// Filename: Config.cpp
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#include <tinyxml2.h>
#include "Config.h"
#include "Core.h"
#include "SgOglException.h"

bool sg::ogl::Config::ToBool(const std::string& t_value)
{
    SG_OGL_CORE_ASSERT(t_value == "0" || t_value == "1", "[Config::ToBool()] Invalid value.");
    return t_value == "1";
}

void sg::ogl::Config::LoadOptions(
    const std::string& t_fileName,
    std::string& t_libResFolder,
    WindowOptions& t_windowOptions,
    ProjectionOptions& t_projectionOptions
)
{
    Log::SG_OGL_CORE_LOG_DEBUG("[Config::LoadOptions()] Loading options from {}.", t_fileName);

    tinyxml2::XMLDocument document;

    const auto result{ document.LoadFile(t_fileName.c_str()) };
    if (result != tinyxml2::XML_SUCCESS)
    {
        throw SG_OGL_EXCEPTION("[Config::LoadOptions()] Error while loading Xml-Config file " + t_fileName + ".");
    }

    const auto* root{ document.FirstChildElement("init") };
    if (root)
    {
        const auto* libResFolder{ root->FirstChildElement("libResFolder") };
        if (libResFolder)
        {
            t_libResFolder = libResFolder->GetText();
        }
        else
        {
            throw SG_OGL_EXCEPTION("[Config::LoadOptions()] The <libResFolder> element could not be found.");
        }

        const auto* window{ root->FirstChildElement("window") };
        if (window)
        {
            t_windowOptions.title = window->FirstChildElement("title")->GetText();
            t_windowOptions.compatibleProfile = ToBool(window->FirstChildElement("compatibleProfile")->GetText());
            t_windowOptions.debugContext = ToBool(window->FirstChildElement("debugContext")->GetText());
            t_windowOptions.antialiasing = ToBool(window->FirstChildElement("antialiasing")->GetText());
            t_windowOptions.printFrameRate = ToBool(window->FirstChildElement("printFrameRate")->GetText());
            t_windowOptions.glMajor = std::strtol(window->FirstChildElement("glMajor")->GetText(), nullptr, 10);
            t_windowOptions.glMinor = std::strtol(window->FirstChildElement("glMinor")->GetText(), nullptr, 10);
            t_windowOptions.fps = std::strtod(window->FirstChildElement("fps")->GetText(), nullptr);
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

void sg::ogl::Config::LoadOptions(const std::string& t_fileName, TerrainOptions& t_terrainOptions)
{
    Log::SG_OGL_CORE_LOG_DEBUG("[Config::LoadOptions()] Loading terrain options from {}.", t_fileName);

    tinyxml2::XMLDocument document;

    const auto result{ document.LoadFile(t_fileName.c_str()) };
    if (result != tinyxml2::XML_SUCCESS)
    {
        throw SG_OGL_EXCEPTION("[Config::LoadOptions()] Error while loading Xml-Config file " + t_fileName + ".");
    }

    const auto* root{ document.FirstChildElement("terrain") };
    if (root)
    {
        const auto* position{ root->FirstChildElement("position") };
        if (position)
        {
            t_terrainOptions.xPos = position->FloatAttribute("x");
            t_terrainOptions.zPos = position->FloatAttribute("z");
        }
        else
        {
            throw SG_OGL_EXCEPTION("[Config::LoadOptions()] The <position> element could not be found.");
        }

        const auto* scale{ root->FirstChildElement("scale") };
        if (scale)
        {
            t_terrainOptions.scaleXz = scale->FloatAttribute("xz");
            t_terrainOptions.scaleY = scale->FloatAttribute("y");
        }
        else
        {
            throw SG_OGL_EXCEPTION("[Config::LoadOptions()] The <scale> element could not be found.");
        }

        const auto* heightmap{ root->FirstChildElement("heightmapPath") };
        if (heightmap)
        {
            t_terrainOptions.heightmapPath = root->FirstChildElement("heightmapPath")->GetText();
        }
        else
        {
            throw SG_OGL_EXCEPTION("[Config::LoadOptions()] The <heightmap> element could not be found.");
        }

        const auto* textures{ root->FirstChildElement("textures") };
        if (textures)
        {
            for (auto* texture{ textures->FirstChildElement() }; texture != nullptr; texture = texture->NextSiblingElement())
            {
                t_terrainOptions.textureContainer.emplace(texture->Attribute("name"), texture->Attribute("path"));
            }
        }
        else
        {
            throw SG_OGL_EXCEPTION("[Config::LoadOptions()] The <textures> element could not be found.");
        }

        const auto* normalmap{ root->FirstChildElement("normalmap") };
        if (normalmap)
        {
            t_terrainOptions.normalmapShaderName = normalmap->Attribute("shaderName");
            t_terrainOptions.normalmapTextureName = normalmap->Attribute("textureName");
        }
        else
        {
            throw SG_OGL_EXCEPTION("[Config::LoadOptions()] The <normalmap> element could not be found.");
        }

        const auto* splatmap{ root->FirstChildElement("splatmap") };
        if (splatmap)
        {
            t_terrainOptions.splatmapShaderName = splatmap->Attribute("shaderName");
            t_terrainOptions.splatmapTextureName = splatmap->Attribute("textureName");
        }
        else
        {
            throw SG_OGL_EXCEPTION("[Config::LoadOptions()] The <splatmap> element could not be found.");
        }

        const auto* strength{ root->FirstChildElement("normalStrength") };
        if (strength)
        {
            t_terrainOptions.normalStrength = static_cast<float>(std::strtod(root->FirstChildElement("normalStrength")->GetText(), nullptr));
        }
        else
        {
            throw SG_OGL_EXCEPTION("[Config::LoadOptions()] The <normalStrength> element could not be found.");
        }
    }
    else
    {
        throw SG_OGL_EXCEPTION("[Config::LoadOptions()] The <terrain> element could not be found.");
    }
}
