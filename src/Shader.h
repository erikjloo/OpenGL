#pragma once

#include <string>
#include <unordered_map>

#include "Renderer.h"

// ============================================================================
// Class definition
// ============================================================================

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    std::string m_Filepath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string &filepath);
    ~Shader() { GLCall(glDeleteProgram(m_RendererID)); }

    void Bind() const { GLCall(glUseProgram(m_RendererID)); }
    void Unbind() const { GLCall(glUseProgram(0)); }

    // Set uniforms
    void SetUniform1f(const std::string &name, float v0);
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);

private:
    ShaderProgramSource ParseShader(const std::string &filepath);
    unsigned int CompileShader(unsigned int type, const std::string &source);
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
    int GetUniformLocation(const std::string &name);
};