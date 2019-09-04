#include "Scene.h"
#include "Node.h"
#include "Renderer.h"
#include "SkyboxRenderer.h"
#include "resource/Skybox.h"
#include "resource/Model.h"
#include "resource/Mesh.h"
#include "camera/LookAtCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Scene::Scene(
    const RendererSharedPtr& t_renderer,
    const SkyboxRendererSharedPtr& t_skyboxRenderer
)
    : m_renderer{ t_renderer }
    , m_skyboxRenderer{ t_skyboxRenderer }
{
    // set this scene as parent for the renderer
    SG_OGL_CORE_ASSERT(m_renderer, "[Scene::Scene()] Null pointer.")
    m_renderer->SetParentScene(this);

    // set this scene as parent for the skybox renderer
    SG_OGL_CORE_ASSERT(m_skyboxRenderer, "[Scene::Scene()] Null pointer.")
    m_skyboxRenderer->SetParentScene(this);

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

sg::ogl::scene::SkyboxRenderer& sg::ogl::scene::Scene::GetSkyboxRenderer() noexcept
{
    return *m_skyboxRenderer;
}

const sg::ogl::scene::SkyboxRenderer& sg::ogl::scene::Scene::GetSkyboxRenderer() const noexcept
{
    return *m_skyboxRenderer;
}

sg::ogl::camera::LookAtCamera& sg::ogl::scene::Scene::GetCurrentCamera() noexcept
{
    return *m_currentCamera;
}

const sg::ogl::camera::LookAtCamera& sg::ogl::scene::Scene::GetCurrentCamera() const noexcept
{
    return *m_currentCamera;
}

sg::ogl::light::DirectionalLight& sg::ogl::scene::Scene::GetDirectionalLight() noexcept
{
    return *m_directionalLight;
}

const sg::ogl::light::DirectionalLight& sg::ogl::scene::Scene::GetDirectionalLight() const noexcept
{
    return *m_directionalLight;
}

sg::ogl::light::PointLight& sg::ogl::scene::Scene::GetPointLight() noexcept
{
    return *m_pointLight;
}

const sg::ogl::light::PointLight& sg::ogl::scene::Scene::GetPointLight() const noexcept
{
    return *m_pointLight;
}

bool sg::ogl::scene::Scene::IsDirectionalLight() const
{
    return m_directionalLight != nullptr;
}

bool sg::ogl::scene::Scene::IsPointLight() const
{
    return m_pointLight != nullptr;
}

sg::ogl::scene::Node* sg::ogl::scene::Scene::GetRoot() const
{
    return m_rootNode;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::scene::Scene::SetCurrentCamera(const CameraSharedPtr& t_camera)
{
    // the Ctor. creates a default camera, so m_currentCamera should never be null
    SG_OGL_CORE_ASSERT(m_currentCamera, "[Scene::SetCurrentCamera()] Null pointer.")
    m_currentCamera.reset();

    m_currentCamera = t_camera;
}

void sg::ogl::scene::Scene::SetSkybox(const SkyboxSharedPtr& t_skybox)
{
    if (m_skybox)
    {
        m_skybox.reset();
    }

    m_skybox = t_skybox;
}

void sg::ogl::scene::Scene::SetDirectionalLight(const DirectionalLightSharedPtr& t_directionalLight)
{
    if (m_directionalLight)
    {
        m_directionalLight.reset();
    }

    m_directionalLight = t_directionalLight;
}

void sg::ogl::scene::Scene::SetPointLight(const PointLightSharedPtr& t_pointLight)
{
    if (m_pointLight)
    {
        m_pointLight.reset();
    }

    m_pointLight = t_pointLight;
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
    // render nodes
    for (auto* rootChildren : m_rootNode->GetChildren())
    {
        m_renderer->Render(*rootChildren);
    }

    // if a skybox exists, it will be rendered
    if (m_skybox)
    {
        m_skyboxRenderer->Render(
            m_skybox->GetCubemapId(),
            *m_skybox->GetMesh(),
            "skybox"
        );
    }
}
