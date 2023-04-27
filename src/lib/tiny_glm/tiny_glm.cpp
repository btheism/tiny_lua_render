#include <tiny_glm.hpp>

const luaL_Reg tiny_glm_func[] = {
    {"new_mat4", new_mat4},
    {nullptr, nullptr}
};

void luaopen_tiny_glmlib(lua_State *L){
    //把所有函数注册到了表中
    luaL_newlib(L, tiny_glm_func);
    //为表命名并弹出表
    lua_setglobal(L, "glm");
    log("register tiny_glm to lua\n");
};
