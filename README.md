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

The original goal was only to create a large terrain with a kind of LOD technique. Over time, elements have been added that can be found in GameEngines. But of course there is still a lot missing to create a real game with this library. However, a scene with appealing lighting can be created in just a few steps. Models can be loaded and a beautiful water surface can be generated. To keep things as simple as possible, I'm working on being able to configure all game objects with Lua.

**You just have to:**

* Create an `Application` and a `State` class. 
* Use a Lua file for the description of all the scene objects (Models, Cameras, Lights etc), which is then loaded with our `State` class (**this is currently under heavy development**).

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
    using LuaScriptUniquePtr = std::unique_ptr<sg::ogl::LuaScript>;

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
    LuaScriptUniquePtr m_luaScript;

    void Init();
};

```

The cpp file contains nothing special so far.

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

void GameState::Init()
{
}
```

The newly created game state must be registered. This is done by the `Sandbox` class in the `RegisterStates()` function.
In addition, the game state must be pushed to the state stack. We do this in the `Init()` function of the `Sandbox` class.
The `RegisterStates()` and `Init()` functions are called later before the game loop.

```cpp
// File: Sandbox.cpp

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
// File: Sandbox.cpp

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
-- File: Config.lua

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


Here is an example:


```lua
-- File: newApi.lua

------------------
-- Create Scene --
------------------

scene = Scene.new(applicationContext)


-----------------------------
-- Create and add Renderer --
-----------------------------

ForwardRenderer.new(10, scene)


----------------------------
-- Create and add Cameras --
----------------------------

firstPersonCamera = FirstPersonCamera.new("first_person_camera1", applicationContext, Vec3.new(1334.0, 820.0, 227.0), -178.0, -22.0, scene)
firstPersonCamera:SetCameraVelocity(128.0)
firstPersonCamera:SetMouseSensitivity(0.025)
--print(firstPersonCamera:GetCameraVelocity())
--print(firstPersonCamera:GetMouseSensitivity())

thirdPersonCamera = ThirdPersonCamera.new("third_person_camera1", applicationContext, Vec3.new(1334.0, 820.0, 227.0), scene)
thirdPersonCamera:SetPlayerRotationY(45.0)
--print(thirdPersonCamera:GetPlayerRotationY())


------------------
-- Config Scene --
------------------

scene:SetCurrentCamera("first_person_camera1")
--scene:SetCurrentCamera("third_person_camera1")

scene:SetAmbientIntensity(Vec3.new(1.4, 1.4, 1.4))


---------------------
-- Create Entities --
---------------------

plane = modelManager:GetModel("res/primitive/plane1/plane1.obj")
sphere = modelManager:GetModel("res/primitive/sphere/sphere.obj")
jade = modelManager:GetMaterialByName("jade")

e0 = ecs:CreateEntity()
ecs:AddModelComponent(e0, plane, false)
ecs:AddTransformComponent(e0, Vec3.new(0.0, 150.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(1000.0, 1.0, 1000.0))

e1 = ecs:CreateEntity()
ecs:AddModelComponent(e1, sphere, false)
ecs:AddTransformComponent(e1, Vec3.new(0.0, 240.0, 380.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(4.0, 4.0, 4.0))
ecs:AddMaterialComponent(e1, jade)
```


In our GameState now a `LuaScript` object has to be created. The script creates a Scene object that can be reached via `GetApplicationContext()->currentScene`.


```cpp
// File: GameState.cpp

bool GameState::Input()
{
    if (GetApplicationContext()->currentScene)
    {
        GetApplicationContext()->currentScene->Input();
    }

    return true;
}

bool GameState::Update(const double t_dt)
{
    if (GetApplicationContext()->currentScene)
    {
        GetApplicationContext()->currentScene->Update(t_dt);
    }

    return true;
}

void GameState::Render()
{
    if (GetApplicationContext()->currentScene)
    {
        GetApplicationContext()->currentScene->Render();
    }

    RenderImGui();
}

void GameState::Init()
{
    // ...

    m_luaScript = std::make_unique<sg::ogl::LuaScript>(GetApplicationContext(), "res/scene/newApi.lua");
}
```
