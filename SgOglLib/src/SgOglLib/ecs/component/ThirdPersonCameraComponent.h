// This file is part of the SgOgl package.
// 
// Filename: ThirdPersonCameraComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>

namespace sg::ogl::camera
{
    class ThirdPersonCamera;
}

namespace sg::ogl::ecs::component
{
    struct ThirdPersonCameraComponent
    {
        std::shared_ptr<camera::ThirdPersonCamera> thirdPersonCamera;
    };
}
