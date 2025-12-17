#pragma once

#include <memory>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "math/Matrix.hpp"

namespace ShadowEngine {

namespace Rendering {

class Shader;
class Mesh;
class Material;

class RenderSystem {
public:
    RenderSystem();
    ~RenderSystem();

    // Initialize the rendering system
    bool Initialize(GLFWwindow* window);
    
    // Main rendering loop
    void Render();

    // Shader management
    std::shared_ptr<Shader> CreateShader(const std::string& vertexPath, const std::string& fragmentPath);
    
    // Mesh management
    std::shared_ptr<Mesh> CreateMesh(const std::vector<float>& vertices, 
                                    const std::vector<unsigned int>& indices);

    // Camera/view control
    void SetViewMatrix(const Math::Matrix4& viewMatrix);

private:
    GLFWwindow* m_Window;
    std::vector<std::shared_ptr<Shader>> m_Shaders;
    std::vector<std::shared_ptr<Mesh>> m_Meshes;

    // Cached view matrix from the current camera (if provided by a scene)
    Math::Matrix4 m_ViewMatrix;
    bool m_HasExternalView = false;
    
    // Internal initialization
    bool InitializeOpenGL();
    void SetupDebugCallback();
};

} // namespace Rendering
} // namespace ShadowEngine 