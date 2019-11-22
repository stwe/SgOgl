// This file is part of the SgOgl package.
// 
// Filename: ModelComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>

namespace sg::ogl::resource
{
    class Model;
}

namespace sg::ogl::ecs::component
{
    struct ModelComponent
    {
        std::shared_ptr<resource::Model> model;
        bool showTriangles{ false };
    };
}
