//该文件原用于开发把glm包装到lua的库,现已放弃,决定自己写库
#ifndef __TINY_GLM
#define __TINY_GLM
#include <tiny_common.hpp>

int new_mat4(lua_State* L);
int mat4_to_lua_table(lua_State* L);
int delete_mat4(lua_State* L);

void luaopen_tiny_glmlib(lua_State *L);
#endif
