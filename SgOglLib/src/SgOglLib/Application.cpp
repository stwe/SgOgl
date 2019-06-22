#include "Application.h"
#include "Log.h"
#include "Window.h"
#include "Color.h"
#include "State.h"
#include "resource/ShaderManager.h"


// todo start temp code
#include "buffer/Vao.h"
#include "buffer/VertexAttribute.h"
#include "buffer/BufferLayout.h"
#include "resource/ShaderProgram.h"
#include "resource/ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>
// todo end temp code

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::Application::Application(const std::string& t_configFileName)
{
    SG_OGL_CORE_LOG_DEBUG("[Application::Application] Execute the Application constructor.");

    Config::LoadOptions(t_configFileName, m_windowOptions, m_projectionOptions);

    m_window = std::make_unique<Window>(this);
    m_shaderManager = std::make_unique<resource::ShaderManager>();
}

sg::ogl::Application::~Application() noexcept
{
    SG_OGL_CORE_LOG_DEBUG("[Application::~Application] Execute the Application destructor.");

    CleanUp();
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

const sg::ogl::WindowOptions& sg::ogl::Application::GetWindowOptions() const noexcept
{
    return m_windowOptions;
}

sg::ogl::WindowOptions& sg::ogl::Application::GetWindowOptions() noexcept
{
    return m_windowOptions;
}

const sg::ogl::ProjectionOptions& sg::ogl::Application::GetProjectionOptions() const noexcept
{
    return m_projectionOptions;
}

sg::ogl::ProjectionOptions& sg::ogl::Application::GetProjectionOptions() noexcept
{
    return m_projectionOptions;
}

//-------------------------------------------------
// Run
//-------------------------------------------------

void sg::ogl::Application::Run()
{
    SG_OGL_CORE_LOG_DEBUG("[Application::Run()] Application is started.");

    Init();


    // todo start temp code
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 col;
    };

    std::vector<Vertex> vertices = {
        { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.8f, 0.2f, 0.8f) },
        { glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.2f, 0.3f, 0.8f) },
        { glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec3(0.8f, 0.8f, 0.2f) }
    };

    std::vector<uint32_t> indices = { 0, 1, 2 };

    buffer::Vao vao;

    const buffer::BufferLayout bufferLayout = {
        { buffer::VertexAttributeType::POSITION, "vPosition" },
        { buffer::VertexAttributeType::COLOR, "vColor" }
    };

    // copy to GPU
    vao.AllocateVertices(reinterpret_cast<float*>(vertices.data()), 3, 18 * sizeof(float), bufferLayout);
    vao.AllocateIndices(indices);

    // clear on CPU
    vertices.clear();
    indices.clear();

    resource::ShaderProgram shaderProgram;

    #if defined(_WIN64) && defined(_MSC_VER)

        shaderProgram.AddVertexShader(resource::ShaderUtil::ReadShaderFile("res/shader/simple/Vertex.vert"));
        shaderProgram.AddFragmentShader(resource::ShaderUtil::ReadShaderFile("res/shader/simple/Fragment.frag"));

    #elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)

        shaderProgram.AddVertexShader(resource::ShaderUtil::ReadShaderFile("/home/steffen/Dev/SgOgl/Sandbox/res/shader/simple/Vertex.vert"));
        shaderProgram.AddFragmentShader(resource::ShaderUtil::ReadShaderFile("/home/steffen/Dev/SgOgl/Sandbox/res/shader/simple/Fragment.frag"));

    #else

        #error Unsupported platform or unsupported compiler!

    #endif

    shaderProgram.LinkAndValidateProgram();
    //shaderProgram.AddAllFoundUniforms();

    // todo end temp code


    while (m_window->WindowIsNotClosed())
    {
        Window::SetClearColor(Color::CornflowerBlue());
        Window::Clear();

        if (PeekState())
        {
            PeekState()->Input();
            PeekState()->Update(0.0f);
            PeekState()->Render(0.0f);

            // todo start temp code
            vao.BindVao();
            shaderProgram.Bind();
            vao.DrawPrimitives();
            buffer::Vao::UnbindVao();
            // todo end temp code
        }

        m_window->Update();
    }
}

//-------------------------------------------------
// States
//-------------------------------------------------

void sg::ogl::Application::PushState(State* t_state)
{
    t_state->SetApplication(this);
    m_states.push(t_state);
}

void sg::ogl::Application::PopState()
{
    // Top() returns a reference to the top element.
    delete m_states.top();

    // Remove top element.
    m_states.pop();
}

void sg::ogl::Application::ChangeState(State* t_state)
{
    if (!m_states.empty())
    {
        PopState();
    }

    PushState(t_state);
}

sg::ogl::State* sg::ogl::Application::PeekState()
{
    if (m_states.empty())
    {
        return nullptr;
    }

    return m_states.top();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::Application::Init()
{
    m_window->Init();
}

//-------------------------------------------------
// CleanUp
//-------------------------------------------------

void sg::ogl::Application::CleanUp()
{
    SG_OGL_CORE_LOG_DEBUG("[Application::CleanUp()] Cleaning up Application...");

    SG_OGL_CORE_LOG_DEBUG("[Application::CleanUp()] Cleaning up States.");
    while (!m_states.empty())
    {
        PopState();
    }

    m_window->CleanUp();
}
