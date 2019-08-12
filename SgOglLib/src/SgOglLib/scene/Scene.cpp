#include "Scene.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Scene::Scene()
{
	m_rootNode = new Node;
	m_rootNode->SetParent(this);
}

sg::ogl::scene::Scene::~Scene() noexcept
{
}

//-------------------------------------------------
// Scene objects
//-------------------------------------------------

void sg::ogl::scene::Scene::AddObject(Node* t_node) const
{
	m_rootNode->AddChild(t_node);
}
