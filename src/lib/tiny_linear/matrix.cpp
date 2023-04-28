#include <tiny_linear.hpp>

matrix::matrix(size_t col, size_t row):col(col),row(row){
    int len=row*col;
    if(row==0||col==0){
        fatal("invaild shape\n");
    }
    content = new float[col*row]();
};
matrix::matrix(size_t col, size_t row, float diag):col(col),row(row){
    if(row!=col){
        fatal("invaild shape for creating identity matrix \n");
    }
    content = new float[row*row]();
    for(int i=0; i<col; i++){
        content[col*col+i]=diag;
    }
};
std::string matrix::tostr(void){
    log("attempt to print matrix\n");
    std::string res;
    res.append("{");
    for(size_t c=0; c<col-1; c++){
        res.append("{");
        for(size_t r=0; r<row-1; r++){
            res.append(std::to_string(content[row*c+r]));
            res.append(", ");
        }
        res.append(std::to_string(content[row*c+(row-1)]));
        res.append("}, ");
    }
    for(size_t r=0; r<row-1; r++){
        res.append(std::to_string(content[row*(col-1)+r]));
        res.append(", ");
    }
    res.append(std::to_string(content[row*(col-1)+(row-1)]));
    res.append("}");
    res.append("}");
    return res;
}

//该函数应配合new_mesh使用,设置栈顶上的元素的元表为mesh
void create_matrix_table(lua_State* L){
    //该函数可以避免元表被重复注册,并把元表放在栈顶
    if(luaL_newmetatable(L, "mat")){
        static const luaL_Reg functions[] =
        {
            {"__gc", delete_matrix},
            {"__tostring", matrix_to_string},
            {nullptr, nullptr}
        };
        //这个函数把上面的函数填入表
        luaL_setfuncs(L, functions, 0);
        //由于__index有额外的重载,因此这里不把index指向元表自身(即不能通过矩阵自己调用这些方法)
    }
    //把栈的-2处的元素的元表设为栈顶元素(即matrix的元表),这也会自动弹出元表,现在栈顶元素是设置了元表的userdata
    lua_setmetatable(L, -2);
    return;
}

int new_matrix(lua_State* L){
    size_t col = luaL_checkinteger(L, 1);
    size_t row = luaL_checkinteger(L, 2);

    void* matrix_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)matrix_pp = new matrix(col, row);
    create_matrix_table(L);
    return 1;
}


int delete_matrix(lua_State* L){
    delete *(matrix**)(luaL_checkudata(L, 1, "mat"));
    return 0;
}


int matrix_to_string(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    lua_pushstring(L, mat->tostr().c_str());
    return 1;
}




