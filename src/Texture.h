#pragma once

#include <string>

#include "Util.h"
#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image/stb_image.h"

// ============================================================================
// Class definition
// ============================================================================

class Texture
{
private:
    uint m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;

public:
    Texture(const std::string& path);
    ~Texture() { GLCall(glDeleteTextures(1, &m_RendererID)); };

    void Bind(uint slot = 0) const;
    void Unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); };

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};

// ============================================================================
// Implementation
// ============================================================================

Texture::Texture(const std::string &path) : m_RendererID(0), m_FilePath(path), 
    m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
    // Flip image vertically (OpenGL y-axis goes from bottom to top)
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    // Set some default parameters, otherwise OpenGL won't render the image
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // GL_RGBA8 is internal format and GL_RGBA is external format
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);
}

void Texture::Bind(uint slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}