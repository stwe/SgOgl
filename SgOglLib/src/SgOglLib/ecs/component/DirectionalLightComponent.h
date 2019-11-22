// This file is part of the SgOgl package.
// 
// Filename: DirectionalLightComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>

namespace sg::ogl::light
{
    struct DirectionalLight;
}

namespace sg::ogl::ecs::component
{
    struct DirectionalLightComponent
    {
        std::shared_ptr<light::DirectionalLight> directionalLight;
    };
}
