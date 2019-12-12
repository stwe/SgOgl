// This file is part of the SgOgl package.
// 
// Filename: SkeletalModelComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>

namespace sg::ogl::resource
{
    class SkeletalModel;
}

namespace sg::ogl::ecs::component
{
    struct SkeletalModelComponent
    {
        std::shared_ptr<resource::SkeletalModel> model;
        bool showTriangles{ false };
        bool fakeNormals{ false };
        bool useExistingNormalmaps{ false };
    };
}
