#include "rendering/RenderSystem.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Mesh.hpp"
#include "math/Matrix.hpp"
#include <iostream>

namespace ShadowEngine {
namespace Rendering {

RenderSystem::RenderSystem() : m_Window(nullptr) {}

RenderSystem::~RenderSystem() {
    // Cleanup will be handled by the destructors of the member variables
}

bool RenderSystem::Initialize(GLFWwindow* window) {
    m_Window = window;
    
    if (!InitializeOpenGL()) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        return false;
    }
    
    SetupDebugCallback();
    return true;
}

void RenderSystem::Render() {
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Get window dimensions for aspect ratio
    int width, height;
    glfwGetFramebufferSize(m_Window, &width, &height);
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    
    // Create camera matrices
    auto projection = ShadowEngine::Math::CreatePerspective(45.0f, aspectRatio, 0.1f, 100.0f);
    Math::Matrix4 view = m_HasExternalView
        ? m_ViewMatrix
        : ShadowEngine::Math::CreateLookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    auto model = ShadowEngine::Math::CreateRotation(glfwGetTime() * 50.0f, 0.0f, 1.0f, 0.0f);
    
    // Render all meshes with their respective shaders
    for (size_t i = 0; i < m_Meshes.size(); ++i) {
        if (i < m_Shaders.size()) {
            auto& shader = m_Shaders[i];
            shader->Use();
            
            // Set transformation matrices
            shader->SetUniform("projection", projection.GetData());
            shader->SetUniform("view", view.GetData());
            shader->SetUniform("model", model.GetData());
            
            m_Meshes[i]->Render(shader);
        }
    }
    
    // Note: buffer swapping is handled by the window/engine main loop.
}

std::shared_ptr<Shader> RenderSystem::CreateShader(const std::string& vertexPath, 
                                                 const std::string& fragmentPath) {
    auto shader = std::make_shared<Shader>();
    if (shader->LoadFromFiles(vertexPath, fragmentPath)) {
        m_Shaders.push_back(shader);
        return shader;
    }
    return nullptr;
}

std::shared_ptr<Mesh> RenderSystem::CreateMesh(const std::vector<float>& vertices, 
                                             const std::vector<unsigned int>& indices) {
    auto mesh = std::make_shared<Mesh>();
    if (mesh->Initialize(vertices, indices)) {
        m_Meshes.push_back(mesh);
        return mesh;
    }
    return nullptr;
}

bool RenderSystem::InitializeOpenGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // NOTE: Backface culling is disabled for now so all cube faces are visible
    // while developing. Once all mesh winding orders are verified to be CCW,
    // you can re-enable this for better performance:
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    return true;
}

void RenderSystem::SetViewMatrix(const Math::Matrix4& viewMatrix) {
    m_ViewMatrix = viewMatrix;
    m_HasExternalView = true;
}

void RenderSystem::SetupDebugCallback() {
    // TODO: Implement OpenGL debug callback for better error reporting
}

} // namespace Rendering
} // namespace ShadowEngine 