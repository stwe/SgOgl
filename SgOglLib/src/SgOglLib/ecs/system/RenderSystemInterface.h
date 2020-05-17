// This file is part of the SgOgl package.
// 
// Filename: RenderSystemInterface.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

namespace sg::ogl::ecs::system
{
    class RenderSystemInterface
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        RenderSystemInterface() = default;

        RenderSystemInterface(const RenderSystemInterface& t_other) = delete;
        RenderSystemInterface(RenderSystemInterface&& t_other) noexcept = delete;
        RenderSystemInterface& operator=(const RenderSystemInterface& t_other) = delete;
        RenderSystemInterface& operator=(RenderSystemInterface&& t_other) noexcept = delete;

        virtual ~RenderSystemInterface() {}

        //-------------------------------------------------
        // Interface
        //-------------------------------------------------

        virtual void Update(double t_dt) = 0;
        virtual void Render() = 0;

    protected:

    private:

    };
}
