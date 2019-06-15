#pragma once

#include "SgOgl.h"

class PlaygroundState : public sg::ogl::State
{
public:
    //-------------------------------------------------
    // Ctors. / Dtor.
    //-------------------------------------------------

    PlaygroundState();

    PlaygroundState(const PlaygroundState& t_other) = delete;
    PlaygroundState(PlaygroundState&& t_other) noexcept = delete;
    PlaygroundState& operator=(const PlaygroundState& t_other) = delete;
    PlaygroundState& operator=(PlaygroundState&& t_other) noexcept = delete;

    ~PlaygroundState() noexcept override;

    //-------------------------------------------------
    // State logic
    //-------------------------------------------------

    void Input() override;
    void Update(float t_dt) override;
    void Render(float t_dt) override;

protected:

private:

};
