#include <tiny_common.hpp>
void CheckOpenGLError(const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        fatal("OpenGL error %08x, at %s:%i\n", err, fname, line);
        abort();
    }
}
