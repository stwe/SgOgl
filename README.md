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
5. [Code snippets](#6-code-snippets)

***

## 1. What it does

A GameEngine library for OpenGL developed for educational purposes - a hobby project that I program in my free time.

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
    return std::make_unique<Sandbox>("res/config/Config.xml");
}

```

We can see that the constructor of the `Application` class or `Sandbox` class takes the path to a config file as an argument.
The config file can look like this.

The libResFolder option contains the full path to the library's built-in assets.

```xml
<?xml version="1.0" encoding="utf-8"?>

<init>
    <libResFolder>E:\Dev\SgOgl\SgOglLib\res</libResFolder>
    <window>
        <title>Sandbox</title>
        <compatibleProfile>0</compatibleProfile>
        <debugContext>1</debugContext>
        <antialiasing>1</antialiasing>
        <printFrameRate>1</printFrameRate>
        <glMajor>4</glMajor>
        <glMinor>3</glMinor>
        <fps>60.0</fps>
    </window>
    <projection>
        <fovDeg>70.0</fovDeg>
        <width>1024</width>
        <height>768</height>
        <near>0.1</near>
        <far>10000.0</far>
    </projection>
</init>
```

## 5. Code snippets


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

<img src="https://github.com/stwe/SgOgl/blob/master/Sandbox/res/devlog/terrain.png" alt="Terrain" width="128" height="128" />
<img src="https://github.com/stwe/SgOgl/blob/master/Sandbox/res/devlog/terrainWithTrees.png" alt="Terrain_with_trees" width="128" height="128" />

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
