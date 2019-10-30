// This file is part of the SgOgl package.
// 
// Filename: MaterialComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>

namespace sg::ogl::resource
{
    struct Material;
}

namespace sg::ogl::ecs::component
{
    struct MaterialComponent
    {
        std::shared_ptr<resource::Material> material;
    };
}
