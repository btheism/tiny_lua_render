#include<tiny_mesh.hpp>

const luaL_Reg tiny_mesh_func[] = {
    {"new_mesh", new_mesh},
    {nullptr, nullptr}
};

void luaopen_tiny_meshlib(lua_State *L){
    //把所有函数注册到了表中
    luaL_newlib(L, tiny_mesh_func);
    //为表命名并弹出表
    lua_setglobal(L, "mesh");
    log("register tiny_mesh to lua\n");
};

