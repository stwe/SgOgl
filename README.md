# SgOgl

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/499a4726db5844449126e0101ac1074f)](https://app.codacy.com/app/stwe/SgOgl?utm_source=github.com&utm_medium=referral&utm_content=stwe/SgOgl&utm_campaign=Badge_Grade_Dashboard)

***

1. [What is does](#1-what-is-does)
1. [Installing](#2-installing)
1. [Getting started](#3-getting-started)
    1. [Create application class and entry point](#a-create-application-class-and-entry-point)
    1. [Load a model from a obj file](#b-load-a-model-from-a-obj-file)
    1. [Create a Skybox](#c-create-a-Skybox)
    1. [Create a Terrain](#d-create-a-terrain)
    1. [Create a Gui](#e-create-a-gui)
1. [Advanced](#4-advanced)
    1. [Instancing](#a-instancing)
    1. [Water rendering](#b-water-rendering)
    1. [Particle systems](#c-particle-systems)
    1. [Lighting](#d-lighting)

***

## 1. What it does

A GameEngine library for OpenGL developed for educational purposes.

## 2. Installing

Install Visual Studio 2019 and use Premake5 and the premake5.lua file to create the project files.

## 3. Getting started

In the next few points, a skybox, a terrain with a water surface and several models are loaded.
In addition, two particle systems are created (smoke and fire). The result should look something like this.

![Result](https://github.com/stwe/SgOgl/blob/master/Sandbox/res/devlog/TestAll.png)

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

The next step is to render two obj models. To do this we add to the `GameState` a render system, a scene and a camera. In addition, a private function `Init()` is required.

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
    SceneUniquePtr m_scene;
    CameraSharedPtr m_camera;

    std::unique_ptr<ModelRenderSystem<ModelShaderProgram>> m_modelRenderSystem;

    void Init();
};
```

Let's start with the `Init()` method. Nothing is complicated here.
It will create a camera and a scene. Then the camera is added to the scene and the `ModelRenderSystem` is created.

```cpp
// File: GameState.cpp

void GameState::Init()
{
    // create camera and set a camera position
    m_camera = std::make_shared<sg::ogl::camera::LookAtCamera>(glm::vec3(-1163.0f, 119.0f, -2246.0f), 74.0f, -9.0f);

    // create scene and set the camera as current
    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext());
    m_scene->SetCurrentCamera(m_camera);

    // create render systems
    m_modelRenderSystem = std::make_unique<ModelRenderSystem<ModelShaderProgram>>(m_scene.get());
}
```

The `ModelRenderSystem` renders entities with specific components. Entities for this render system can be created as follows:


```cpp
// File: GameState.cpp

void GameState::Init()
{
    // ...

    // create house entity
    //auto height{ m_terrain->GetHeightAtWorldPosition(-1000.0f, -2000.0f) };
    auto height{ 0.0f };
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/House/farmhouse_obj.obj",
        glm::vec3(-1000.0f, height, -2000.0f),
        glm::vec3(0.0f),
        glm::vec3(2.0f)
    );

    // create tree entity
    //height = m_terrain->GetHeightAtWorldPosition(-1090.0f, -2060.0f);
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Tree_02/tree02.obj",
        glm::vec3(-1090.0f, height, -2060.0f),
        glm::vec3(0.0f),
        glm::vec3(64.0f)
    );
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

The first class creates a class for our shader program. The `UpdateUniforms()` method must be implemented. This determines which values ​​the uniforms receive. The `GetFolderName()` method needs to be implemented further. Here, the name is returned from the directory under `res/shader` where the shaders are stored. The `ShaderManager` automatically loads the shaders and finds all the uniforms themselves. There are conventions for naming the shaders. See the examples on Github.


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
        SetUniform("plane", t_scene.GetCurrentClipPlane());
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
        return "model";
    }
};
```

Finally the `ModelRenderSystem`. It iterates and renders all entities with specific components.

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

        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::ModelComponent,
            sg::ogl::ecs::component::TransformComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            // todo
            if (!m_scene->GetApplicationContext()->registry.has<sg::ogl::ecs::component::SkydomeComponent>(entity))
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
        }

        sg::ogl::resource::ShaderProgram::Unbind();

        FinishRendering();
    }

protected:
    void PrepareRendering() override {}
    void FinishRendering() override {}
};
```

### c) Create a Skybox

As with the obj model a render system is needed.

```cpp
// File: GameState.h

#pragma once

#include "SgOgl.h"
#include "renderer/SkyboxRenderSystem.h"
#include "shader/SkyboxShaderProgram.h"

class GameState : public sg::ogl::state::State
{
public:

    // ...

private:
    // ...

    std::unique_ptr<SkyboxRenderSystem<SkyboxShaderProgram>> m_skyboxRenderSystem;
};
```

Call the render system for the Skybox. The Skyboy itself is created in the `Init()` method.

```cpp
// File: GameState.cpp

void GameState::Render()
{
    m_modelRenderSystem->Render();
    m_skyboxRenderSystem->Render();
}

void GameState::Init()
{
    // ...

    m_skyboxRenderSystem = std::make_unique<SkyboxRenderSystem<SkyboxShaderProgram>>(m_scene.get());

    // create skybox entity
    const std::vector<std::string> cubemapFileNames{
        "res/texture/sky/sRight.png",
        "res/texture/sky/sLeft.png",
        "res/texture/sky/sUp.png",
        "res/texture/sky/sDown.png",
        "res/texture/sky/sBack.png",
        "res/texture/sky/sFront.png"
    };
    GetApplicationContext()->GetEntityFactory().CreateSkyboxEntity(cubemapFileNames);
}
```

The Skybox shader program.

```cpp
// File: SkyboxShaderProgram.h

class SkyboxShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(const sg::ogl::scene::Scene& t_scene, const entt::entity t_entity, const sg::ogl::resource::Mesh& t_currentMesh) override
    {
        // remove translation from the view matrix
        const auto skyboxViewMatrix{ glm::mat4(glm::mat3(t_scene.GetCurrentCamera().GetViewMatrix())) };

        // get projection matrix
        const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };

        // set shader uniforms
        SetUniform("projectionMatrix", projectionMatrix);
        SetUniform("viewMatrix", skyboxViewMatrix);
        SetUniform("cubeSampler", 0);

        // get cubemap component
        auto& cubemapComponent = t_scene.GetApplicationContext()->registry.get<sg::ogl::ecs::component::CubemapComponent>(t_entity);

        // bind cubemap
        sg::ogl::resource::TextureManager::BindForReading(cubemapComponent.cubemapId, GL_TEXTURE0, GL_TEXTURE_CUBE_MAP);
    }

    std::string GetFolderName() override
    {
        return "skybox";
    }
};
```

The skybox render system. Here, the `DepthFunc` is changed in `PrepareRendering()` and `FinishRendering()`.


```cpp
// File: SkyboxRenderSystem.h

template <typename TShaderProgram>
class SkyboxRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit SkyboxRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(double t_dt) override {}

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::MeshComponent,
            sg::ogl::ecs::component::CubemapComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            auto& meshComponent = view.get<sg::ogl::ecs::component::MeshComponent>(entity);

            meshComponent.mesh->InitDraw();
            shaderProgram.UpdateUniforms(*m_scene, entity, *meshComponent.mesh);
            meshComponent.mesh->DrawPrimitives();
            meshComponent.mesh->EndDraw();
        }

        sg::ogl::resource::ShaderProgram::Unbind();

        FinishRendering();
    }

protected:
    void PrepareRendering() override
    {
        sg::ogl::OpenGl::SetDepthFunc(GL_LEQUAL);
    }

    void FinishRendering() override
    {
        // GL_LESS is the initial depth comparison function
        sg::ogl::OpenGl::SetDepthFunc(GL_LESS);
    }
};
```

### d) Create a Terrain

For the terrain, a config file must be created.

```xml
<?xml version="1.0" encoding="utf-8"?>

<terrain>
    <position x="0.0" z="0.0"/>
    <scale xz="6000.0" y="600.0"/>
    <heightmapPath>res/heightmap/map.bmp</heightmapPath>
    <textures>
        <texture name="grass" path="res/texture/terrain/Grass.jpg"/>
        <texture name="sand" path="res/texture/terrain/GrassAndRock.jpg"/>
        <texture name="rock" path="res/texture/terrain/Cliff.jpg"/>
    </textures>
    <normalmap shaderName="normalmap" textureName="normalmapTexture"/>
    <splatmap shaderName="splatmap" textureName="splatmapTexture"/>
    <normalStrength>60.0</normalStrength>
</terrain>

```

In addition to the render system an instance of `Terrain()` is needed.

```cpp
// File: GameState.h

#pragma once

#include "SgOgl.h"
#include "shader/TerrainShaderProgram.h"
#include "renderer/TerrainRenderSystem.h"

class GameState : public sg::ogl::state::State
{
public:
    /// ...

    // terrain
    using TerrainSharedPtr = std::shared_ptr<sg::ogl::terrain::Terrain>;

    // ...

    void Render() override;

private:
    // ...

    TerrainSharedPtr m_terrain;

    std::unique_ptr<TerrainRenderSystem<TerrainShaderProgram>> m_terrainRenderSystem;

    // ...
};
```

As always.

```cpp
// File: GameState.cpp

void GameState::Render()
{
    // ...

    m_terrainRenderSystem->Render();
}
```

This is where the instance of `Terrain()` is created. The above created config is passed.

```cpp
// File: GameState.cpp

void GameState::Init()
{
    // ...

    // create terrain
    m_terrain = std::make_shared<sg::ogl::terrain::Terrain>(GetApplicationContext(), "res/config/Terrain.xml");
    m_terrain->GenerateTerrain<ComputeNormalmap, ComputeSplatmap>();

    m_terrainRenderSystem = std::make_unique<TerrainRenderSystem<TerrainShaderProgram>>(m_scene.get());

    // create terrain entity
    GetApplicationContext()->GetEntityFactory().CreateTerrainEntity(m_terrain);
}
```

The shader program and the renderer.


```cpp
// File: TerrainShaderProgram.h

class TerrainShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(const sg::ogl::scene::Scene& t_scene, const entt::entity t_entity, const sg::ogl::resource::Mesh& t_currentMesh) override
    {
        // get components
        auto& terrainComponent = t_scene.GetApplicationContext()->registry.get<sg::ogl::ecs::component::TerrainComponent>(t_entity);
        auto& transformComponent = t_scene.GetApplicationContext()->registry.get<sg::ogl::ecs::component::TransformComponent>(t_entity);

        // get terrain options
        const auto& terrainOptions{ terrainComponent.terrain->GetTerrainOptions() };

        // calc mvp matrix
        const auto projectionMatrix{ t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix() };
        const auto mvp{ projectionMatrix * t_scene.GetCurrentCamera().GetViewMatrix() * static_cast<glm::mat4>(transformComponent) };

        // set mvp uniform
        SetUniform("mvpMatrix", mvp);

        // set and bind textures
        auto counter{ 0 };
        for (const auto& entry : terrainOptions.textureContainer)
        {
            SetUniform(entry.first, counter);
            sg::ogl::resource::TextureManager::BindForReading(t_scene.GetApplicationContext()->GetTextureManager().GetTextureIdFromPath(entry.second), GL_TEXTURE0 + counter);
            counter++;
        }

        // set and bind normalmap
        SetUniform("normalmap", counter);
        sg::ogl::resource::TextureManager::BindForReading(terrainComponent.terrain->GetNormalmapTextureId(), GL_TEXTURE0 + counter);
        counter++;

        // set and bind splatmap
        SetUniform("splatmap", counter);
        sg::ogl::resource::TextureManager::BindForReading(terrainComponent.terrain->GetSplatmapTextureId(), GL_TEXTURE0 + counter);
    }

    std::string GetFolderName() override
    {
        return "terrain";
    }
};
```

```cpp
// File: TerrainRenderSystem.h

template <typename TShaderProgram>
class TerrainRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit TerrainRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(double t_dt) override {}

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::TerrainComponent,
            sg::ogl::ecs::component::TransformComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            auto& terrainComponent = view.get<sg::ogl::ecs::component::TerrainComponent>(entity);

            terrainComponent.terrain->GetMesh().InitDraw();
            shaderProgram.UpdateUniforms(*m_scene, entity, terrainComponent.terrain->GetMesh());
            terrainComponent.terrain->GetMesh().DrawPrimitives();
            terrainComponent.terrain->GetMesh().EndDraw();
        }

        sg::ogl::resource::ShaderProgram::Unbind();

        FinishRendering();
    }

protected:
    void PrepareRendering() override
    {
        sg::ogl::OpenGl::EnableFaceCulling();
    }

    void FinishRendering() override
    {
        sg::ogl::OpenGl::DisableFaceCulling();
    }
};
```

### e) Create a Gui

Here, a `Gui` should be created and the content should be a water reflection texture.

```cpp
// File: GameState.h

#include "SgOgl.h"
#include "shader/GuiShaderProgram.h"
#include "renderer/GuiRenderSystem.h"

class GameState : public sg::ogl::state::State
{
public:
    // ...

    void Render() override;

private:
    std::unique_ptr<GuiRenderSystem<GuiShaderProgram>> m_guiRenderSystem;

    // ...

    void Init();
};
```

Create an Gui entity.

```cpp
// File: GameState.cpp

void GameState::Render()
{
    // ...

    // Always render last.
    m_guiRenderSystem->Render();
}

void GameState::Init()
{
    // ...

    m_guiRenderSystem = std::make_unique<GuiRenderSystem<GuiShaderProgram>>(m_scene.get());

    // create gui entity
    const auto posX{ 0.5f };
    const auto posY{ 0.5f };
    const auto scaleX{ 0.25f };
    const auto scaleY{ 0.25f };
    GetApplicationContext()->GetEntityFactory().CreateGuiEntity(
        posX, posY,
        scaleX, scaleY,
        m_water->GetWaterFbos().GetReflectionColorTextureId() // your texture Id
    );
}
```

The shader program and the renderer.

```cpp
// File: GuiShaderProgram.h

class GuiShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(const sg::ogl::scene::Scene& t_scene, const entt::entity t_entity, const sg::ogl::resource::Mesh& t_currentMesh) override
    {
        auto& transformComponent = t_scene.GetApplicationContext()->registry.get<sg::ogl::ecs::component::TransformComponent>(t_entity);
        SetUniform("transformationMatrix", static_cast<glm::mat4>(transformComponent));

        auto& guiComponent = t_scene.GetApplicationContext()->registry.get<sg::ogl::ecs::component::GuiComponent>(t_entity);
        SetUniform("guiTexture", 0);
        sg::ogl::resource::TextureManager::BindForReading(guiComponent.textureId, GL_TEXTURE0);
    }

    std::string GetFolderName() override
    {
        return "gui";
    }
};
```

```cpp
// File: GuiRenderSystem.h

template <typename TShaderProgram>
class GuiRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit GuiRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(double t_dt) override {}

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::TransformComponent,
            sg::ogl::ecs::component::MeshComponent,
            sg::ogl::ecs::component::GuiComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            auto& meshComponent = view.get<sg::ogl::ecs::component::MeshComponent>(entity);

            meshComponent.mesh->InitDraw();
            shaderProgram.UpdateUniforms(*m_scene, entity, *meshComponent.mesh);
            meshComponent.mesh->DrawPrimitives(GL_TRIANGLE_STRIP);
            meshComponent.mesh->EndDraw();
        }

        sg::ogl::resource::ShaderProgram::Unbind();

        FinishRendering();
    }

protected:
    void PrepareRendering() override
    {
        sg::ogl::OpenGl::EnableAlphaBlending();
        sg::ogl::OpenGl::DisableDepthTesting();
    }

    void FinishRendering() override
    {
        sg::ogl::OpenGl::EnableDepthTesting();
        sg::ogl::OpenGl::DisableBlending();
    }
};
```

## 4. Advanced

### a) Instancing

The next step is to add grass to the scene. The following image shows 50,000 grass models that were rendered with a single draw call without lighting.

![Result](https://github.com/stwe/SgOgl/blob/master/Sandbox/res/devlog/GrassInstancing.png)

The `CreatePlantPositions()` method generates random positions for all grass instances.

```cpp
// File: GameState.h

#pragma once

#include "SgOgl.h"
#include "shader/InstancingShaderProgram.h"
#include "renderer/InstancingRenderSystem.h"

class GameState : public sg::ogl::state::State
{
public:
    // ...

    void Render() override;

protected:

private:
    // ...

    std::unique_ptr<InstancingRenderSystem<InstancingShaderProgram>> m_instancingRenderSystem;


    void Init();

    // ...

    std::vector<glm::mat4> CreatePlantPositions(uint32_t t_instances) const;
};
```

The `CreatePlantPositions()` method.

```cpp
// File: GameState.cpp

std::vector<glm::mat4> GameState::CreatePlantPositions(const uint32_t t_instances) const
{
    std::random_device seeder;
    std::mt19937 engine(seeder());

    const std::uniform_real_distribution<float> posX(-1100.0f, -800.0f);
    const std::uniform_real_distribution<float> posZ(-2050.0f, -1900.0);

    std::vector<glm::mat4> matrices;

    for (auto i{ 0u }; i < t_instances; ++i)
    {
        sg::ogl::math::Transform transform;

        const auto pos{ glm::vec3(posX(engine), 0.0f, posZ(engine)) };
        const auto height{ m_terrain->GetHeightAtWorldPosition(pos.x, pos.z) };

        transform.position = glm::vec3(pos.x, height, pos.z);
        transform.scale = glm::vec3(2.0f);

        matrices.push_back(transform.GetModelMatrix());
    }

    return matrices;
}
```

The `Render()` and `Init()` methods.


```cpp
// File: GameState.cpp

void GameState::Render()
{
    // ...

    m_instancingRenderSystem->Render();
}

void GameState::Init()
{
    // ...

    m_instancingRenderSystem = std::make_unique<InstancingRenderSystem<InstancingShaderProgram>>(m_scene.get());

    // ...

    // create plant entity (instancing)
    const uint32_t instances{ 10000 };
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        instances,
        "res/model/Grass/grassmodel.obj",
        CreatePlantPositions(instances)
    );
}
```

The shader program and the renderer.

```cpp
// File: InstancingShaderProgram.h

class InstancingShaderProgram : public sg::ogl::resource::ShaderProgram
{
public:
    void UpdateUniforms(const sg::ogl::scene::Scene& t_scene, const entt::entity t_entity, const sg::ogl::resource::Mesh& t_currentMesh) override
    {
        SetUniform("projectionMatrix", t_scene.GetApplicationContext()->GetWindow().GetProjectionMatrix());
        SetUniform("viewMatrix", t_scene.GetCurrentCamera().GetViewMatrix());
        SetUniform("ambientIntensity", glm::vec3(1.0f));
        SetUniform("diffuseMap", 0);
        sg::ogl::resource::TextureManager::BindForReading(t_currentMesh.GetDefaultMaterial()->mapKd, GL_TEXTURE0);
    }

    std::string GetFolderName() override
    {
        return "instancing";
    }
};
```

```cpp
// File: InstancingRenderSystem.h

template <typename TShaderProgram>
class InstancingRenderSystem : public sg::ogl::ecs::system::RenderSystem<TShaderProgram>
{
public:
    explicit InstancingRenderSystem(sg::ogl::scene::Scene* t_scene)
        : sg::ogl::ecs::system::RenderSystem<TShaderProgram>(t_scene)
    {}

    void Update(double t_dt) override {}

    void Render() override
    {
        PrepareRendering();

        auto view = m_scene->GetApplicationContext()->registry.view<
            sg::ogl::ecs::component::InstancesComponent,
            sg::ogl::ecs::component::ModelComponent>();

        auto& shaderProgram{ m_scene->GetApplicationContext()->GetShaderManager().GetShaderProgram(m_shaderFolderName) };

        shaderProgram.Bind();

        for (auto entity : view)
        {
            auto& instancesComponent = view.get<sg::ogl::ecs::component::InstancesComponent>(entity);
            auto& modelComponent = view.get<sg::ogl::ecs::component::ModelComponent>(entity);

            for (auto& mesh : modelComponent.model->GetMeshes())
            {
                mesh->InitDraw();
                shaderProgram.UpdateUniforms(*m_scene, entity, *mesh);
                mesh->DrawInstanced(instancesComponent.instances);
                mesh->EndDraw();
            }
        }

        sg::ogl::resource::ShaderProgram::Unbind();

        FinishRendering();
    }

protected:
    void PrepareRendering() override
    {
        sg::ogl::OpenGl::EnableAlphaBlending();
    }

    void FinishRendering() override
    {
        sg::ogl::OpenGl::DisableBlending();
    }
};
```


### b) Water rendering


![Result](https://github.com/stwe/SgOgl/blob/master/Sandbox/res/devlog/TestWater.png)


### c) Particle systems


### d) Lighting



