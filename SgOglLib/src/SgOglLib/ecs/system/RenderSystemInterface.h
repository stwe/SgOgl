// This file is part of the SgOgl package.
// 
// Filename: RenderSystemInterface.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include <string>

namespace sg::ogl::ecs::system
{
    class RenderSystemInterface
    {
    public:
        //-------------------------------------------------
        // Public member
        //-------------------------------------------------

        int priority{ 100 };
        std::string debugName;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        RenderSystemInterface() = default;

        RenderSystemInterface(const RenderSystemInterface& t_other) = delete;
        RenderSystemInterface(RenderSystemInterface&& t_other) noexcept = delete;
        RenderSystemInterface& operator=(const RenderSystemInterface& t_other) = delete;
        RenderSystemInterface& operator=(RenderSystemInterface&& t_other) noexcept = delete;

        virtual ~RenderSystemInterface() = default;

        //-------------------------------------------------
        // Interface
        //-------------------------------------------------

        virtual void Update(double t_dt) = 0;
        virtual void Render() = 0;
        virtual void PrepareRendering() = 0;
        virtual void FinishRendering() = 0;

    protected:

    private:

    };
}
