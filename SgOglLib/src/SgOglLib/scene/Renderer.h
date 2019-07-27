#pragma once

#include <vector>

namespace sg::ogl::scene
{
    class Scene;

    class Renderer
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
