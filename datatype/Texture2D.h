#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Texture.h"

#include "GL/glew.h"

/**
 * @class Texture2D
 *
 * Wrapper class for OpenGL 2D Textures. This is probably the class you'll want to use
 * for all of your texturing purposes.
 */
class Texture2D : public Texture {
public:
    Texture2D(unsigned char *data, int width, int height, GLenum type = GL_UNSIGNED_BYTE);

    virtual void bind() const override;
    virtual void unbind() const override;
};

#endif // TEXTURE2D_H
