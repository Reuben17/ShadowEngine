#include "rendering/Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace ShadowEngine {
namespace Rendering {

Shader::Shader() : m_ProgramID(0) {}

Shader::~Shader() {
    if (m_ProgramID != 0) {
        glDeleteProgram(m_ProgramID);
    }
}

bool Shader::LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    // Read vertex shader
    std::string vertexCode;
    std::ifstream vertexFile(vertexPath);
    if (vertexFile.is_open()) {
        std::stringstream vertexStream;
        vertexStream << vertexFile.rdbuf();
        vertexCode = vertexStream.str();
        vertexFile.close();
    } else {
        std::cerr << "Failed to open vertex shader file: " << vertexPath << std::endl;
        return false;
    }

    // Read fragment shader
    std::string fragmentCode;
    std::ifstream fragmentFile(fragmentPath);
    if (fragmentFile.is_open()) {
        std::stringstream fragmentStream;
        fragmentStream << fragmentFile.rdbuf();
        fragmentCode = fragmentStream.str();
        fragmentFile.close();
    } else {
        std::cerr << "Failed to open fragment shader file: " << fragmentPath << std::endl;
        return false;
    }

    // Compile shaders
    GLuint vertexShader, fragmentShader;
    if (!CompileShader(vertexShader, vertexCode, GL_VERTEX_SHADER) ||
        !CompileShader(fragmentShader, fragmentCode, GL_FRAGMENT_SHADER)) {
        return false;
    }

    // Create shader program
    m_ProgramID = glCreateProgram();
    glAttachShader(m_ProgramID, vertexShader);
    glAttachShader(m_ProgramID, fragmentShader);
    glLinkProgram(m_ProgramID);

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return LinkProgram();
}

void Shader::Use() const {
    glUseProgram(m_ProgramID);
}

void Shader::SetUniform(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform(const std::string& name, const float* matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix);
}

bool Shader::CompileShader(GLuint& shaderID, const std::string& shaderCode, GLenum shaderType) {
    shaderID = glCreateShader(shaderType);
    const char* code = shaderCode.c_str();
    glShaderSource(shaderID, 1, &code, nullptr);
    glCompileShader(shaderID);

    // Check for compilation errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool Shader::LinkProgram() {
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ProgramID, 512, nullptr, infoLog);
        std::cerr << "Shader program linking error: " << infoLog << std::endl;
        return false;
    }
    return true;
}

GLint Shader::GetUniformLocation(const std::string& name) {
    auto it = m_UniformLocations.find(name);
    if (it != m_UniformLocations.end()) {
        return it->second;
    }
    
    GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
    m_UniformLocations[name] = location;
    return location;
}

} // namespace Rendering
} // namespace ShadowEngine 