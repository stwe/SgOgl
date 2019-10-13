#pragma once

#include <string>
#include <tinyxml2.h>
#include <vector>
#include <map>
#include <memory>

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::resource
{
    struct Material;
}

namespace sg::ogl::scene
{
    class Entity;
    class Scene;

    class SceneLoader
    {
    public:
        // atmosphere
        using AtmosphereKey = std::string;
        using AtmosphereContainer = std::map<AtmosphereKey, Entity*>;

        // materials
        using MaterialKey = std::string;
        using MaterialSharedPtr = std::shared_ptr<resource::Material>;
        using MaterialContainer = std::map<MaterialKey, MaterialSharedPtr>;

        // entities
        using EntityContainer = std::vector<Entity*>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        SceneLoader() = delete;

        explicit SceneLoader(const std::string& t_configFileName);

        SceneLoader(const SceneLoader& t_other) = delete;
        SceneLoader(SceneLoader&& t_other) noexcept = delete;
        SceneLoader& operator=(const SceneLoader& t_other) = delete;
        SceneLoader& operator=(SceneLoader&& t_other) noexcept = delete;

        ~SceneLoader() noexcept;

        //-------------------------------------------------
        // Load
        //-------------------------------------------------

        void LoadAtmosphere(Scene* t_scene, AtmosphereContainer& t_atmosphere) const;
        void LoadMaterials(Application* t_application, MaterialContainer& t_materials) const;
        void LoadEntities(Scene* t_scene, EntityContainer& t_entities, const MaterialContainer& t_materials) const;

    protected:

    private:
        tinyxml2::XMLDocument m_document;
        tinyxml2::XMLElement* m_rootElement{ nullptr };
        tinyxml2::XMLElement* m_atmosphereElement{ nullptr };
        tinyxml2::XMLElement* m_materialsElement{ nullptr };
        tinyxml2::XMLElement* m_entitiesElement{ nullptr };

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void LoadConfigFile(const std::string& t_configFileName);
    };
}
