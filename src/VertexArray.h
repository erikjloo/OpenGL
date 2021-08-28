#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

// ============================================================================
// Class definition
// ============================================================================

class VertexArray
{
private:
    uint m_RendererID;

public:
    VertexArray() { GLCall(glGenVertexArrays(1, &m_RendererID)); }
    ~VertexArray() { GLCall(glDeleteVertexArrays(1, &m_RendererID)); }

    void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
    void Bind() const { GLCall(glBindVertexArray(m_RendererID)); }
    void Unbind() const { GLCall(glBindVertexArray(0)); }
};

// ============================================================================
// Implementation
// ============================================================================

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    Bind();
    vb.Bind();
    const std::vector<VertexBufferElement> &elements = layout.GetElements();
    intptr_t offset = 0;
    for (uint i = 0; i < elements.size(); ++i)
    {
        const VertexBufferElement &element = elements[i];
        GLCall(glEnableVertexAttribArray(i)); // 0 is the index
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}