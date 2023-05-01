#include<tiny_linear.hpp>

const luaL_Reg tiny_linear_func[] = {
    //关于矩阵的函数
    {"id_mat", id_mat_lua},
    {"inverse", inverse_mat_lua},
    {"trans", transpose_mat_lua},
    {"get_col", get_mat_col_lua},
    {"get_row", get_mat_row_lua},
    {"col_mat", col_mat_lua},
    {"row_mat", row_mat_lua},
    {"ortho_mat", ortho_mat_lua},
    {"persp_mat", perspective_mat_lua},
    {"move_mat", move_mat_lua},
    {"rotate_mat", rotate_mat_lua},
    {"lookat_mat", lookat_mat_lua},
    {"scale_mat", scale_mat_lua},

    //关于向量的函数
    {"new_vec", new_vec},
    {"get_size", get_vec_size_lua},
    {"norm_vec", normalize_vec_lua},

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

