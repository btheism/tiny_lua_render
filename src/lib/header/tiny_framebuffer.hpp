#ifndef __TINY_FRAMEBUFFER
#define __TINY_FRAMEBUFFER

#include <tiny_common.hpp>

//带有2d纹理附件和深度+模版缓冲对象附件
class framebuffer_2d{
public:
    GLuint ID;
    GLuint texture;
    GLuint rbo;
    GLsizei width;
    GLsizei height;
    framebuffer_2d(GLsizei width, GLsizei height);
    framebuffer_2d(const framebuffer_2d &)=delete;
    framebuffer_2d(framebuffer_2d && mirror){
        ID = mirror.ID;
        texture = mirror.texture;
        rbo = mirror.rbo;
        mirror.ID = 0;
        mirror.texture = 0;
        mirror.rbo = 0;
    }

    ~framebuffer_2d(void){
        GL_CHECK(glDeleteTextures(1, &texture));
        GL_CHECK(glDeleteRenderbuffers(1, &rbo));
        GL_CHECK(glDeleteFramebuffers(1, &ID))
    }
    void use(void){
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, ID));
        GL_CHECK(glViewport(0, 0, width, height));

    }
    void active_texture(int slot){
        GL_CHECK(glBindTextureUnit(slot, texture));//glActiveTexture需要做加法,glBindTextureUnit不需要
    }
};

//带有深度立方体贴图附件
class framebuffer_depth_cube{
public:
    GLuint ID;
    GLuint texture;
    GLsizei len;
    framebuffer_depth_cube(GLsizei len);
    framebuffer_depth_cube(const framebuffer_depth_cube &)=delete;
    framebuffer_depth_cube(framebuffer_depth_cube && mirror){
        ID = mirror.ID;
        texture = mirror.texture;
        mirror.ID = 0;
        mirror.texture = 0;
    }

    ~framebuffer_depth_cube(void){
        GL_CHECK(glDeleteTextures(1, &texture));
        GL_CHECK(glDeleteFramebuffers(1, &ID))
    }
    void use(void){
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, ID));
        GL_CHECK(glViewport(0, 0, len, len));

    }
    void active_texture(int slot){
        GL_CHECK(glBindTextureUnit(slot, texture));//glActiveTexture需要做加法,glBindTextureUnit不需要
    }
};

int new_framebuffer_2d_lua(lua_State* L);
int use_framebuffer_2d_lua(lua_State* L);
int active_framebuffer_2d_texture_lua(lua_State* L);
int delete_framebuffer_2d_lua(lua_State* L);

int new_framebuffer_depth_cube_lua(lua_State* L);
int use_framebuffer_depth_cube_lua(lua_State* L);
int active_framebuffer_depth_cube_texture_lua(lua_State* L);
int delete_framebuffer_depth_cube_lua(lua_State* L);

void luaopen_tiny_framebufferlib(lua_State *L);

#endif
