#include "Scene.h"
#include "Renderer.h"
#include "Log.h"
#include "OpenGl.h"
#include "math/Transform.h"
#include "camera/LookAtCamera.h"
#include "resource/Material.h"
#include "resource/Model.h"
#include "resource/Mesh.h"
#include "resource/ShaderManager.h"
#include "resource/ShaderProgram.h"
#include "resource/TextureManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::scene::Scene::Scene(
    std::string t_name,
    std::string t_shaderProgramName,
    resource::ShaderManager& t_shaderManager,
    camera::LookAtCamera& t_defaultCamera,
    glm::mat4& t_projectionMatrix,
    Renderer* t_renderer
)
    : m_name{ std::move(t_name) }
    , m_shaderProgramName{ std::move(t_shaderProgramName) }
    , m_shaderManager{ t_shaderManager }
    , m_defaultCamera{ t_defaultCamera }
    , m_projectionMatrix{ t_projectionMatrix }
{
    SG_OGL_CORE_ASSERT(m_name.length() > 1, "[Scene::Scene()] Invalid value.")
    SG_OGL_CORE_ASSERT(m_shaderProgramName.length() > 1, "[Scene::Scene()] Invalid value.")

    SG_OGL_CORE_LOG_DEBUG("[Scene::Scene()] Create Scene {}.", m_name);

    if (t_renderer)
    {
        SetRenderer(t_renderer);
        SG_OGL_CORE_ASSERT(m_renderer, "[Scene::Scene()] Null pointer.")
    }
}

sg::ogl::scene::Scene::~Scene() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Scene::~Scene()] Destruct Scene {}.", m_name);
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
    SG_OGL_CORE_ASSERT(t_renderer, "[Scene::SetRenderer()] Null pointer.")

    SG_OGL_CORE_LOG_DEBUG("[Scene::SetRenderer()] Add a Renderer to Scene {}.", m_name);

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
    // get ShaderProgram
    auto& shaderProgram{ m_shaderManager.GetShaderProgram(m_shaderProgramName) };

    // bind ShaderProgram
    shaderProgram->Bind();

    // for each Model in the Scene
    for (const auto& model : m_models)
    {
        // todo: entity = model + transform
        // todo: own multiple cameras

        // get model matrix
        math::Transform transform;
        transform.position = glm::vec3(370.0f, 4.0f, 370.0f);
        transform.scale = glm::vec3(0.25f);

        // set transform
        const auto mvp{ m_projectionMatrix * m_defaultCamera.GetViewMatrix() * transform.GetModelMatrix() };
        shaderProgram->SetUniform("transform", mvp);

        // todo: end

        // set ambient intensity
        shaderProgram->SetUniform("ambientIntensity", glm::vec3(1.0f));

        // for each Mesh in the Model
        for (const auto& mesh : model.get().GetMeshes())
        {
            // get Material
            auto& material{ mesh->GetMaterial() };

            // set diffuse map
            shaderProgram->SetUniform("diffuseMap", 0);
            resource::TextureManager::BindForReading(material->mapKd, GL_TEXTURE0);

            // render Mesh
            mesh->InitDraw();
            mesh->DrawPrimitives();
            mesh->EndDraw();
        }
    }

    // unbind ShaderProgram
    shaderProgram->Unbind();
}

//-------------------------------------------------
// Models
//-------------------------------------------------

sg::ogl::scene::Scene::ModelContainer& sg::ogl::scene::Scene::GetModels() noexcept
{
    return m_models;
}

const sg::ogl::scene::Scene::ModelContainer& sg::ogl::scene::Scene::GetModels() const noexcept
{
    return m_models;
}

void sg::ogl::scene::Scene::AddModel(resource::Model& t_model)
{
    SG_OGL_CORE_LOG_DEBUG("[Scene::AddModel()] Add Model {} to Scene {}.", t_model.GetFullFilePath(), m_name);

    m_models.push_back(t_model);
}

void sg::ogl::scene::Scene::RemoveModel(const resource::Model& t_model)
{
    SG_OGL_CORE_LOG_DEBUG("[Scene::RemoveModel()] Remove Model {} from Scene {}.", t_model.GetFullFilePath(), m_name);

    m_models.erase(std::find_if(
        m_models.begin(),
        m_models.end(),
        [&](const std::reference_wrapper<resource::Model>& t_referenceModel)
        {
            return t_referenceModel.get().GetFullFilePath() == t_model.GetFullFilePath();
        }
    ));
}

void sg::ogl::scene::Scene::ClearModels()
{
    SG_OGL_CORE_LOG_DEBUG("[Scene::ClearModels()] Clear all Models from Scene {}.", m_name);

    m_models.clear();
}

//-------------------------------------------------
// Cameras
//-------------------------------------------------

sg::ogl::scene::Scene::CameraContainer& sg::ogl::scene::Scene::GetCameras() noexcept
{
    return m_cameras;
}

const sg::ogl::scene::Scene::CameraContainer& sg::ogl::scene::Scene::GetCameras() const noexcept
{
    return m_cameras;
}

void sg::ogl::scene::Scene::AddCamera(CameraUniquePtr t_camera)
{
    m_cameras.push_back(std::move(t_camera));
}
