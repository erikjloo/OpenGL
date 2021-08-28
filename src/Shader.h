#pragma once

#include <unordered_map>
#include <string>
#include <sstream> // string stream
#include <fstream> // file reader

#include "Util.h"

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
    uint m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string &filepath);
    ~Shader() { GLCall(glDeleteProgram(m_RendererID)); }

    void Bind() const { GLCall(glUseProgram(m_RendererID)); }
    void Unbind() const { GLCall(glUseProgram(0)); }

    // Set uniforms
    void SetUniform1i(const std::string &name, int v0);
    void SetUniform1f(const std::string &name, float v0);
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);

private:
    ShaderProgramSource ParseShader(const std::string &filepath);
    uint CompileShader(uint type, const std::string &source);
    uint CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
    int GetUniformLocation(const std::string &name);
};