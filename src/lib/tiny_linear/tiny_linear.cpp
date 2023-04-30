#include<tiny_linear.hpp>

const luaL_Reg tiny_linear_func[] = {
    //关于矩阵的函数
    {"new_mat", new_mat},
    {"inverse", inverse_mat_lua},
    {"trans", transpose_mat_lua},
    {"mat_col", get_mat_col},
    {"mat_row", get_mat_row},

    //关于向量的函数
    {"new_vec", new_vec},
    {"vec_size", get_vec_size},

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

