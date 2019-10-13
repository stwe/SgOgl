#include <glm/vec3.hpp>
#include "SceneLoader.h"
#include "SgOglException.h"
#include "Application.h"
#include "Scene.h"
#include "resource/Material.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::SceneLoader::SceneLoader(const std::string& t_configFileName)
{
    LoadConfigFile(t_configFileName);
}

sg::ogl::scene::SceneLoader::~SceneLoader() noexcept
{
}

//-------------------------------------------------
// Loader
//-------------------------------------------------

void sg::ogl::scene::SceneLoader::LoadAtmosphere(Scene* t_scene, AtmosphereContainer& t_atmosphere) const
{
    auto* skyboxElement{ m_atmosphereElement->FirstChildElement("skybox") };
    if (skyboxElement)
    {
        const auto shaderFolderName{ skyboxElement->FirstChildElement("shader_folder_name")->GetText() };
        const auto size{ static_cast<float>(std::strtod(skyboxElement->FirstChildElement("size")->GetText(), nullptr)) };

        // load cubemap element
        auto* cubemapElement{ skyboxElement->FirstChildElement("cubemap") };

        // get texture names
        std::vector<std::string> textureFileNames;
        for (auto* textureElement{ cubemapElement->FirstChildElement() }; textureElement != nullptr; textureElement = textureElement->NextSiblingElement())
        {
            textureFileNames.emplace_back(textureElement->GetText());
        }

        t_atmosphere.emplace("skybox", t_scene->CreateSkyboxEntity(textureFileNames, shaderFolderName, size));
    }

    auto* skydomeElement{ m_atmosphereElement->FirstChildElement("skydome") };
    if (skydomeElement)
    {
        const auto model{ skydomeElement->FirstChildElement("model")->GetText() };
        const auto shaderFolderName{ skydomeElement->FirstChildElement("shader_folder_name")->GetText() };

        t_atmosphere.emplace("skydome", t_scene->CreateSkydomeEntity(model, shaderFolderName));
    }
}

void sg::ogl::scene::SceneLoader::LoadMaterials(Application* t_application, MaterialContainer& t_materials) const
{
    for (auto* materialElement{ m_materialsElement->FirstChildElement() }; materialElement != nullptr; materialElement = materialElement->NextSiblingElement())
    {
        const auto name{ materialElement->FirstChildElement("name")->GetText() };
        const auto diffTexture{ materialElement->FirstChildElement("map_kd")->GetText() };

        auto material{ std::make_shared<resource::Material>() };
        material->mapKd = t_application->GetTextureManager()->GetTextureIdFromPath(diffTexture);

        t_materials.emplace(name, material);
    }
}

void sg::ogl::scene::SceneLoader::LoadEntities(Scene* t_scene, EntityContainer& t_entities, const MaterialContainer& t_materials) const
{
    for (auto* entityElement{ m_entitiesElement->FirstChildElement() }; entityElement != nullptr; entityElement = entityElement->NextSiblingElement())
    {
        const auto model{ entityElement->FirstChildElement("model")->GetText() };
        const auto shaderFolderName{ entityElement->FirstChildElement("shader_folder_name")->GetText() };

        auto* positionElement{ entityElement->FirstChildElement("position") };
        auto* rotationElement{ entityElement->FirstChildElement("rotation") };
        auto* scaleElement{ entityElement->FirstChildElement("scale") };
        auto* alternativeMaterial{ entityElement->FirstChildElement("alternative_material_name") };

        Entity* entity{ nullptr };
        if (alternativeMaterial)
        {
            entity = t_scene->CreateModelEntity(model, shaderFolderName, t_materials.at(alternativeMaterial->GetText()));
        }
        else
        {
            entity = t_scene->CreateModelEntity(model, shaderFolderName);
        }

        if (positionElement)
        {
            entity->GetLocalTransform().position = glm::vec3(positionElement->FloatAttribute("x"), positionElement->FloatAttribute("y"), positionElement->FloatAttribute("z"));
        }

        if (rotationElement)
        {
            entity->GetLocalTransform().rotation = glm::vec3(rotationElement->FloatAttribute("x"), rotationElement->FloatAttribute("y"), rotationElement->FloatAttribute("z"));
        }

        if (scaleElement)
        {
            entity->GetLocalTransform().scale = glm::vec3(scaleElement->FloatAttribute("x"), scaleElement->FloatAttribute("y"), scaleElement->FloatAttribute("z"));
        }

        t_entities.push_back(entity);
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::scene::SceneLoader::LoadConfigFile(const std::string& t_configFileName)
{
    // load config file
    const auto result{ m_document.LoadFile(t_configFileName.c_str()) };
    if (result != tinyxml2::XML_SUCCESS)
    {
        throw SG_OGL_EXCEPTION("[SceneLoader::LoadConfigFile()] Error while loading Xml-Config file " + t_configFileName + ".");
    }

    // get main elements
    m_rootElement = m_document.FirstChildElement("scene");
    if (m_rootElement)
    {
        m_atmosphereElement = m_rootElement->FirstChildElement("atmosphere");
        if (!m_atmosphereElement)
        {
            throw SG_OGL_EXCEPTION("[SceneLoader::LoadConfigFile()] The <atmosphere> element could not be found.");
        }

        m_materialsElement = m_rootElement->FirstChildElement("materials");
        if (!m_materialsElement)
        {
            throw SG_OGL_EXCEPTION("[SceneLoader::LoadConfigFile()] The <materials> element could not be found.");
        }

        m_entitiesElement = m_rootElement->FirstChildElement("entities");
        if (!m_entitiesElement)
        {
            throw SG_OGL_EXCEPTION("[SceneLoader::LoadConfigFile()] The <entities> element could not be found.");
        }
    }
    else
    {
        throw SG_OGL_EXCEPTION("[SceneLoader::LoadConfigFile()] The <scene> element could not be found.");
    }
}
