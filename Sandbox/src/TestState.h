// This file is part of the SgOgl package.
// 
// Filename: TestState.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOgl.h"

class TestState : public sg::ogl::state::State
{
public:
    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    TestState() = delete;

    explicit TestState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "TestState" }
    {
        Init();
    }

    TestState(const TestState& t_other) = delete;
    TestState(TestState&& t_other) noexcept = delete;
    TestState& operator=(const TestState& t_other) = delete;
    TestState& operator=(TestState&& t_other) noexcept = delete;

    ~TestState() noexcept override
    {
        SG_OGL_LOG_DEBUG("[TestState::~TestState()] Destruct TestState.");
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
