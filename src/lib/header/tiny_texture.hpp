#ifndef __TINY_TEXTURE
#define __TINY_TEXTURE

#include <tiny_common.hpp>
#include <tiny_image.hpp>

class texture_2d{
public:
    GLuint ID;
    texture_2d(const char* image_path, GLint image_mode, GLint texture_mode, GLint swrap, int twrap, GLint min_filter, GLint max_filter);
    ~texture_2d(void){
        GL_CHECK(glDeleteTextures(1, &ID));
    }
    void active(int slot){
        GL_CHECK(glActiveTexture(GL_TEXTURE0+slot));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, ID));
    }
};

class texture_cube{
public:
    GLuint ID;
    texture_cube(const char* image_path[], GLint image_mode, GLint texture_mode, GLint min_filter, GLint max_filter);
    ~texture_cube(void){
        GL_CHECK(glDeleteTextures(1, &ID));
    }
    void active(int slot){
        GL_CHECK(glActiveTexture(GL_TEXTURE0+slot));
        GL_CHECK(glBindTexture(GL_TEXTURE_3D, ID));
    }
};

int new_texture_2d_lua(lua_State* L);
int delete_texture_2d_lua(lua_State* L);
int new_texture_cube_lua(lua_State* L);
int delete_texture_cube_lua(lua_State* L);

int active_texture_2d_lua(lua_State* L);
int active_texture_cube_lua(lua_State* L);


void luaopen_tiny_texturelib(lua_State *L);
#endif
