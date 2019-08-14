#include "Scene.h"
#include "Renderer.h"
#include "camera/LookAtCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Scene::Scene(Renderer& t_renderer, camera::LookAtCamera& t_camera)
    : m_renderer{ t_renderer }
    , m_camera{ t_camera }
{
    m_renderer.SetParentScene(this);

    m_rootNode = new Node;
    m_rootNode->SetParent(this);
}

sg::ogl::scene::Scene::~Scene() noexcept
{
    // todo
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::scene::Renderer& sg::ogl::scene::Scene::GetRenderer()
{
    return m_renderer;
}

sg::ogl::camera::LookAtCamera& sg::ogl::scene::Scene::GetCamera()
{
    return m_camera;
}

//-------------------------------------------------
// Scene objects
//-------------------------------------------------

void sg::ogl::scene::Scene::AddObject(Node* t_node) const
{
    m_rootNode->AddChild(t_node);
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::scene::Scene::Update()
{

}

void sg::ogl::scene::Scene::Render()
{
    for (auto* rootChildren : m_rootNode->GetChildren())
    {
        m_renderer.Render(*rootChildren);
    }
}
