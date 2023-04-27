#include <tiny_glm.hpp>

//该函数应配合new_mat4使用,设置栈顶上的元素的元表为shader
void create_mat4_table(lua_State* L){
    //该函数可以避免元表被重复注册,并把元表放在栈顶
    if(luaL_newmetatable(L, "mat4")){
        static const luaL_Reg functions[] =
        {
            {"__gc", delete_mat4},
            {"to_array", mat4_to_lua_table},
            {nullptr, nullptr}
        };
        //这个函数把上面的函数填入表
        luaL_setfuncs(L, functions, 0);
        //复制元表自身
        lua_pushvalue(L, -1);
        //设置元表的"__index"指向自身
        lua_setfield(L, -2, "__index");//该函数把栈的[-1]元素的"__index"设为栈顶元素,这个函数会弹出栈顶元素,因此在设置设置元表的"__index"指向自身前需要复制元表,见https://stackoverflow.com/questions/35589708/how-to-register-c-class-constructor-to-lua-userdata-and-use-it-by-default
    }
    //把栈的-2处的元素的元表设为栈顶元素,这也会自动弹出元表,现在栈顶元素是设置了元表的userdata
    lua_setmetatable(L, -2);
    return;
}

int new_mat4(lua_State* L)
{
    void* mat4_pp = lua_newuserdata(L, sizeof(void*));
    *(glm::mat4**)mat4_pp = new glm::mat4();
    create_mat4_table(L);
    return 1;
}

int mat4_to_lua_table(lua_State* L){
    glm::mat4* mat = *(glm::mat4**)luaL_checkudata(L, 1, "mat4");
    lua_newtable(L);
    for(int i=1; i<=4; i++){
        lua_pushinteger(L, i);
        lua_newtable(L);
        for(int j=1; j<=4; j++){
            lua_pushinteger(L, j);
            lua_pushnumber(L, (*mat)[i-1][j-1]);
            lua_settable(L, -3);
        }
        lua_settable(L, -3);
    }
    return 1;
}


int delete_mat4(lua_State* L){
    delete *(glm::mat4**)(luaL_checkudata(L, 1, "mat4"));
    return 0;
}
