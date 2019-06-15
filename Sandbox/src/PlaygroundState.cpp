#include "PlaygroundState.h"

PlaygroundState::PlaygroundState()
{
    SG_OGL_LOG_DEBUG("[PlaygroundState::PlaygroundState()] Execute the PlaygroundState constructor.");
}

PlaygroundState::~PlaygroundState() noexcept
{
    SG_OGL_LOG_DEBUG("[PlaygroundState::~PlaygroundState()] Execute the PlaygroundState destructor.");
}

void PlaygroundState::Input()
{
}

void PlaygroundState::Update(float t_dt)
{
}

void PlaygroundState::Render(float t_dt)
{
}
