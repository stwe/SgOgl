#include "Renderer.h"
#include "Log.h"
#include "Scene.h"

void sg::ogl::scene::Renderer::AddScene(Scene* t_scene)
{
    SG_OGL_CORE_ASSERT(t_scene, "[Renderer::AddScene()] Null pointer.")

    for (auto* scene : m_scenes)
    {
        if (scene == t_scene)
        {
            SG_OGL_CORE_ASSERT(false, "[Renderer::AddScene()] Scene already added.")
        }
    }

    m_scenes.push_back(t_scene);
}

void sg::ogl::scene::Renderer::RemoveScene(Scene* t_scene)
{
    SG_OGL_CORE_ASSERT(t_scene, "[Renderer::RemoveScene()] Null pointer.")

    auto i{ 0 };
    for (auto* scene : m_scenes)
    {
        if (scene == t_scene)
        {
            m_scenes.erase(m_scenes.begin() + i);
        }

        i++;
    }
}

void sg::ogl::scene::Renderer::RenderScenes()
{
    for (auto* const scene : m_scenes)
    {
        scene->Render();
    }
}
