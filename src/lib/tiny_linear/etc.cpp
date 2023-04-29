#include <tiny_linear.hpp>
#define PI 3.1415926535897932384626

int degree_to_radians(lua_State* L){
    double degree = luaL_checknumber(L, 1);
    lua_pushnumber(L, degree*PI/180.0);
    return 1;
}

int radians_to_degree(lua_State* L){
    double degree = luaL_checknumber(L, 1);
    lua_pushnumber(L, degree*180.0/PI);
    return 1;
}
