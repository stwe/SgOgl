// This file is part of the SgOgl package.
// 
// Filename: NewState.h
// Author:   stwe
// 
// License:  MIT
// 
// 2020 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

#include "SgOgl.h"
#include "ClientLog.h"

class NewState : public sg::ogl::state::State
{
public:
    using SceneUniquePtr = std::unique_ptr<sg::ogl::scene::Scene>;

    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    NewState() = delete;

    explicit NewState(sg::ogl::state::StateStack* t_stateStack)
        : State{ t_stateStack, "NewState" }
    {
        Init();
    }

    NewState(const NewState& t_other) = delete;
    NewState(NewState&& t_other) noexcept = delete;
    NewState& operator=(const NewState& t_other) = delete;
    NewState& operator=(NewState&& t_other) noexcept = delete;

    ~NewState() noexcept override
    {
        ClientLog::SG_SANDBOX_LOG_DEBUG("[NewState::~NewState()] Destruct NewState.");
        CleanUpImGui();
    }

    //-------------------------------------------------
    // Logic
    //-------------------------------------------------

    bool Input() override;
    bool Update(double t_dt) override;
    void Render() override;

protected:

private:
    SceneUniquePtr m_scene;

    float m_temp{ 0.0f };

    //-------------------------------------------------
    // Helper
    //-------------------------------------------------

    void Init();

    //-------------------------------------------------
    // ImGui
    //-------------------------------------------------

    void InitImGui() const;
    void RenderImGui() const;
    static void CleanUpImGui();
};
