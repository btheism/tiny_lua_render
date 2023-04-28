#include<tiny_matrix.hpp>

const luaL_Reg tiny_matrix_func[] = {
    {"new_mat", new_matrix},
    {nullptr, nullptr}
};

void luaopen_tiny_matrixlib(lua_State *L){
    //把所有函数注册到了表中
    luaL_newlib(L, tiny_matrix_func);
    //为表命名并弹出表
    lua_setglobal(L, "mat");
    log("register tiny_matrix to lua\n");
};

