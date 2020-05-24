# SgOgl

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/499a4726db5844449126e0101ac1074f)](https://app.codacy.com/app/stwe/SgOgl?utm_source=github.com&utm_medium=referral&utm_content=stwe/SgOgl&utm_campaign=Badge_Grade_Dashboard)


![Terrain](https://github.com/stwe/SgOgl/blob/master/Sandbox/res/devlog/terrainWithTrees.png)


[![](http://img.youtube.com/vi/LKAUq4dDoZE/0.jpg)](http://www.youtube.com/watch?v=LKAUq4dDoZE "SgOgl Sponza Playground with an animated model.")


![Sponza](https://github.com/stwe/SgOgl/blob/master/Sandbox/res/devlog/SponzaLight.png)


***

1. [What is does](#1-what-is-does)
2. [Installing](#2-installing)
3. [Features](#3-features)
4. [Getting started](#4-getting-started)
5. [Create scene objects with Lua](#5-create-scene-objects-with-lua)

***

## 1. What it does

A GameEngine library for OpenGL developed for educational purposes - a hobby project that I program in my free time.

The original goal was only to create a large terrain with a kind of LOD technique. Over time, elements have been added that can be found in GameEngines. But of course there is still a lot missing to create a real game with this library. However, a scene with appealing lighting can be created in just a few steps. Models can be loaded and a beautiful water surface can be generated. To keep it simple, this can be done using a Lua script.

**You just have to:**

* Create an `Application` and a `State` class. 
* Use a Lua file for the description of all the scene objects (Models, Cameras, Lights etc), which is then loaded with our `State` class.

## 2. Installing

#### Vs2019 && Premake

First: Use the Conan Package Manager to install the thirdparty libraries. The project already includes a `conanfile.txt`.
It is important that `premake` is specified as a generator in this file.

```txt
#conanfile.txt

# ...

[generators]
premake
```

For a release build:

```bash
conan install . -s build_type=Release
```

Or when need to debug:

```bash
conan install . -s build_type=Debug
```

After that use Premake5 and the `premake5.lua` file to create the project files.

```bash
premake5 vs2019
```


#### Gcc && CMake

I work with Win10 and MinGW and have created two new Conan profiles.

```txt
# debug_gcc

[settings]
os=Windows
os_build=Windows
arch=x86_64
arch_build=x86_64
compiler=gcc
compiler.version=9.3
compiler.libcxx=libstdc++11
build_type=Debug
[options]
[build_requires]
[env]

```



```txt
# release_gcc

[settings]
os=Windows
os_build=Windows
arch=x86_64
arch_build=x86_64
compiler=gcc
compiler.version=9.3
compiler.libcxx=libstdc++11
build_type=Release
[options]
[build_requires]
[env]
```


CLion users can now, for example, assign the Conan profiles to the CLion profiles.
The Conan plugin does the rest. Before that the generator has to be changed.


```txt
#conanfile.txt

# ...

[generators]
cmake
```


***It may be necessary to compile the packages. Do this with `--build missing`.***


## 3. Features

- Configuration of the scene and all entities with Lua
- Entity component system from [EnTT](https://github.com/skypjack/entt)
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

## 4. Getting started

The project already contains a sandbox and it is a good idea to start with it.

If you want to create it yourself, do this as follows:

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
        GetStateStack().RegisterState<GameState>(sg::ogl::state::GAME);
    }

    void Init() override
    {
        GetStateStack().PushState(sg::ogl::state::GAME);
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
    return std::make_unique<Sandbox>("res/config/Config.lua");
}

```

We can see that the constructor of the `Application` class or `Sandbox` class takes the path to a config lua-file as an argument.
The config file can look like this.

The `libResFolder` option contains the full path to the library's built-in assets.

```lua
libResFolder = "E:/Dev/SgOgl/SgOglLib/res"

window = {
    title = "Sandbox",
    compatibleProfile = false,
    debugContext = true,
    antialiasing = true,
    printFrameRate = true,
    glMajor = 4,
    glMinor = 3,
    fps = 60.0
}

projection = {
    fovDeg = 70.0,
    width = 1024,
    height = 768,
    near = 0.1,
    far = 10000.0
}
```

## 5. Create scene objects with Lua

All game objects can be configured in a Lua script file.


Here is an full example:


```lua
-----------
-- Scene --
-----------

scene = {
    ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
}

-------------
-- Cameras --
-------------

cameras = {
    firstCamera = {
        type = "first",
        cameraVelocity = 256.0,
        mouseSensitivity = 0.1,
        position = { x = 2633.0, y = 923.0, z = 328.0 },
        yaw = -178.0,
        pitch = -10.0,
        current = true,
    },
    secondCamera = {
        type = "third",
        cameraVelocity = 4.0,
        position = {},
        playerPosition = { x = 0.0, y = 0.0, z = 0.0 },
        playerRotationY = {},
        yaw = 20.0,
        pitch = 0.0,
        current = false,
    },
}

--------------
-- Entities --
--------------

local xWaterPos = 0.0
local zWaterPos = 0.0
local waterHeight = 0.0
local waterTileSize = 5000.0

entities = {
    ------------
    -- models --
    ------------
    playground = {
        TransformComponent = {
            position = { x = 0.0, y = 150.0, z = 0.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 1000.0, y = 1.0, z = 1000.0 },
        },
        ModelComponent = {
            path = "res/primitive/plane1/plane1.obj",
            showTriangles = false,
        },
    },
    rock = {
        TransformComponent = {
            position = { x = 0.0, y = 150.0, z = 380.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 40.0, y = 40.0, z = 40.0 },
        },
        ModelComponent = {
            path = "res/model/Rock/rock.obj",
            showTriangles = false,
        },
    },
    ------------------
    -- point lights --
    ------------------
    pointLight1 = {
        PointLightComponent = {
            position = { x = 30.0, y = 240.0, z = 657.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 10.0, y = 1.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.0014,
            quadratic = 0.000007,
        },
    },
    pointLight2 = {
        PointLightComponent = {
            position = { x = 204.0, y = 240.0, z = -319.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 1.0, y = 10.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.0014,
            quadratic = 0.000007,
        },
    },
    modelWithPointLight = {
        TransformComponent = {
            position = { x = 6.0, y = 150.0, z = 0.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 40.0, y = 40.0, z = 40.0 },
        },
        ModelComponent = {
            path = "res/model/Streetlamp/streetlamp.obj",
            showTriangles = false,
        },
        PointLightComponent = {
            position = { x = -98.0, y = 400.0, z = 11.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 1.0, y = 1.0, z = 10.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.0014,
            quadratic = 0.000007,
        },
    },
    ------------------------
    -- directional lights --
    ------------------------
    --[[
    directionalLight1 = {
        DirectionalLightComponent = {
            direction = { x = 0.5, y = -1.0, z = 0.0 },
            diffuseIntensity = { x = 0.7, y = 0.7, z = 0.7 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
        },
    },
    ]]
    sun = {
        SunComponent = {
            sunTexturePath = "res/sun/sun.png",
            scale = 10.0,
            direction = { x = 1.0, y = -0.2, z = -0.4 },
            diffuseIntensity = { x = 1.0, y = 0.8, z = 0.6 },
            specularIntensity = { x = 1.0, y = 0.8, z = 0.6 },
        },
    },
    ---------------------------
    -- skybox, skydome, guis --
    ---------------------------
    skybox = {
        CubemapComponent = {
            right = "res/skybox/sky1/sRight.png",
            left = "res/skybox/sky1/sLeft.png",
            up = "res/skybox/sky1/sUp.png",
            down = "res/skybox/sky1/sDown.png",
            back = "res/skybox/sky1/sBack.png",
            front = "res/skybox/sky1/sFront.png",
        },
    },
    skydome = {
        TransformComponent = {
            position = { x = 0.0, y = 0.0, z = 0.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 5000.0, y = 5000.0, z = 5000.0 },
        },
        ModelComponent = {
            path = "res/model/Dome/dome.obj",
            showTriangles = false,
        },
        SkydomeComponent = {},
    },
    gui1 = {
        GuiComponent = {
            guiTexturePath = "res/gui/foodIcon.png",
        },
        TransformComponent = {
            position = { x = 0.9, y = 0.9, z = 0.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 0.031, y = 0.031, z = 1.0 },
        },
    },
    gui2 = {
        GuiComponent = {
            guiTexturePath = "res/gui/healthIcon.png",
        },
        TransformComponent = {
            position = { x = 0.9, y = 0.8, z = 0.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 0.031, y = 0.031, z = 1.0 },
        },
    },
    -----------
    -- water --
    -----------
    ocean = {
        WaterComponent = {
            xPosition = xWaterPos,
            zPosition = zWaterPos,
            height = waterHeight,
            tileSize = { x = waterTileSize, y = 1.0, z = waterTileSize },
            dudvTexturePath = "res/water/waterDUDV.png",
            normalMapTexturePath = "res/water/normal.png",
            renderToReflectionTexture = { [0] = "SkydomeRenderSystem", [1] = "SunRenderSystem", [2] = "ForwardRenderSystem" },
            renderToRefractionTexture = { [0] = "SkydomeRenderSystem", [1] = "SunRenderSystem", [2] = "ForwardRenderSystem" },
        },
        TransformComponent = {
            position = { x = xWaterPos, y = waterHeight, z = zWaterPos },
            scale = { x = waterTileSize, y = 1.0, z = waterTileSize },
        },
    },
}

--------------
-- Renderer --
--------------

renderer = {

    --DeferredRenderer = { priority = 99, name = "DeferredRenderSystem" },
    --SkyboxRenderer = { priority = 0, name = "SkyboxRenderSystem" },

    SkydomeRenderer = { priority = 10, name = "SkydomeRenderSystem" },
    WaterRenderer = { priority = 4, name = "WaterRenderSystem" },
    SunRenderer = { priority = 2, name = "SunRenderSystem" },
    ForwardRenderer = { priority = 1, name = "ForwardRenderSystem" },
    GuiRenderer = { priority = 0, name = "GuiRenderSystem" },
}
```


In our GameState now only one scene object has to be created.


```cpp
bool GameState::Input()
{
    m_scene->Input();

    return true;
}

bool GameState::Update(const double t_dt)
{
    m_scene->Update(t_dt);

    return true;
}

void GameState::Render()
{
    m_scene->Render();
}

void GameState::Init()
{
    sg::ogl::OpenGl::SetClearColor(sg::ogl::Color::Black());

    m_scene = std::make_unique<sg::ogl::scene::Scene>(GetApplicationContext(), "res/scene/rendererTest.lua");
}
```
