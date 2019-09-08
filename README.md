# SgOgl

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/499a4726db5844449126e0101ac1074f)](https://app.codacy.com/app/stwe/SgOgl?utm_source=github.com&utm_medium=referral&utm_content=stwe/SgOgl&utm_campaign=Badge_Grade_Dashboard)

***

1. [What is does](#what-is-does)
2. [Installing](#installing)
3. [Using](#using)

***

## What it does

Open Source, multi-platform GameEngine library for OpenGL.

## Installing

### Windows

For Visual Studio there is a premake5.lua file.

### Linux

For Linux some CMakeLists.txt files included to help you. A installed G++ 7 or above is necessary.

Install GLFW3

```bash
sudo apt install libglfw3 libglfw3-dev
```

Install GLEW

```bash
sudo apt install libglew-dev
```

Install Assimp

```bash
sudo apt install libassimp-dev libassimp4
```

Install UUID-DEV

```bash
sudo apt install uuid-dev
```

## Using

### 1. Create application class && entry point

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

    Sandbox(const Sandbox& t_other) = delete;
    Sandbox(Sandbox&& t_other) noexcept = delete;
    Sandbox& operator=(const Sandbox& t_other) = delete;
    Sandbox& operator=(Sandbox&& t_other) noexcept = delete;

    ~Sandbox() noexcept override
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

Next, a game state must be created. The class is usefully named "GameState" inherits from `sg::ogl::state::State`.

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
        Init();
    }

    GameState(const GameState& t_other) = delete;
    GameState(GameState&& t_other) noexcept = delete;
    GameState& operator=(const GameState& t_other) = delete;
    GameState& operator=(GameState&& t_other) noexcept = delete;

    ~GameState() noexcept override
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
In our case, an instance of the `Sandbox` class should be created. The return type is `unique_ptr`.

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
        <faceCulling>0</faceCulling>
        <frustumCulling>0</frustumCulling>
        <printFrameRate>1</printFrameRate>
        <glMajor>4</glMajor>
        <glMinor>3</glMinor>
    </window>
    <projection>
        <fovDeg>70.0</fovDeg>
        <width>1024</width>
        <height>768</height>
        <near>0.1</near>
        <far>5000.0</far>
    </projection>
</init>
```

### 2. Example: Making a simple solar system

[Screenshot](https://github.com/stwe/SgOgl/blob/master/Sandbox/res/screen/sun.png)


#### The complete source code

[Sandbox.cpp](https://github.com/stwe/SgOgl/blob/bc724e30922e6b13814546808cbddacdb48abde5/Sandbox/src/Sandbox.cpp)

[GameState.h](https://github.com/stwe/SgOgl/blob/bc724e30922e6b13814546808cbddacdb48abde5/Sandbox/src/GameState.h)

[GameState.cpp](https://github.com/stwe/SgOgl/blob/bc724e30922e6b13814546808cbddacdb48abde5/Sandbox/src/Sandbox.cpp)

#### The scene graph

We create the sun, the earth and the moon.

```cpp
// GameState.cpp

void GameState::Init()
{
    // ...

    // load model
    m_sphereModel = GetApplicationContext()->GetModelManager()->GetModelFromPath("res/model/sphere/sphere2.obj");

    // create materials
    m_moonMaterial = std::make_shared<sg::ogl::resource::Material>();
    m_earthMaterial = std::make_shared<sg::ogl::resource::Material>();
    m_sunMaterial = std::make_shared<sg::ogl::resource::Material>();

    // load textures
    const auto moonId{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/moon.jpg") };
    const auto earthId{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/earth.jpg") };
    const auto sunId{ GetApplicationContext()->GetTextureManager()->GetTextureIdFromPath("res/texture/sun.jpg") };

    // config materials
    m_moonMaterial->mapKd = moonId;
    m_moonMaterial->ns = 32.0f;

    m_earthMaterial->mapKd = earthId;
    m_earthMaterial->ns = 32.0f;

    m_sunMaterial->mapKd = sunId;
    m_sunMaterial->ns = 32.0f;

    // create renderer
    m_renderer = std::make_shared<sg::ogl::scene::Renderer>(
        *GetApplicationContext()->GetShaderManager(),
        m_projectionMatrix
        );

    // ...

    // create scene, pass the renderer
    m_scene = std::make_unique<sg::ogl::scene::Scene>(m_renderer, m_skyboxRenderer);

    // ...

    // create scene nodes
    m_sunNode = m_scene->CreateNode(m_sphereModel, m_sunMaterial);
    m_sunNode->GetLocalTransform().position = glm::vec3(0.0f, 0.0f, 0.0f); // sun at center
    m_sunNode->GetLocalTransform().scale = glm::vec3(2.0f);
    m_sunNode->SetDebugName("Sun");

    m_earthNode = m_scene->CreateNode(m_sphereModel, m_earthMaterial);
    m_earthNode->GetLocalTransform().position = glm::vec3(10.0f, 0.0f, 0.0f); // earth 10 units from the sun
    m_earthNode->SetDebugName("Earth");

    m_moonNode = m_scene->CreateNode(m_sphereModel, m_moonMaterial);
    m_moonNode->GetLocalTransform().position = glm::vec3(2.0f, 0.0f, 0.0f); // moon 2 units from the earth
    m_moonNode->GetLocalTransform().scale = glm::vec3(0.25f);
    m_moonNode->SetDebugName("Moon");

    // AddChild()
    //m_sunNode->AddChild(m_earthNode);
    //m_earthNode->AddChild(m_moonNode);

    // or SetParent()
    m_moonNode->SetParent(m_earthNode);
    m_earthNode->SetParent(m_sunNode);

    // add (only) the sun node to scene root node
    m_scene->AddNodeToRoot(m_sunNode);

    // ...
}
```

In addition, an asteroid belt is to be created.

```cpp
// GameState.cpp

// ...

void GameState::Init()
{
    // ...

    // load model
    m_asteroidModel = GetApplicationContext()->GetModelManager()->GetModelFromPath("res/model/rock/rock.obj");

    // create scene nodes
    // ...
    m_asteroidNode = m_scene->CreateNode(m_asteroidModel);
    m_asteroidNode->SetDebugName("Asteroid");
    m_asteroidNode->instanceCount = 5000;

    // create a model matrix for each asteroid instance
    GenerateAsteroidPositions(100.0f, 20.0f, m_asteroidNode->instanceCount);

    // create a vbo
    sg::ogl::scene::Scene::SetNodeInstancePositions(m_asteroidModelMatrices, m_asteroidNode);
}
```

#### The GameState logic

##### Input

Here, e.g. the mouse will be updated. See source code.

##### Update

At update time we'll update each object's local matrix.

```cpp
// GameState.cpp


bool GameState::Update(const double t_dt)
{
    // ...

    // update nodes local transform
    m_sunNode->GetLocalTransform().rotation += glm::vec3(0.0f, 0.0315f, 0.0f);
    m_earthNode->GetLocalTransform().rotation += glm::vec3(0.0f, 0.125f, 0.0f);
    m_moonNode->GetLocalTransform().rotation += glm::vec3(0.0f, 0.125f, 0.0f);

    // update scene - calc nodes world matrix
    m_scene->Update();

    // ...

    return true;
}
```

##### Render

Simply render the scene graph.

```cpp
// GameState.cpp

void GameState::Render()
{
    // render sun, earth and the moon
    m_scene->Render();

    // render asteroid field
    m_scene->Render(m_asteroidNode);
}
```
