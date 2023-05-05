#ifndef __TINY_FRAMEBUFFER
#define __TINY_FRAMEBUFFER

#include <tiny_common.hpp>

class framebuffer{
public:
    GLuint ID;
    //传入一个函数指针,用来完成剩余的初始化工作
    framebuffer(void);
    ~framebuffer(void){
        GL_CHECK(glDeleteFramebuffers(1, &ID))
    }
    void active(void){
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, ID));
    }
};

int new_framebuffer_2d_lua(lua_State* L);
int active_framebuffer_lua(lua_State* L);
int reset_framebuffer_lua(lua_State* L);
int delete_framebuffer_lua(lua_State* L);

void luaopen_tiny_framebufferlib(lua_State *L);

#endif
