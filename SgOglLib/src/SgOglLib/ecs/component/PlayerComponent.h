// This file is part of the SgOgl package.
// 
// Filename: PlayerComponent.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

namespace sg::ogl::ecs::component
{
    struct PlayerComponent
    {
        static constexpr auto RUN_SPEED{ 40.0f };
        static constexpr auto TURN_SPEED{ 160.0f };
        static constexpr auto GRAVITY{ -50.0f };
        static constexpr auto JUMP_POWER{ 18.0f };

        float currentSpeed{ 0.0f };
        float currentTurnSpeed{ 0.0f };
        float upSpeed{ 0.0f };

        bool isInAir{ false };
    };
}
