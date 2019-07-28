#pragma once

#include <vector>
#include "Core.h"

namespace sg::ogl::scene
{
    class Scene;

    class SG_OGL_API Renderer
    {
    public:
        friend Scene;

        using ScenesContainer = std::vector<Scene*>;

        void RenderScenes();

    protected:

    private:
        ScenesContainer m_scenes;

        void AddScene(Scene* t_scene);
        void RemoveScene(Scene* t_scene);
    };
}
