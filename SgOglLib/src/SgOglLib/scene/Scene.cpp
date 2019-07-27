#include "Scene.h"
#include "Renderer.h"
#include "Log.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Scene::Scene(std::string t_name, Renderer* t_renderer)
    : m_name{ std::move(t_name) }
{
    if (t_renderer)
    {
        SetRenderer(t_renderer);
        SG_OGL_CORE_ASSERT(m_renderer, "[Scene::Scene()] Null pointer.")
    }
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::scene::Renderer* sg::ogl::scene::Scene::GetRenderer() const
{
    return m_renderer;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::scene::Scene::SetRenderer(Renderer* t_renderer)
{
    // nullptr makes not really sense
    SG_OGL_CORE_ASSERT(t_renderer, "[Scene::SetRenderer()] Null pointer.")

    if (m_renderer)
    {
        m_renderer->RemoveScene(this);
        m_renderer = nullptr;
    }

    m_renderer = t_renderer;
    m_renderer->AddScene(this);
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::scene::Scene::Render()
{
}
