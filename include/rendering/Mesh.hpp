#pragma once

#include <vector>
#include <memory>
#include <glad/glad.h>

namespace ShadowEngine {
namespace Rendering {

class Shader;

class Mesh {
public:
    Mesh();
    ~Mesh();

    // Initialize mesh with vertex and index data
    bool Initialize(const std::vector<float>& vertices, 
                   const std::vector<unsigned int>& indices);
    
    // Render the mesh using the specified shader
    void Render(const std::shared_ptr<Shader>& shader);

private:
    GLuint m_VAO;  // Vertex Array Object
    GLuint m_VBO;  // Vertex Buffer Object
    GLuint m_EBO;  // Element Buffer Object
    
    size_t m_IndexCount;
    
    // Helper functions
    void SetupMesh();
    void Cleanup();
};

} // namespace Rendering
} // namespace ShadowEngine 