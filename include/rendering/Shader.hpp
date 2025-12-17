#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>

namespace ShadowEngine {
namespace Rendering {

class Shader {
public:
    Shader();
    ~Shader();

    // Load and compile shaders
    bool LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    
    // Use the shader program
    void Use() const;
    
    // Uniform setters
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, const float* matrix);
    
    // Get the program ID
    GLuint GetProgramID() const { return m_ProgramID; }

private:
    GLuint m_ProgramID;
    std::unordered_map<std::string, GLint> m_UniformLocations;
    
    // Helper functions
    bool CompileShader(GLuint& shaderID, const std::string& shaderCode, GLenum shaderType);
    bool LinkProgram();
    GLint GetUniformLocation(const std::string& name);
};

} // namespace Rendering
} // namespace ShadowEngine 