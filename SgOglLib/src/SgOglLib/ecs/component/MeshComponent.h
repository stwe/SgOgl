// This file is part of the SgOgl package.
// 
// Filename: MeshComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>

namespace sg::ogl::resource
{
    class Mesh;
}

namespace sg::ogl::ecs::component
{
    struct MeshComponent
    {
        std::shared_ptr<resource::Mesh> mesh;
    };
}
