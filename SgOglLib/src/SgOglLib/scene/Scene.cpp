#include "Scene.h"
#include "resource/Model.h"
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "Node.h"
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
    SG_OGL_CORE_ASSERT(m_rootNode, "[Scene::Scene()] Null pointer.")
    m_rootNode->SetDebugName("root");
}

sg::ogl::scene::Scene::~Scene() noexcept
{
    delete m_rootNode; // todo
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::scene::Renderer& sg::ogl::scene::Scene::GetRenderer() noexcept
{
    return m_renderer;
}

const sg::ogl::scene::Renderer& sg::ogl::scene::Scene::GetRenderer() const noexcept
{
    return m_renderer;
}

sg::ogl::camera::LookAtCamera& sg::ogl::scene::Scene::GetCamera() noexcept
{
    return m_camera;
}

const sg::ogl::camera::LookAtCamera& sg::ogl::scene::Scene::GetCamera() const noexcept
{
    return m_camera;
}

sg::ogl::scene::Node* sg::ogl::scene::Scene::GetRoot() const
{
    return m_rootNode;
}

//-------------------------------------------------
// Scene objects (Nodes)
//-------------------------------------------------

sg::ogl::scene::Node* sg::ogl::scene::Scene::CreateNode(resource::Model* t_model, resource::Material* t_material)
{
    // create node
    auto* node{ new Node };
    SG_OGL_CORE_ASSERT(node, "[Scene::CreateNode()] Null pointer.")

    // add meshes
    if (t_model->GetMeshes().size() > 1)
    {
        // for each mesh
        for (auto& modelMesh : t_model->GetMeshes())
        {
            auto* childNode{ new Node };
            SG_OGL_CORE_ASSERT(childNode, "[Scene::CreateNode()] Null pointer.")

            childNode->mesh = modelMesh.get(); // get raw pointer from unique_ptr
            childNode->material = t_material ? t_material : &modelMesh.get()->GetDefaultMaterial();

            node->AddChild(childNode);
        }
    }
    else
    {
        node->mesh = t_model->GetMeshes()[0].get(); // get raw pointer from unique_ptr
        node->material = t_material ? t_material : &t_model->GetMeshes()[0].get()->GetDefaultMaterial();
    }

    return node;
}

void sg::ogl::scene::Scene::AddNodeToRoot(Node* t_node)
{
    SG_OGL_CORE_ASSERT(t_node, "[Scene::AddNodeToRoot()] Null pointer.")
    m_rootNode->AddChild(t_node);
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::scene::Scene::Update()
{
    m_rootNode->CalcWorldMatrix();
}

void sg::ogl::scene::Scene::Render()
{
    for (auto* rootChildren : m_rootNode->GetChildren())
    {
        m_renderer.Render(*rootChildren);
    }
}
