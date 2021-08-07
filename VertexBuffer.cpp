#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) 
{
    GLCall(glGenBuffers(1, &m_RendererID));              // Generate vertex buffer ID
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // Assign buffer type Array
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() 
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // Assign buffer type Array
}

void VertexBuffer::Ubind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Assign buffer type Array
}
