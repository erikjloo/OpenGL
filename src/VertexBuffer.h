#pragma once

#include "Renderer.h"

// ============================================================================
// Class definition
// ============================================================================

class VertexBuffer
{
private:
    unsigned int m_RendererID;

public:
    VertexBuffer(const void *data, unsigned int size);
    ~VertexBuffer() { GLCall(glDeleteBuffers(1, &m_RendererID)); }

    void Bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); }
    void Unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
};

// ============================================================================
// Implementation
// ============================================================================

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));              // Generate vertex buffer ID
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // Assign buffer type Array
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
