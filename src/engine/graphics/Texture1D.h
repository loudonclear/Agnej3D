#ifndef TEXTURE1D_H
#define TEXTURE1D_H

#include "Texture.h"

#include "GL/glew.h"

/**
 * @class Texture1D
 *
 * Wrapper class for OpenGL 1D Textures.
 */
class Texture1D : public Texture {
public:
    Texture1D(unsigned char *data, int width, Texture::DATA_TYPE type = Texture::DATA_TYPE::UNSIGNED_BYTE, int numComponents = 4);

    virtual void bind() const override;
    virtual void unbind() const override;

protected:
    void _setFilterMethod(GLenum filter) override;
    void _setWrapMethod(GLenum wrap) override;
    void _setTextureParams(GLenum filter, GLenum wrap) override;
};

#endif // TEXTURE1D_H
