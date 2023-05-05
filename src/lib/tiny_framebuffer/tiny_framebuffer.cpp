#include <tiny_framebuffer.hpp>

static const luaL_Reg tiny_texture_func[] = {
    {"new_2d", new_framebuffer_2d_lua},
    {nullptr, nullptr}
};

void luaopen_tiny_framebufferlib(lua_State *L){
    //把所有函数注册到了表中
    luaL_newlib(L, tiny_texture_func);
    //为表命名并弹出表
    lua_setglobal(L, "framebuffer");
    log("register tiny_texture to lua\n");
};


