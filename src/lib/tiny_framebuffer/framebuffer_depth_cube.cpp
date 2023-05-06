#include <tiny_framebuffer.hpp>

framebuffer_depth_cube::framebuffer_depth_cube(GLsizei len):len(len){
    GL_CHECK(glCreateFramebuffers(1, &ID));
    GL_CHECK(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &texture));
    GL_CHECK(glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTextureParameteri(texture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CHECK(glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK(glTextureStorage2D(texture, 1, GL_DEPTH_COMPONENT24, len, len));

    GL_CHECK(glNamedFramebufferTexture(ID, GL_DEPTH_ATTACHMENT, texture, 0));
    glNamedFramebufferDrawBuffer(ID, GL_NONE);
    glNamedFramebufferReadBuffer(ID, GL_NONE);
}


//该函数应配合new_framebuffer_depth_cube使用,设置栈顶上的元素的元表为shader
void create_framebuffer_depth_cube_table(lua_State* L){
    //该函数可以避免元表被重复注册,并把元表放在栈顶
    if(luaL_newmetatable(L, "fb_cube")){
        static const luaL_Reg functions[] =
        {
            {"__gc", delete_framebuffer_depth_cube_lua},
            {"use", use_framebuffer_depth_cube_lua},
            {"active", active_framebuffer_depth_cube_texture_lua},
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

int new_framebuffer_depth_cube_lua(lua_State* L)
{
    GLsizei fb_len = luaL_checkinteger(L, 1);
    framebuffer_depth_cube * res = new framebuffer_depth_cube(fb_len);

    //创建userdata并把原表填充到栈顶
    void* framebuffer_depth_cube_pp = lua_newuserdata(L, sizeof(void*));
    *(framebuffer_depth_cube**)framebuffer_depth_cube_pp = res;
    create_framebuffer_depth_cube_table(L);
    return 1;
}

int delete_framebuffer_depth_cube_lua(lua_State* L){
    delete *(framebuffer_depth_cube**)(luaL_checkudata(L, 1, "fb_cube"));
    return 0;
}

int use_framebuffer_depth_cube_lua(lua_State* L){
    framebuffer_depth_cube* current_framebuffer =  *(framebuffer_depth_cube**)(luaL_checkudata(L, 1, "fb_cube"));
    current_framebuffer->use();
    return 0;
}

int active_framebuffer_depth_cube_texture_lua(lua_State* L){
    framebuffer_depth_cube* current_framebuffer =  *(framebuffer_depth_cube**)(luaL_checkudata(L, 1, "fb_cube"));
    GLuint slot = luaL_checkinteger(L, 2);
    current_framebuffer->active_texture(slot);
    return 0;
}

