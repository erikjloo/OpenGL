#pragma once

#include <vector>
#include "Util.h"

// ============================================================================
// Class definitions
// ============================================================================

struct VertexBufferElement
{
    uint type;
    uint count;
    unsigned char normalized;
    static uint GetSizeOfType(uint type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    uint m_Stride;

public:
    VertexBufferLayout() : m_Stride(0){};
    ~VertexBufferLayout() {};

    template <typename T>
    void Push(uint count); //{ static_assert(false); }
    inline const std::vector<VertexBufferElement> &GetElements() const { return m_Elements; };
    inline uint GetStride() const { return m_Stride; }
};