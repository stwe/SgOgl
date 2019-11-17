// This file is part of the SgOgl package.
// 
// Filename: ParticleEmitterComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <memory>

namespace sg::ogl::particle
{
    class ParticleEmitter;
}

namespace sg::ogl::ecs::component
{
    struct ParticleEmitterComponent
    {
        std::shared_ptr<particle::ParticleEmitter> particleEmitter;
    };
}
