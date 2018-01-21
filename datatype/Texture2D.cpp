#include "Texture2D.h"

#include <utility>

Texture2D::Texture2D(unsigned char *data, int width, int height, GLenum type)
{
    GLenum internalFormat = type == GL_FLOAT ? GL_RGBA32F : GL_RGBA;

    // Bind the texture by calling bind() and filling it in
    // Generate the texture with glTexImage2D
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat , width, height, 0, GL_RGBA, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unbind();
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture2D::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
