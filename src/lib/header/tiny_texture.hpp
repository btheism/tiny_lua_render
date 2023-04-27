#ifndef __TINY_TEXTURE
#define __TINY_TEXTURE

#include <tiny_common.hpp>

class texture_2d{
public:
    GLuint ID;
    texture_2d(const char* image_path, GLint image_mode, GLint texture_mode, GLint swrap, int twrap, GLint min_filter, GLint max_filter);
    ~texture_2d(void){
        glDeleteTextures(1, &ID);
    }
    void active(int slot){
        glActiveTexture(GL_TEXTURE0+slot);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
};

int new_texture_2d(lua_State* L);
int delete_texture_2d(lua_State* L);

int active_texture_2d(lua_State* L);

void luaopen_tiny_texturelib(lua_State *L);
#endif
