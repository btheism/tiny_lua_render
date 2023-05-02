#include <tiny_shader.hpp>

const luaL_Reg tiny_shader_func[] = {
    {"new_shader", new_shader_lua},
    {nullptr, nullptr}
};

void luaopen_tiny_shaderlib(lua_State *L){
    //把所有函数注册到了表中
    luaL_newlib(L, tiny_shader_func);
    //为表命名并弹出表
    lua_setglobal(L, "shader");
    log("register tiny_shader to lua\n");
};
