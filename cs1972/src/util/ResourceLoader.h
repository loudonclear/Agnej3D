#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include "GL/glew.h"

/**
 * A helper class for loading and compiling shaders.
 *
 * @brief The ResourceLoader class
 */
class ResourceLoader
{
public:
    ResourceLoader();
    static GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path);
};

#endif // RESOURCELOADER_H
