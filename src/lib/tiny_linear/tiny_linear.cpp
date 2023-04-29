#include<tiny_linear.hpp>

const luaL_Reg tiny_linear_func[] = {
    //关于矩阵的函数
    {"new_mat", new_matrix},
    {"inverse", inverse_matrix_lua},
    {"trans", transpose_matrix_lua},
    {"mat_col", get_matrix_col},
    {"mat_row", get_matrix_row},
    //关于角度的函数
    {"radians", degree_to_radians},
    {"degree", radians_to_degree},
    {nullptr, nullptr}
};

void luaopen_tiny_linearlib(lua_State *L){
    //把所有函数注册到了表中
    luaL_newlib(L, tiny_linear_func);
    //为表命名并弹出表
    lua_setglobal(L, "linear");
    log("register tiny_linear to lua\n");
};

