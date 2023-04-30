#include <tiny_linear.hpp>

vector::vector(const vector & mirror){
    size = mirror.size;
    content = new float[size];
    for(size_t i=0; i<size; i++){
        content[i]=mirror.content[i];
    }
}

vector::vector(vector && mirror){
    size = mirror.size;
    content = mirror.content;
    mirror.content =nullptr;
}

vector::vector(size_t size):size(size){
    content = new float[size]();
}

vector::vector(size_t size, const std::vector<float>& list):size(size){
    content = new float[size];
    for(size_t i=0; i<size; i++){
        content[i]=list[i];
    }
}


std::string vector::tostr(void){
    std::string res;
    res.append("{");
    for(size_t i=0; i<size-1; i++){
        res.append(std::to_string(content[i]));
        res.append(", ");
    }
    res.append(std::to_string(content[size-1]));
    res.append("}");
    return res;
}

vector cross_vec_vec(const vector& lvec, const vector& rvec){
    if(lvec.size!=3&&rvec.size!=3){
        fatal("cross two vectors of length %zu and %zu\n", lvec.size, rvec.size)
    }
    vector res(3);
    res.content[0] = lvec.content[1]*rvec.content[2]-lvec.content[2]*rvec.content[1];
    res.content[1] = lvec.content[2]*rvec.content[0]-lvec.content[0]*rvec.content[2];
    res.content[2] = lvec.content[0]*rvec.content[1]-lvec.content[1]*rvec.content[0];
    return res;
}

double dot_vec_vec(const vector& lvec, const vector& rvec){
    if(lvec.size!=rvec.size){
        fatal("dot mul two vectors of length %zu and %zu\n", lvec.size, rvec.size)
    }
    double res=0;
    for(size_t i=0; i< lvec.size; i++){
        res+=lvec.content[i]*rvec.content[i];
    }
    return res;
}

vector add_vec_vec(const vector& lvec, const vector& rvec){
    if(lvec.size!=rvec.size){
        fatal("add two vectors of length %zu and %zu\n", lvec.size, rvec.size)
    }
    vector res(lvec.size);
    for(size_t i=0; i<res.size; i++){
        res.content[i]=lvec.content[i]+rvec.content[i];
    }
    return res;
}

vector sub_vec_vec(const vector& lvec, const vector& rvec){
    if(lvec.size!=rvec.size){
        fatal("sub two vectors of length %zu and %zu\n", lvec.size, rvec.size)
    }
    vector res(lvec.size);
    for(size_t i=0; i<res.size; i++){
        res.content[i]=lvec.content[i]-rvec.content[i];
    }
    return res;
}

float vec_len(const vector& vec){
    float acc;
    for(int i=0; i<vec.size; i++){
        acc+=vec.content[i]*vec.content[i];
    }
    return std::sqrt(acc);
}

vector normalize_vec(const vector& vec){
    vector res(vec);
    float len = vec_len(res);
    for(int i=0; i<res.size; i++){
        res.content[i]/=len;
    }
    return res;
}

vector neg_vec(const vector& vec){
    vector res(vec);
    for(int i=0; i<res.size; i++){
        res.content[i]=-res.content[i];
    }
    return res;
}

//该函数应配合new_vec使用,设置栈顶上的元素的元表为mesh
void create_vec_table(lua_State* L){
    //该函数可以避免元表被重复注册,并把元表放在栈顶
    if(luaL_newmetatable(L, "vec")){
        static const luaL_Reg functions[] =
        {
            {"__gc", delete_vec},
            {"__tostring", vec_to_string},
            {"__unm", neg_vec_lua},
            {"__add", add_vec_lua},
            {"__mul", mul_vec_lua},
            {"__pow", cross_vec_lua},
            {"__index",index_vec_lua},
            {"__newindex", modify_vec_lua},
            {nullptr, nullptr}
        };
        //这个函数把上面的函数填入表
        luaL_setfuncs(L, functions, 0);
    }
    //把栈的-2处的元素的元表设为栈顶元素(即matrix的元表),这也会自动弹出元表,现在栈顶元素是设置了元表的userdata
    lua_setmetatable(L, -2);
    return;
}

