#include<tiny_linear.hpp>

const luaL_Reg tiny_linear_func[] = {
    {"new_mat", new_matrix},
    {"inverse", inverse_matrix_lua},
    {"mat_col", get_matrix_col},
    {"mat_row", get_matrix_row},
    {nullptr, nullptr}
};

void luaopen_tiny_linearlib(lua_State *L){
    //把所有函数注册到了表中
    luaL_newlib(L, tiny_linear_func);
    //为表命名并弹出表
    lua_setglobal(L, "linear");
    log("register tiny_linear to lua\n");
};

