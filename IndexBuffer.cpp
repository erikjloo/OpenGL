#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count)
{
    GLCall(glGenBuffers(1, &m_RendererID));              // Generate Index buffer ID
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // Assign buffer type Array
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() 
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // Assign buffer type Array
}

void IndexBuffer::Ubind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // Assign buffer type Array
}