int new_vec(lua_State* L){
    size_t size = luaL_checkinteger(L, 1);
    luaL_checktype(L, 2, LUA_TTABLE);
    size_t list_len = lua_objlen(L,2);
    if(list_len<size){
        luaL_error(L, "fail to create vector, expect %zu elements, only get %zu", size, list_len);
    }
    std::vector<float> list(size);
    for(int i=1; i<=size; i++){
        lua_pushinteger(L, i);
        lua_gettable(L, 2);
        list[i-1]=luaL_checknumber(L, -1);
    }
    void* vec_pp = lua_newuserdata(L, sizeof(void*));
    *(vector**)vec_pp = new vector(size, list);
    create_vec_table(L);
    return 1;
};

int delete_vec(lua_State* L){
    delete *(vector**)(luaL_checkudata(L, 1, "vec"));
    return 0;
};

int vec_to_string(lua_State* L){
    vector* vec = *(vector**)(luaL_checkudata(L, 1, "vec"));
    lua_pushstring(L, vec->tostr().c_str());
    return 1;
};

int get_vec_size(lua_State* L){
    vector* vec = *(vector**)(luaL_checkudata(L, 1, "vec"));
    lua_pushinteger(L, vec->size);
    return 1;
};

int neg_vec_lua(lua_State* L){
    vector* vec= *(vector**)(luaL_checkudata(L, 1, "vec"));
    vector* res = new vector(neg_vec(*vec));
    void* vec_pp = lua_newuserdata(L, sizeof(void*));
    *(vector**)vec_pp = res;
    create_vec_table(L);
    return 1;
}

int add_vec_lua(lua_State* L){
    vector* vecl = *(vector**)(luaL_checkudata(L, 1, "vec"));
    vector* vecr = *(vector**)(luaL_checkudata(L, 2, "vec"));
    vector* res = new vector(add_vec_vec(*vecl, *vecr));
    void* vec_pp = lua_newuserdata(L, sizeof(void*));
    *(vector**)vec_pp = res;
    create_vec_table(L);
    return 1;
}

int sub_vec_lua(lua_State* L){
    vector* vecl = *(vector**)(luaL_checkudata(L, 1, "vec"));
    vector* vecr = *(vector**)(luaL_checkudata(L, 2, "vec"));
    vector* res = new vector(sub_vec_vec(*vecl, *vecr));
    void* vec_pp = lua_newuserdata(L, sizeof(void*));
    *(vector**)vec_pp = res;
    create_vec_table(L);
    return 1;
}

int mul_vec_lua(lua_State* L){
    vector* vecl = *(vector**)(luaL_checkudata(L, 1, "vec"));
    vector* vecr = *(vector**)(luaL_checkudata(L, 2, "vec"));
    double res = dot_vec_vec(*vecl, *vecl);
    lua_pushnumber(L, res);
    return 1;
};

int cross_vec_lua(lua_State* L){
    vector* vecl = *(vector**)(luaL_checkudata(L, 1, "vec"));
    vector* vecr = *(vector**)(luaL_checkudata(L, 2, "vec"));
    vector* res = new vector(cross_vec_vec(*vecl, *vecr));
    void* vec_pp = lua_newuserdata(L, sizeof(void*));
    *(vector**)vec_pp = res;
    create_vec_table(L);
    return 1;
};

int index_vec_lua(lua_State* L){
    vector* vec = *(vector**)(luaL_checkudata(L, 1, "vec"));
    size_t index = luaL_checkinteger(L, 2);
    if(index>vec->size){
        luaL_error(L, "access vector index %zu, but vector size is %zu", index, vec->size);
    }
    lua_pushnumber(L, vec->content[index-1]);
    return 1;
}

int modify_vec_lua(lua_State* L){
    vector* vec = *(vector**)(luaL_checkudata(L, 1, "vec"));
    size_t index = luaL_checkinteger(L, 2);
    if(index>vec->size){
        luaL_error(L, "modify vector index %zu, but vector size is %zu", index, vec->size);
    }
    vec->content[index-1]=luaL_checknumber(L, 3);;
    return 0;
}


