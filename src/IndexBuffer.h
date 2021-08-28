#pragma once

#include "Util.h"

// ============================================================================
// Class definition
// ============================================================================

class IndexBuffer
{
private:
    uint m_RendererID;
    uint m_Count;

public:
    IndexBuffer(const uint *data, uint count);
    ~IndexBuffer() { GLCall(glDeleteBuffers(1, &m_RendererID)); }

    void Bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); }
    void Unbind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

    inline uint GetCount() const { return m_Count; }
};

// ============================================================================
// Implementation
// ============================================================================

IndexBuffer::IndexBuffer(const uint *data, uint count) : m_Count(count)
{
    GLCall(glGenBuffers(1, &m_RendererID));                      // Generate Index buffer ID
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // Assign buffer type Array
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW));
}