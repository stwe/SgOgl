// This file is part of the SgOgl package.
// 
// Filename: EmptyState.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOgl.h"

class EmptyState : public sg::ogl::state::State
{
public:
    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    EmptyState() = delete;

    explicit EmptyState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "EmptyState" }
    {
        Init();
    }

    EmptyState(const EmptyState& t_other) = delete;
    EmptyState(EmptyState&& t_other) noexcept = delete;
    EmptyState& operator=(const EmptyState& t_other) = delete;
    EmptyState& operator=(EmptyState&& t_other) noexcept = delete;

    ~EmptyState() noexcept override
    {
        SG_OGL_LOG_DEBUG("[EmptyState::~EmptyState()] Destruct EmptyState.");
    }

    //-------------------------------------------------
    // Logic
    //-------------------------------------------------

    bool Input() override;
    bool Update(double t_dt) override;
    void Render() override;

protected:

private:
    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();
};
