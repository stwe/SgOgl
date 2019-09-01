#include "Scene.h"
#include "Node.h"
#include "Renderer.h"
#include "resource/Model.h"
#include "resource/Mesh.h"
#include "camera/LookAtCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Scene::Scene(const RendererSharedPtr& t_renderer)
    : m_renderer{ t_renderer }
{
    // set this scene as parent for the renderer
    m_renderer->SetParentScene(this);

    // create a camera with default values
    m_currentCamera = std::make_shared<camera::LookAtCamera>();
    SG_OGL_CORE_ASSERT(m_currentCamera, "[Scene::Scene()] Null pointer.")

    // create the root node
    m_rootNode = new Node;
    SG_OGL_CORE_ASSERT(m_rootNode, "[Scene::Scene()] Null pointer.")
    m_rootNode->SetDebugName("root");
}

sg::ogl::scene::Scene::~Scene() noexcept
{
    delete m_rootNode;
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

sg::ogl::scene::Renderer& sg::ogl::scene::Scene::GetRenderer() noexcept
{
    return *m_renderer;
}

const sg::ogl::scene::Renderer& sg::ogl::scene::Scene::GetRenderer() const noexcept
{
    return *m_renderer;
}

sg::ogl::camera::LookAtCamera& sg::ogl::scene::Scene::GetCurrentCamera() noexcept
{
    return *m_currentCamera;
}

const sg::ogl::camera::LookAtCamera& sg::ogl::scene::Scene::GetCurrentCamera() const noexcept
{
    return *m_currentCamera;
}

sg::ogl::scene::Node* sg::ogl::scene::Scene::GetRoot() const
{
    return m_rootNode;
}

//-------------------------------------------------
// Camera
//-------------------------------------------------

void sg::ogl::scene::Scene::SetCurrentCamera(const CameraSharedPtr& t_camera)
{
    SG_OGL_CORE_ASSERT(m_currentCamera, "[Scene::SetCurrentCamera()] Null pointer.")
    m_currentCamera.reset();

    m_currentCamera = t_camera;
}

//-------------------------------------------------
// Scene objects (Nodes)
//-------------------------------------------------

sg::ogl::scene::Node* sg::ogl::scene::Scene::CreateNode(const ModelSharedPtr& t_model, const MaterialSharedPtr& t_material)
{
    // create node
    auto* node{ new Node };
    SG_OGL_CORE_ASSERT(node, "[Scene::CreateNode()] Null pointer.")

    // get model meshes container as const reference
    const auto& meshes{ t_model->GetMeshes() };

    // create a child node for each mesh
    if (meshes.size() > 1)
    {
        // get each mesh as const reference
        for (const auto& mesh : meshes)
        {
            // create a child node
            auto* childNode{ new Node };
            SG_OGL_CORE_ASSERT(childNode, "[Scene::CreateNode()] Null pointer.")

            // make a copy of the mesh and material shared_ptr
            childNode->mesh = mesh;
            childNode->material = t_material ? t_material : mesh->GetDefaultMaterial();

            // add node as child
            node->AddChild(childNode);
        }
    }
    else
    {
        node->mesh = meshes[0];
        node->material = t_material ? t_material : meshes[0]->GetDefaultMaterial();
    }

    return node;
}

void sg::ogl::scene::Scene::AddNodeToRoot(Node* t_node) const
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
        m_renderer->Render(*rootChildren);
    }
}
