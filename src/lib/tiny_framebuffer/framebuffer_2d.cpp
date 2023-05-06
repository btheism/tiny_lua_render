#include <tiny_framebuffer.hpp>

framebuffer_2d::framebuffer_2d(GLsizei width, GLsizei height):width(width),height(height){
    GL_CHECK(glCreateFramebuffers(1, &ID));
    GL_CHECK(glCreateTextures(GL_TEXTURE_2D ,1, &texture));
    GL_CHECK(glTextureStorage2D(texture, 1, GL_RGBA8, width, height));
    GL_CHECK(glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_CHECK(glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CHECK(glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK(glNamedFramebufferTexture(ID, GL_COLOR_ATTACHMENT0, texture, 0));

    //由于无需把深度和模版缓冲渲染出来,因此使用模版
    GL_CHECK(glCreateRenderbuffers(1, &rbo));
    GL_CHECK(glNamedRenderbufferStorage(rbo, GL_DEPTH24_STENCIL8, width, height));
    GL_CHECK(glNamedFramebufferRenderbuffer(ID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo))
}


//该函数应配合new_framebuffer_2d使用,设置栈顶上的元素的元表为shader
void create_framebuffer_2d_table(lua_State* L){
    //该函数可以避免元表被重复注册,并把元表放在栈顶
    if(luaL_newmetatable(L, "framebuffer_2d")){
        static const luaL_Reg functions[] =
        {
            {"__gc", delete_framebuffer_2d_lua},
            {"use", use_framebuffer_2d_lua},
            {"active", active_framebuffer_2d_texture_lua},
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
    GLsizei fb_width = luaL_checkinteger(L, 1);
    GLsizei fb_height = luaL_checkinteger(L, 2);
    framebuffer_2d * res = new framebuffer_2d(fb_width, fb_height);

    //创建userdata并把原表填充到栈顶
    void* framebuffer_2d_pp = lua_newuserdata(L, sizeof(void*));
    *(framebuffer_2d**)framebuffer_2d_pp = res;
    create_framebuffer_2d_table(L);
    return 1;
}

int delete_framebuffer_2d_lua(lua_State* L){
    delete *(framebuffer_2d**)(luaL_checkudata(L, 1, "framebuffer_2d"));
    return 0;
}

int use_framebuffer_2d_lua(lua_State* L){
    framebuffer_2d* current_framebuffer =  *(framebuffer_2d**)(luaL_checkudata(L, 1, "framebuffer_2d"));
    current_framebuffer->use();
    return 0;
}

int active_framebuffer_2d_texture_lua(lua_State* L){
    framebuffer_2d* current_framebuffer =  *(framebuffer_2d**)(luaL_checkudata(L, 1, "framebuffer_2d"));
    GLuint slot = luaL_checkinteger(L, 2);
    current_framebuffer->active_texture(slot);
    return 0;
}

