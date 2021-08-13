#pragma once

#include "Renderer.h"

// ============================================================================
// Class definition
// ============================================================================

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;

public:
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer() { GLCall(glDeleteBuffers(1, &m_RendererID)); }

    void Bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); }
    void Unbind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

    inline unsigned int GetCount() const { return m_Count; }
};

// ============================================================================
// Implementation
// ============================================================================

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) : m_Count(count)
{
    GLCall(glGenBuffers(1, &m_RendererID));                      // Generate Index buffer ID
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // Assign buffer type Array
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}