# SgOgl

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/499a4726db5844449126e0101ac1074f)](https://app.codacy.com/app/stwe/SgOgl?utm_source=github.com&utm_medium=referral&utm_content=stwe/SgOgl&utm_campaign=Badge_Grade_Dashboard)


[![](http://img.youtube.com/vi/LKAUq4dDoZE/0.jpg)](http://www.youtube.com/watch?v=LKAUq4dDoZE "SgOgl Sponza Playground with an animated model.")


![Result](https://github.com/stwe/SgOgl/blob/master/Sandbox/res/devlog/SponzaLight.png)


***

1. [What is does](#1-what-is-does)
1. [Installing](#2-installing)
1. [Features](#3-features)
1. [Dependencies](#4-dependencies)
1. [Getting started](#5-getting-started)
1. [Code snippets](#6-code-snippets)

***

## 1. What it does

A GameEngine library for OpenGL developed for educational purposes - a hobby project that I program in my free time.

## 2. Installing

Install Visual Studio 2019 and use Premake5 and the premake5.lua file to create the project files.

## 3. Features

- Deferred rendering / Forward rendering
- Advanced Terrain rendering (Quadtree and Tessellation, 16-bit Heightmaps)
- Water rendering
- Gui rendering
- Skydome / Skybox
- Particle systems
- Instanced rendering
- Mouse picking
- Model file loading
- Animations loading
- Different camera types (Third person, First person)
- Different light types (Ambient, Point, Directional)
- Normal mapping
- Entity component system from [EnTT](https://github.com/skypjack/entt)

## 4. Dependencies

Everything is included except the Assimp Lib.

ImGui is optional.

- [GLFW3](https://www.glfw.org/)
- [GLEW](http://glew.sourceforge.net/)
- [GLM](https://glm.g-truc.net/0.9.9/index.html)
- [GLI](http://gli.g-truc.net/0.8.2/index.html)
- [stb](https://github.com/nothings/stb)
- [Assimp](http://www.assimp.org/)
- [EnTT](https://github.com/skypjack/entt)
- [spdlog](https://github.com/gabime/spdlog)
- [TinyXML2](https://github.com/leethomason/tinyxml2)
- [ImGui](https://github.com/ocornut/imgui)

## 5. Getting started

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

## 6. Code snippets


### Forward rendering with some Point Lights and the Sun as directional light source

<img src="https://github.com/stwe/SgOgl/blob/master/Sandbox/res/devlog/forward.png" alt="Forward" width="128" height="128" />


[Forward rendering](https://github.com/stwe/SgOgl/blob/master/Sandbox/src/example/rendering/ForwardRenderingState.cpp)


### Deferred rendering with some Point Lights and the Sun as directional light source

<img src="https://github.com/stwe/SgOgl/blob/master/Sandbox/res/devlog/deferred.png" alt="Deferred" width="128" height="128" />


[Deferred rendering](https://github.com/stwe/SgOgl/blob/master/Sandbox/src/example/rendering/DeferredRenderingState.cpp)


### Forward Water rendering with the Sun as directional light source

<img src="https://github.com/stwe/SgOgl/blob/master/Sandbox/res/devlog/water.png" alt="Water" width="128" height="128" />


[Water rendering](https://github.com/stwe/SgOgl/blob/master/Sandbox/src/example/water/WaterState.cpp)


### Terrain rendering (Quadtree and Tessellation, 16-bit Heightmaps)

<img src="https://github.com/stwe/SgOgl/blob/master/Sandbox/res/devlog/terrain.png" alt="Water" width="128" height="128" />


[Terrain rendering](https://github.com/stwe/SgOgl/blob/master/Sandbox/src/example/terrain/TerrainState.cpp)


### Other examples

[Sponza playground](https://github.com/stwe/SgOgl/blob/master/Sandbox/src/SponzaPlaygroundState.cpp)


[Animations](https://github.com/stwe/SgOgl/blob/master/Sandbox/src/TestState.cpp)

```cpp
    // create third person camera
    m_thirdPersonCamera = std::make_shared<sg::ogl::camera::ThirdPersonCamera>(
        GetApplicationContext(),
        m_playerPosition
    );

    // create a first person camera
    m_firstPersonCamera = std::make_shared<sg::ogl::camera::FirstPersonCamera>(
        GetApplicationContext(),
        glm::vec3(308.0f, 176.0f, 268.0f),
        -131.0f,
        -6.0f
    );
    m_firstPersonCamera->SetCameraVelocity(24.0f);

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

    // create a model entity
    GetApplicationContext()->GetEntityFactory().CreateModelEntity(
        "res/model/Plane/plane1.obj",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(100.0f, 1.0f, 100.0f),
        false
    );

    // create an animated character in the third person perspective - our player or game hero
    m_player = GetApplicationContext()->GetEntityFactory().CreateTppCharacterEntity(
        m_thirdPersonCamera,
        "res/model/Player/drone.X",
        m_playerPosition,
        glm::vec3(0.0f),
        glm::vec3(1.0f),
        nullptr
    );

    // an other skeletal model
    m_castleGuardIdle = GetApplicationContext()->GetEntityFactory().CreateSkeletalModelEntity(
        "res/model/CastleGuard01/Idle.dae",
        glm::vec3(15.0f, 0.0f, 5.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0625f * 0.5f),
        false,
        false,
        true,
        false
    );
```
