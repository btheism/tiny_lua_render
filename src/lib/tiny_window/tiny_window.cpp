#include<tiny_window.hpp>


const luaL_Reg tiny_window_func[] = {
    {"fresh", fresh},
    {"clear", clear},
    {"should_close", should_close},
    {"set_close_bit", set_close_bit},
    {"set_clear_color", set_clear_color},
    {"key_pressed", key_pressed},
    {nullptr, nullptr}
};

void luaopen_tiny_windowlib(lua_State *L){
    //把所有函数注册到了表中
    luaL_newlib(L, tiny_window_func);
    //为表命名并弹出表
    lua_setglobal(L, "window");
    log("register tiny_window to lua\n");
};

