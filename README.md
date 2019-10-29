# SgOgl

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/499a4726db5844449126e0101ac1074f)](https://app.codacy.com/app/stwe/SgOgl?utm_source=github.com&utm_medium=referral&utm_content=stwe/SgOgl&utm_campaign=Badge_Grade_Dashboard)

***

1. [What is does](#1-what-is-does)
2. [Installing](#2-installing)
3. [Using](#3-using)
a) [Create application class and entry point](#a-Create-application-class-and-entry-point)
b) [Load a model from a obj file](#b-Load-a-model-from-a-obj-file)
c) [Create a Skybox](#c-Create-a-Skybox)
d) [Create a Terrain](#d-Create-a-terrein)

***

## 1. What it does

A GameEngine library for OpenGL developed for educational purposes.

## 2. Installing

Install Visual Studio 2019 and use Premake5 and the premake5.lua file to create the project files.

## 3. Using

### a) Create application class and entry point

We need to create a class that inherits from `sg::ogl::Application`. Here we call this class `Sandbox`.
The header `SgOgl.h` must be included.

```cpp
// File: Sandbox.cpp

#include "SgOgl.h"

class Sandbox final : public sg::ogl::Application
{
public:
    Sandbox() = delete;

    explicit Sandbox(const std::string& t_configFileName)
        : Application{ t_configFileName }
    {
    }

protected:
    void RegisterStates() override
    {
        // ...
    }

    void Init() override
    {
        // ...
    }

private:

};
```

Next, a game state must be created. The class is named `GameState` and inherits from `sg::ogl::state::State`.

```cpp
// File: GameState.h

#pragma once

#include "SgOgl.h"

class GameState : public sg::ogl::state::State
{
public:
    GameState() = delete;

    explicit GameState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "GameState" }
    {
    }

    bool Input() override;
    bool Update(double t_dt) override;
    void Render() override;

protected:

private:

};

```

The cpp file contains nothing special.

```cpp
// File: GameState.cpp

#include "GameState.h"

bool GameState::Input()
{
    return true;
}

bool GameState::Update(const double t_dt)
{
    return true;
}

void GameState::Render()
{
}
```

The newly created game state must be registered. This is done by the `Sandbox` class in the `RegisterStates()` function.
In addition, the game state must be pushed to the state stack. We do this in the `Init()` function of the `Sandbox` class.
The `RegisterStates()` and `Init()` functions are called later before the game loop.

```cpp
// Sandbox.cpp

#include "SgOgl.h"
#include "GameState.h"

class Sandbox final : public sg::ogl::Application
{
public:
    // ...

protected:
    void RegisterStates() override
    {
        GetStateStack()->RegisterState<GameState>(sg::ogl::state::GAME);
    }

    void Init() override
    {
        GetStateStack()->PushState(sg::ogl::state::GAME);
    }

private:

};
```

The `main()` function of the lib calls `sg::ogl::create_application()` to create an `sg::ogl::Application`.
In our case, an instance of the `Sandbox` class should be created.

In the `Sandbox` class, the header `SgOglEntryPoint.h` must be included as well.

```cpp
// Sandbox.cpp

#include "SgOgl.h"
#include "SgOglEntryPoint.h"
#include "GameState.h"

class Sandbox final : public sg::ogl::Application
{
public:
    // ...

protected:
    // ...

private:

};

std::unique_ptr<sg::ogl::Application> sg::ogl::create_application()
{
    return std::make_unique<Sandbox>("res/config/Config.xml");
}

```

We can see that the constructor of the `Application` class or `Sandbox` class takes the path to a config file as an argument.
The config file can look like this.

```xml
<?xml version="1.0" encoding="utf-8"?>

<init>
    <window>
        <title>Sandbox</title>
        <showTriangles>0</showTriangles>
        <compatibleProfile>0</compatibleProfile>
        <debugContext>1</debugContext>
        <antialiasing>1</antialiasing>
        <printFrameRate>1</printFrameRate>
        <glMajor>4</glMajor>
        <glMinor>3</glMinor>
        <fps>60.0</fps>
    <projection>
        <fovDeg>70.0</fovDeg>
        <width>1024</width>
        <height>768</height>
        <near>0.1</near>
        <far>10000.0</far>
    </projection>
</init>
```

### b) Load a model from a obj file

The next step is render the model of a house. To do this we add to the `GameState` an entity, a render system, a scene and a camera. In addition, a private function `Init()` is required.

As shown below, two more classes are included. This is the `ModelShaderProgram` and the `ModelRenderSystem`.
We'll talk about the classes later.

```cpp
// File: GameState.h

#pragma once

#include "SgOgl.h"
#include "renderer/ModelRenderSystem.h"
#include "shader/ModelShaderProgram.h"

class GameState : public sg::ogl::state::State
{
public:
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;
    using CameraSharedPtr = std::shared_ptr<sg::ogl::camera::LookAtCamera>;

    GameState() = delete;

    explicit GameState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "GameState" }
    {
        Init();
    }

    bool Input() override;
    bool Update(double t_dt) override;
    void Render() override;

protected:

private:
    entt::entity m_entity;

    std::unique_ptr<ModelRenderSystem<ModelShaderProgram>> m_modelRenderSystem;

    SceneUniquePtr m_scene;
    CameraSharedPtr m_camera;

    void Init();
};
```

Let's start with the `Init()` method. Nothing is complicated here.
It will create a camera and a scene. Then the camera is added to the scene. The entity is created with the `registry`.
Now two components (ModelComponent and TransformComponent) are added to the entity.
All obj models are stored in the `ModelManager` as `std::shared_ptr<Model>`. The call `GetApplicationContext()->GetModelManager().GetModelByPath("res/model/House/farmhouse_obj.obj")` returns the smart pointer by value. When the method is called for the first time with this model, the model is preloaded and stored in the `ModelManager`.
Finally, the render system is created.

```cpp
// File: GameState.cpp

void GameState::Init()
{
    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>();
    m_camera->SetPosition(glm::vec3(0.0f, 10.0f, -40.0f));

    // create scene and set a camera
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // create an entity
    m_entity = GetApplicationContext()->registry.create();

    // add model component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::ModelComponent>(
        m_entity,
        GetApplicationContext()->GetModelManager().GetModelByPath("res/model/House/farmhouse_obj.obj")
    );

    // add transform component
    GetApplicationContext()->registry.assign<sg::ogl::ecs::component::TransformComponent>(m_entity);

    // create a render system for the entity
    m_modelRenderSystem = std::make_unique<ModelRenderSystem<ModelShaderProgram>>(m_scene.get());
}
```

The following call renders our entities.

```cpp
// File: GameState.cpp

void GameState::Render()
{
    m_modelRenderSystem->Render();
}
```

To control which entities should be rendered with which shader, the client must create two more classes and at least one vertex shader and a fragment shader.
Here again as a reminder of how the render system was declared:

```cpp
// File: GameState.h

class GameState : public sg::ogl::state::State
{
    // ...

private:
    std::unique_ptr<ModelRenderSystem<ModelShaderProgram>> m_modelRenderSystem;

    // ...
};
```

The first class creates a class for our shader program. The `UpdateUniforms()` method must be implemented. This determines which values ​​the uniforms receive. The `GetFolderName()` method needs to be implemented further. Here, the name is returned from the directory under `res/shader` where the shaders are stored. The ShaderManager automatically loads the shaders and finds all the uniforms themselves. There are conventions for naming the shaders. See the examples on Github.

```cpp
// File: ModelShaderProgram.cpp

class ModelShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(const sg::ogl::scene::Scene& t_scene, const entt::entity t_entity, const sg::ogl::resource::Mesh& t_currentMesh) override
    {
        auto& transformComponent = t_scene.GetApplicationContext()->registry.get<sg::ogl::ecs::component::TransformComponent>(t_entity);

        const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };
        const auto mvp{ projectionMatrix * t_scene.GetCurrentCamera().GetViewMatrix() * static_cast<glm::mat4>(transformComponent) };

        SetUniform("modelMatrix", static_cast<glm::mat4>(transformComponent));
        SetUniform("plane", glm::vec4(0.0f));
        SetUniform("mvpMatrix", mvp);
        SetUniform("ambientIntensity", glm::vec3(1.0f));
        SetUniform("diffuseColor", t_currentMesh.GetDefaultMaterial()->kd);
        SetUniform("hasDiffuseMap", t_currentMesh.GetDefaultMaterial()->HasDiffuseMap());

        if (t_currentMesh.GetDefaultMaterial()->HasDiffuseMap())
        {
            SetUniform("diffuseMap", 0);
            sg::ogl::resource::TextureManager::BindForReading(t_currentMesh.GetDefaultMaterial()->mapKd, GL_TEXTURE0);
        }
    }

    std::string GetFolderName() override
    {
        return "model"; // that means: load Vertex.vert and Fragment.frag from res/shader/model
    }
};
```

Finally the "ModelRenderSystem". It iterates and renders all entities with specific components.

```cpp
// File: ModelRenderSystem.cpp

template <typename TShaderProgram>
class ModelRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit ModelRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(double t_dt) override {}

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<sg::ogl::ecs::component::ModelComponent, sg::ogl::ecs::component::TransformComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            auto& modelComponent = view.get<sg::ogl::ecs::component::ModelComponent>(entity);

            for (auto& mesh : modelComponent.model->GetMeshes())
            {
                mesh->InitDraw();

                shaderProgram.UpdateUniforms(*m_scene, entity, *mesh);

                mesh->DrawPrimitives();
                mesh->EndDraw();
            }
        }

        sg::ogl::resource::ShaderProgram::Unbind();

        FinishRendering();
    }

protected:
    void PrepareRendering() override {}
    void FinishRendering() override {}
};
```
