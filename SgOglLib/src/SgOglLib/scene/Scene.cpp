#include <memory>
#include "Scene.h"
#include "Node.h"
#include "Entity.h"
#include "OpenGl.h"
#include "RenderComponent.h"
#include "resource/ShaderManager.h"
#include "resource/ShaderProgram.h"
#include "resource/Model.h"
#include "resource/Mesh.h"
#include "resource/Material.h"
#include "camera/LookAtCamera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Scene::Scene()
{
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
// Vertex attribute
//-------------------------------------------------

void sg::ogl::scene::Scene::SetNodeInstancePositions(const std::vector<glm::mat4>& t_modelMatrices, Node* t_node)
{
    SG_OGL_CORE_ASSERT(t_node, "[Scene::SetNodeInstancePositions()] Null pointer.")

    if (t_node->mesh && !t_node->HasChildren())
    {
        StorePositions(t_modelMatrices, t_node);
    }

    if (!t_node->mesh && t_node->HasChildren())
    {
        for (auto* child : t_node->GetChildren())
        {
            StorePositions(t_modelMatrices, child);
        }
    }
}

//-------------------------------------------------
// Scene objects
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

sg::ogl::scene::Entity* sg::ogl::scene::Scene::CreateSkydomeEntity(
    const ModelSharedPtr& t_model,
    const glm::vec3& t_scale,
    resource::ShaderProgram& t_shaderProgram
)
{
    // create entity

    auto* entity{ new Entity };
    SG_OGL_CORE_ASSERT(entity, "[Scene::CreateSkydomeEntity()] Null pointer.")

    SG_OGL_CORE_ASSERT(t_model->GetMeshes().size() == 1, "[Scene::CreateSkydomeEntity()] Invalid number of meshes.")

    entity->mesh = t_model->GetMeshes()[0];
    entity->material = t_model->GetMeshes()[0]->GetDefaultMaterial();
    entity->GetLocalTransform().scale = t_scale;
    entity->SetParentScene(this);

    // add render component

    auto renderComponentUniquePtr{ std::make_unique<RenderComponent>() };
    SG_OGL_CORE_ASSERT(renderComponentUniquePtr, "[Scene::CreateSkydomeEntity()] Null pointer.")

    auto renderConfigUniquePtr{ std::make_unique<DefaultRenderConfig>(t_shaderProgram) };
    SG_OGL_CORE_ASSERT(renderConfigUniquePtr, "[Scene::CreateSkydomeEntity()] Null pointer.")

    renderComponentUniquePtr->SetRenderConfig(std::move(renderConfigUniquePtr));

    entity->AddComponent("Renderer", std::move(renderComponentUniquePtr));

    return entity;
}

sg::ogl::scene::Entity* sg::ogl::scene::Scene::CreateSkyboxEntity(
    const uint32_t t_cubemapId,
    resource::ShaderProgram& t_shaderProgram,
    const float t_size
)
{
    // create entity

    auto* entity{ new Entity };
    SG_OGL_CORE_ASSERT(entity, "[Scene::CreateSkyboxEntity()] Null pointer.")

    auto meshUniquePtr{ std::make_unique<resource::Mesh>() };
    SG_OGL_CORE_ASSERT(meshUniquePtr, "[Scene::CreateSkyboxEntity()] Null pointer.")

    auto materialUniquePtr{ std::make_unique<resource::Material>() };
    SG_OGL_CORE_ASSERT(materialUniquePtr, "[Scene::CreateSkyboxEntity()] Null pointer.")

    auto vertices{ CreateSkyboxVertices(t_size) };

    const buffer::BufferLayout bufferLayout{
        { buffer::VertexAttributeType::POSITION, "vPosition" },
    };

    meshUniquePtr->Allocate(bufferLayout, &vertices, static_cast<int32_t>(vertices.size()));

    // set the cubemap id as mapKd
    materialUniquePtr->mapKd = t_cubemapId;

    entity->mesh = std::move(meshUniquePtr);
    entity->material = std::move(materialUniquePtr);
    entity->SetParentScene(this);

    // add render component

    auto renderComponentUniquePtr{ std::make_unique<RenderComponent>() };
    SG_OGL_CORE_ASSERT(renderComponentUniquePtr, "[Scene::CreateSkyboxEntity()] Null pointer.")

    auto renderConfigUniquePtr{ std::make_unique<SkyboxRenderConfig>(t_shaderProgram) };
    SG_OGL_CORE_ASSERT(renderConfigUniquePtr, "[Scene::CreateSkyboxEntity()] Null pointer.")

    renderComponentUniquePtr->SetRenderConfig(std::move(renderConfigUniquePtr));

    entity->AddComponent("Renderer", std::move(renderComponentUniquePtr));

    return entity;
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

void sg::ogl::scene::Scene::Render() const
{
    // render nodes
    for (auto* rootChildren : m_rootNode->GetChildren())
    {
        //m_renderer->Render(*rootChildren);
    }
}

void sg::ogl::scene::Scene::Render(Node* t_node) const
{
    if (t_node->mesh && !t_node->HasChildren())
    {
        //m_renderer->Render(*t_node, t_node->instanceCount);
    }

    if (!t_node->mesh && t_node->HasChildren())
    {
        for (auto* child : t_node->GetChildren())
        {
            //m_renderer->Render(*child, t_node->instanceCount);
        }
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::scene::Scene::StorePositions(const std::vector<glm::mat4>& t_modelMatrices, Node* t_node)
{
    // get vao of the mesh
    auto& vao{ t_node->mesh->GetVao() };

    // bind vao
    vao->BindVao();

    // generate a new vbo
    const auto vboId{ vao->GenerateVbo() };

    // bind the new vbo
    vao->BindVbo(vboId);

    // store data
    glBufferData(GL_ARRAY_BUFFER, t_modelMatrices.size() * sizeof(glm::mat4), t_modelMatrices.data(), GL_STATIC_DRAW);

    // set layout
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));

    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
    glVertexAttribDivisor(8, 1);

    // unbind vbo
    vao->UnbindVbo();

    // unbind vao
    vao->UnbindVao();
}

std::vector<glm::vec3> sg::ogl::scene::Scene::CreateSkyboxVertices(const float t_size)
{
    return std::vector<glm::vec3>
    {
        glm::vec3(-t_size, t_size, -t_size),
        glm::vec3(-t_size, -t_size, -t_size),
        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(t_size, t_size, -t_size),
        glm::vec3(-t_size, t_size, -t_size),

        glm::vec3(-t_size, -t_size, t_size),
        glm::vec3(-t_size, -t_size, -t_size),
        glm::vec3(-t_size, t_size, -t_size),
        glm::vec3(-t_size, t_size, -t_size),
        glm::vec3(-t_size, t_size, t_size),
        glm::vec3(-t_size, -t_size, t_size),

        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(t_size, -t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, t_size, -t_size),
        glm::vec3(t_size, -t_size, -t_size),

        glm::vec3(-t_size, -t_size, t_size),
        glm::vec3(-t_size, t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, -t_size, t_size),
        glm::vec3(-t_size, -t_size, t_size),

        glm::vec3(-t_size, t_size, -t_size),
        glm::vec3(t_size, t_size, -t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(t_size, t_size, t_size),
        glm::vec3(-t_size, t_size, t_size),
        glm::vec3(-t_size, t_size, -t_size),

        glm::vec3(-t_size, -t_size, -t_size),
        glm::vec3(-t_size, -t_size, t_size),
        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(t_size, -t_size, -t_size),
        glm::vec3(-t_size, -t_size, t_size),
        glm::vec3(t_size, -t_size, t_size)
    };
}
