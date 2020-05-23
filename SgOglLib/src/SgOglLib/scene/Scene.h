// This file is part of the SgOgl package.
// 
// Filename: Scene.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

struct lua_State;

namespace sg::ogl
{
    class Application;
}

namespace sg::ogl::light
{
    struct DirectionalLight;
}

namespace sg::ogl::camera
{
    class Camera;
}

namespace sg::ogl::ecs::system
{
    class RenderSystemInterface;
}

namespace sg::ogl::water
{
    class Water;
}

namespace sg::ogl::scene
{
    class Scene
    {
    public:
        using CameraSharedPtr = std::shared_ptr<camera::Camera>;
        using CameraContainer = std::unordered_map<std::string, CameraSharedPtr>;

        using RenderSystemSharedPtr = std::shared_ptr<ecs::system::RenderSystemInterface>;
        using RendererMap = std::unordered_map<std::string, RenderSystemSharedPtr>;
        using RendererArray = std::vector<RenderSystemSharedPtr>;

        using DirectionalLightSharedPtr = std::shared_ptr<light::DirectionalLight>;

        using WaterSharedPtr = std::shared_ptr<water::Water>;
        using WaterContainer = std::unordered_map<std::string, WaterSharedPtr>;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Scene() = delete;

        explicit Scene(Application* t_application);

        Scene(Application* t_application, const std::string& t_configFileName);

        Scene(const Scene& t_other) = delete;
        Scene(Scene&& t_other) noexcept = delete;
        Scene& operator=(const Scene& t_other) = delete;
        Scene& operator=(Scene&& t_other) noexcept = delete;

        ~Scene() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] Application* GetApplicationContext() const;

        [[nodiscard]] camera::Camera& GetCurrentCamera() noexcept;
        [[nodiscard]] const camera::Camera& GetCurrentCamera() const noexcept;

        [[nodiscard]] [[deprecated]] light::DirectionalLight& GetCurrentDirectionalLight() noexcept;
        [[nodiscard]] [[deprecated]] const light::DirectionalLight& GetCurrentDirectionalLight() const noexcept;

        [[nodiscard]] [[deprecated]] bool HasDirectionalLight() const;

        [[nodiscard]] glm::vec4 GetCurrentClipPlane() const;

        [[nodiscard]] glm::vec3 GetAmbientIntensity() const;

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        void SetAmbientIntensity(const glm::vec3& t_ambientIntensity);
        void SetCurrentCamera(const CameraSharedPtr& t_camera);
        void SetCurrentDirectionalLight(const DirectionalLightSharedPtr& t_directionalLight);
        void SetCurrentClipPlane(const glm::vec4& t_currentClipPlane);

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Input();
        void Update(double t_dt);
        void Render();

    protected:

    private:
        Application* m_application{ nullptr };

        std::string m_configFileName;
        CameraContainer m_cameras;
        RendererMap m_rendererMap;
        RendererArray m_rendererArray;
        WaterContainer m_waterContainer;

        CameraSharedPtr m_currentCamera;

        DirectionalLightSharedPtr m_currentDirectionalLight;

        glm::vec4 m_currentClipPlane{ glm::vec4(0.0f, -1.0f, 0.0f, 100000.0f) };
        glm::vec3 m_ambientIntensity{ glm::vec3(0.3f) };

        //-------------------------------------------------
        // Lua config file
        //-------------------------------------------------

        void AddCamera(lua_State* t_luaState, const std::string& t_cameraName);
        void AddEntity(lua_State* t_luaState, const std::string& t_entityName);
        void AddRenderer(int t_priority, const std::string& t_rendererName);

        void ConfigSceneFromFile();
    };
}
