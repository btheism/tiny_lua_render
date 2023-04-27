#include <tiny_texture.hpp>

const luaL_Reg tiny_texture_func[] = {
    {"new_texture_2d", new_texture_2d},
    {nullptr, nullptr}
};

void luaopen_tiny_texturelib(lua_State *L){
    //把所有函数注册到了表中
    luaL_newlib(L, tiny_texture_func);
    //为表命名并弹出表
    lua_setglobal(L, "texture");
    log("register tiny_texture to lua\n");
};
