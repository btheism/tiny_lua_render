#include <tiny_framebuffer.hpp>

framebuffer::framebuffer(void){
    GL_CHECK(glCreateFramebuffers(1, &ID));
}

//2dtex:带有2d纹理附件;dsobj:带有深度和模版缓冲对象附件
framebuffer new_frambuffer_2d(GLsizei width, GLsizei height){
    framebuffer res{};
    GLuint texture;
    GL_CHECK(glCreateTextures(GL_TEXTURE_2D ,1, &texture));
    GL_CHECK(glTextureStorage2D(texture, 0, GL_RGB8, width, height));
    GL_CHECK(glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CHECK(glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK(glNamedFramebufferTexture(res.ID, GL_COLOR_ATTACHMENT0, texture, 0));

    //由于无需把深度和模版缓冲渲染出来,因此使用模版
    GLuint rbo;
    GL_CHECK(glCreateRenderbuffers(1, &rbo));
    GL_CHECK(glNamedRenderbufferStorage(rbo, GL_DEPTH24_STENCIL8, width, height));
    GL_CHECK(glNamedFramebufferRenderbuffer(res.ID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));

    //减小引用计数
    GL_CHECK(glDeleteTextures(1, &texture));
    GL_CHECK(glDeleteRenderbuffers(1, &rbo));

    return res;
}

//该函数应配合new_texture_2d使用,设置栈顶上的元素的元表为shader
void create_framebuffer_table(lua_State* L){
    //该函数可以避免元表被重复注册,并把元表放在栈顶
    if(luaL_newmetatable(L, "framebuffer")){
        static const luaL_Reg functions[] =
        {
            {"__gc", delete_framebuffer_lua},
            {"active", active_framebuffer_lua},
            {"reset", reset_framebuffer_lua},
            {nullptr, nullptr}
        };
        //这个函数把上面的函数填入表
        luaL_setfuncs(L, functions, 0);
        //复制元表自身
        lua_pushvalue(L, -1);
        //设置元表的"__index"指向自身
        lua_setfield(L, -2, "__index");//该函数把栈的[-1]元素的"__index"设为栈顶元素,这个函数会弹出栈顶元素,因此在设置设置元表的"__index"指向自身前需要复制元表,见https://stackoverflow.com/questions/35589708/how-to-register-c-class-constructor-to-lua-userdata-and-use-it-by-default
    }
    //把栈的-2处的元素的元表设为栈顶元素(即shader的元表),这也会自动弹出元表,现在栈顶元素是设置了元表的userdata
    lua_setmetatable(L, -2);
    return;
}

int new_framebuffer_2d_lua(lua_State* L)
{
    const char* framebuffer_mode = luaL_checkstring(L, 1);
    int fb_width = luaL_checkinteger(L, 2);
    int fb_height = luaL_checkinteger(L, 3);
    framebuffer * res = new framebuffer(new_frambuffer_2d(fb_width, fb_height));

    //创建userdata并把原表填充到栈顶
    void* framebuffer_pp = lua_newuserdata(L, sizeof(void*));
    *(framebuffer**)framebuffer_pp = res;
    create_framebuffer_table(L);
    return 1;
}

int delete_framebuffer_lua(lua_State* L){
    delete *(framebuffer**)(luaL_checkudata(L, 1, "framebuffer"));
    return 0;
}

int active_framebuffer_lua(lua_State* L){
    framebuffer* current_framebuffer =  *(framebuffer**)(luaL_checkudata(L, 1, "framebuffer"));
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, current_framebuffer->ID));
    return 0;
}

int reset_framebuffer_lua(lua_State* L){
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    return 0;
}
