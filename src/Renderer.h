#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// ============================================================================
// Class definition
// ============================================================================

class Renderer
{
public:
    void Clear() const
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT)); // Clear before drawing
    }
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
    {
        va.Bind();
        ib.Bind();
        shader.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
    }
};
