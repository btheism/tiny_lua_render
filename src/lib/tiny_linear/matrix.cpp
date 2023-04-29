#include <tiny_linear.hpp>

//该方法返回一个全0矩阵
matrix::matrix(size_t col, size_t row):col(col),row(row){
    int len=row*col;
    if(row==0||col==0){
        fatal("invaild shape\n");
    }
    content = new float[col*row]();
};

//复制矩阵
matrix::matrix(const matrix & mirror){
    row = mirror.row;
    col = mirror.col;
    content = new float[col*row];
    for(size_t i=0; i<col*row; i++){
        content[i]=mirror.content[i];
    }
}

//右值复制
matrix::matrix(matrix && mirror){
    row = mirror.row;
    col = mirror.col;
    content = mirror.content;
    mirror.content=nullptr;
}

//使用向量初始化矩阵,参数由lua检查
//如果row_major为true,则假定输入数据为行优先,否则,假定输入数据为列优先
matrix::matrix(size_t col, size_t row, const std::vector<float>& list, bool row_major):col(col),row(row){
    content = new float[col*row];
    if(!row_major){
        //由于矩阵是按列优先存储的,直接按顺序复制数据
        for(size_t i=0; i<col*row; i++){
            content[i]=list[i];
        }
    }
    else{
        for(size_t r=0; r<row; r++){
            for(size_t c=0; c<col; c++){
                content[c*row+r]=list[r*col+c];
            }
        }
    }
};

//创建一个对角矩阵
matrix diag_matrix(size_t len, float diag){
    matrix res(len, len);
    for(int i=0; i<len; i++){
        res.content[i*len+i]=diag;
    }
    return res;
};

//创建一个正视投影矩阵,该函数的语义与glm::ortho一致,
//把右手系下的[left,right],[top, bottom],[-near, -far]空间内的坐标线性映射到opengl空间[-1,1],[-1,1],[-1,1](opengl使用左手系,-1位于最前面)
matrix ortho_matrix(double left, double right, double top, double bottom, double near, double far){
//matrix ortho_matrix(float left, float right, float top, float bottom, float near, float far){
    matrix res(4, 4);
    //分别计算矩阵的非0元素
    /*
    {{2/(r - l), 0, 0, -(r + l)/(r - l)},
    {0, 2/(t - b), 0, -(t + b)/(t - b)},
    {0, 0, - 2/(f - n), -(f + n)/(f - n)},
    {0, 0, 0, 1}}
    */
    res.content[0]=2.0/(right-left);
    res.content[12]=(left+right)/(left-right);
    res.content[5]=2.0/(bottom-top);
    res.content[13]=(top+bottom)/(top-bottom);
    res.content[10]=2.0/(near-far);
    res.content[14]=(near+far)/(near-far);
    res.content[15]=1.0;
    return res;
};

//生成透视投影矩阵(这里的视角指top和bottom与原点(即水平面)的夹角)
/*
{{Cot[fov/2]/aspect, 0, 0, 0},
{0, Cot[fov/2], 0, 0},
{0, 0, -(f+n)/(f-n), -2*f*n/(f-n)},
{0, 0,-1, 0}}
*/
matrix perspective_matrix(double fov, double aspect, double near, double far){
//matrix ortho_matrix(float left, float right, float top, float bottom, float near, float far){
    matrix res(4, 4);
    //分别计算矩阵的非0元素
    res.content[0]=1.0/(std::tan(fov/2.0)*aspect);
    res.content[5]=1.0/std::tan(fov/2.0);
    res.content[10]=(near+far)/(near-far);
    res.content[14]=2*near*far/(near-far);
    res.content[11]=-1.0;
    return res;
};

std::string matrix::tostr_col_major(void){
    //log("attempt to print matrix\n");
    std::string res;
    res.append("{");
    for(size_t c=0; c<col-1; c++){
        res.append("{");
        for(size_t r=0; r<row-1; r++){
            res.append(std::to_string(content[row*c+r]));
            res.append(", ");
        }
        res.append(std::to_string(content[row*c+(row-1)]));
        res.append("}, \n");
    }
    res.append("{");
    for(size_t r=0; r<row-1; r++){
        res.append(std::to_string(content[row*(col-1)+r]));
        res.append(", ");
    }
    res.append(std::to_string(content[row*(col-1)+(row-1)]));
    res.append("}");
    res.append("}");
    return res;
}

std::string matrix::tostr(void){
    std::string res;
    res.append("{");
    for(size_t r=0; r<row-1; r++){
        res.append("{");
        for(size_t c=0; c<col-1; c++){
            res.append(std::to_string(content[row*c+r]));
            res.append(", ");
        }
        res.append(std::to_string(content[row*(col-1)+r]));
        res.append("}, \n");
    }
    res.append("{");
    for(size_t c=0; c<col-1; c++){
        res.append(std::to_string(content[row*c+(row-1)]));
        res.append(", ");
    }
    res.append(std::to_string(content[row*(col-1)+(row-1)]));
    res.append("}");
    res.append("}");
    return res;
}

//这些矩阵函数的参数检查在lua处进行
matrix add_matrix(const matrix& matl, const matrix& matr){
    matrix res(matl.col, matl.row);
    for(size_t c=0; c<matl.col; c++){
        for(size_t r=0; r<matl.row; r++){
            res.content[c*matl.row+r] = matl.content[c*matl.row+r]+matr.content[c*matl.row+r];
        }
    }
    return res;
}

matrix sub_matrix(const matrix& matl, const matrix& matr){
    matrix res(matl.col, matl.row);
    for(size_t c=0; c<matl.col; c++){
        for(size_t r=0; r<matl.row; r++){
            res.content[c*matl.row+r] = matl.content[c*matl.row+r]-matr.content[c*matl.row+r];
        }
    }
    return res;
}

matrix mul_matrix(const matrix& matl, const matrix& matr){
    matrix res(matl.row, matr.col);
    for(size_t c=0; c<matr.col; c++){
        for(size_t r=0; r<matl.row; r++){
            float acc=0;
            for(size_t k=0; k<matl.col; k++){
                //加上 matl[k][r]*matr[c][k]   ([列][行])
                acc+= matl.content[k*matl.row+r]*matr.content[c*matr.row+k];
            }
            res.content[c*matl.row+r] = acc;
        }
    }
    return res;
}

//以下3个内联函数用于求解逆矩阵
//r1,r2是矩阵的行,以下函数对矩阵进行初等行变换,假设行序号以0开始
static inline void swap_row(const matrix& mat, size_t r1, size_t r2){
    for(size_t c=0; c<mat.col; c++){
        std::swap(mat.content[c*mat.row+r1], mat.content[c*mat.col+r2]);
    }
}
//r1=r1-r2*factor
static inline void sub_row(const matrix& mat, size_t r1, size_t r2, float factor){
    for(size_t c=0; c<mat.col; c++){
        mat.content[c*mat.row+r1]-=mat.content[c*mat.col+r2]*factor;
    }
}
//r=r*factor
static inline void div_row(const matrix& mat, size_t r, float factor){
    for(size_t c=0; c<mat.col; c++){
        mat.content[c*mat.row+r]/=factor;
    }
}


matrix inverse_matrix(const matrix& mat){
    size_t len = mat.col;
    matrix res(diag_matrix(len, 1.0f));
    matrix tmp_mat(mat);
    //逐列化为(0...1...0)的形式
    for(size_t c=0; c<len; c++){
        //寻找主元
        size_t max_row=c;
        float max_val=std::abs(tmp_mat.content[c*len+c]);
        for(size_t r=c+1; r<len; r++){
            if(std::abs(tmp_mat.content[c*len+r])>max_val){
                max_row=r;
                max_val=std::abs(tmp_mat.content[c*len+r]);
            }
        }
        if(max_val==0.0f){
            continue;
        }
        if(max_row!=c){
            swap_row(tmp_mat, max_row, c);
            swap_row(res, max_row, c);
        }
        //把主元化为1
        float pivot = tmp_mat.content[c*len+c];
        if(pivot!=1.0f){
            div_row(tmp_mat, c, pivot);
            div_row(res, c, pivot);
        }
        for(size_t r=0; r<len; r++){
            if(r==c){
                continue;
            }
            //这里必须保存减数,否则调用sub_row(*res...)时tmp_mat.content[c*len+r]已经被改变
            float sub_factor = tmp_mat.content[c*len+r];
            sub_row(tmp_mat, r, c, sub_factor);
            sub_row(res, r, c, sub_factor);
        }
    }
    return res;
}

matrix transpose_matrix(const matrix& mat){
    matrix res(mat.row, mat.col);
    for(size_t res_c=0; res_c<res.col; res_c++){
        for(size_t res_r=0; res_r<res.row; res_r++){
            res.content[res_c*res.row+res_r]=mat.content[res_r*mat.row+res_c];
        }
    }
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
            {"__add", add_matrix_lua},
            {"__mul", mul_matrix_lua},
            //{"__bnot", inverse_matrix_lua},//使用~算符对矩阵求逆,(一共只有两个一元运算符,'~'和'-','~'似乎是唯一适合使用的算符,然而luajit5.1似乎不支持
            {nullptr, nullptr}
        };
        //这个函数把上面的函数填入表
        luaL_setfuncs(L, functions, 0);
        //复制元表自身
        lua_pushvalue(L, -1);
        //设置元表的"__index"指向自身
        lua_setfield(L, -2, "__index");
    }
    //把栈的-2处的元素的元表设为栈顶元素(即matrix的元表),这也会自动弹出元表,现在栈顶元素是设置了元表的userdata
    lua_setmetatable(L, -2);
    return;
}

//这些形如new_*_matrix函数需要配合new_matrix使用,new_matrix根据传入的第一个字符串决定调用哪个函数,参数表从2开始
int new_id_matrix(lua_State* L){
    size_t len = luaL_checkinteger(L, 2);
    float diag = luaL_checknumber(L, 3);
    void* matrix_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)matrix_pp = new matrix(diag_matrix(len, diag));
    create_matrix_table(L);
    return 1;
};

int new_col_matrix(lua_State* L){
    size_t col = luaL_checkinteger(L, 2);
    size_t row = luaL_checkinteger(L, 3);
    size_t len = col*row;
    luaL_checktype(L, 4, LUA_TTABLE);
    size_t list_len = lua_objlen(L,4);
    if(list_len<len){
        luaL_error(L, "fail to create matrix, expect %zu elements, only get %zu", len, list_len);
    }
    std::vector<float> list(len);
    for(int i=1; i<=len; i++){
        lua_pushinteger(L, i);
        lua_gettable(L, 4);
        list[i-1]=luaL_checknumber(L, -1);
    }
    void* matrix_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)matrix_pp = new matrix(col, row, list, false);
    create_matrix_table(L);
    return 1;
};

int new_row_matrix(lua_State* L){
    size_t col = luaL_checkinteger(L, 2);
    size_t row = luaL_checkinteger(L, 3);
    size_t len = col*row;
    luaL_checktype(L, 4, LUA_TTABLE);
    size_t list_len = lua_objlen(L,4);
    if(list_len<len){
        luaL_error(L, "fail to create matrix, expect %zu elements, only get %zu", len, list_len);
    }
    std::vector<float> list(len);
    for(int i=1; i<=len; i++){
        lua_pushinteger(L, i);
        lua_gettable(L, 4);
        list[i-1]=luaL_checknumber(L, -1);
    }
    void* matrix_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)matrix_pp = new matrix(col, row, list , true);
    create_matrix_table(L);
    return 1;
};

int new_ortho_matrix(lua_State* L){
    double left = luaL_checknumber(L, 2);
    double right = luaL_checkinteger(L, 3);
    double top = luaL_checkinteger(L, 4);
    double bottom = luaL_checkinteger(L, 5);
    double near = luaL_checkinteger(L, 6);
    double far = luaL_checkinteger(L, 7);
    void* matrix_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)matrix_pp = new matrix(ortho_matrix(left, right, top, bottom, near, far));
    create_matrix_table(L);
    return 1;
};

int new_perspective_matrix(lua_State* L){
    double fov = luaL_checknumber(L, 2);
    double aspect = luaL_checknumber(L, 3);
    double near = luaL_checknumber(L, 4);
    double far = luaL_checknumber(L, 5);
    void* matrix_pp = lua_newuserdata(L, sizeof(void*));
    //fov:field of view,视野
    *(matrix**)matrix_pp = new matrix(perspective_matrix(fov, aspect, near, far));
    create_matrix_table(L);
    return 1;
};

/*
从lua的{{},{}}结构中创建矩阵似乎没什么意义,有new_serial_matrix就够了
int new_table_matrix(lua_State* L){
    return 1;
};
*/

const std::unordered_map<const std::string, int(*)(lua_State* L), std::hash<std::string>> new_matrix_jump_table{
{"id", new_id_matrix},
{"col", new_col_matrix},
{"row", new_row_matrix},
{"ortho", new_ortho_matrix},
{"persp", new_perspective_matrix},
//{"table", new_table_matrix}
};

int new_matrix(lua_State* L){
    const char* type = luaL_checkstring(L, 1);
    if(!new_matrix_jump_table.count(type)){
        luaL_error(L, "unknown new matrix type %s", *type);
    }
    return new_matrix_jump_table.at(type)(L);
};

int delete_matrix(lua_State* L){
    delete *(matrix**)(luaL_checkudata(L, 1, "mat"));
    return 0;
};

int matrix_to_string(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    lua_pushstring(L, mat->tostr().c_str());
    return 1;
};

int get_matrix_col(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    lua_pushinteger(L, mat->col);
    return 1;
};
int get_matrix_row(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    lua_pushinteger(L, mat->row);
    return 1;
};

int add_matrix_lua(lua_State* L){
    matrix* matl = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    matrix* matr = *(matrix**)(luaL_checkudata(L, 2, "mat"));
    if(matl->col!=matr->col||matl->row!=matr->row){
        luaL_error(L, "add matrix of size (%zu, %zu) and (%zu, %zu)", matl->col, matl->row, matr->col, matr->row);
    }
    void* matrix_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)matrix_pp = new matrix(add_matrix(*matl, *matr));
    create_matrix_table(L);
    return 1;
}

int sub_matrix_lua(lua_State* L){
    matrix* matl = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    matrix* matr = *(matrix**)(luaL_checkudata(L, 2, "mat"));
    if(matl->col!=matr->col||matl->row!=matr->row){
        luaL_error(L, "sub matrix of size (%zu, %zu) and (%zu, %zu)", matl->col, matl->row, matr->col, matr->row);
    }
    void* matrix_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)matrix_pp = new matrix(sub_matrix(*matl, *matr));
    create_matrix_table(L);
    return 1;
}

int mul_matrix_lua(lua_State* L){
    matrix* matl = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    matrix* matr = *(matrix**)(luaL_checkudata(L, 2, "mat"));
    if(matl->col!=matr->row){
        luaL_error(L, "multiply matrix of size (%zu, %zu) and (%zu, %zu)", matl->col, matl->row, matr->col, matr->row);
    }
    void* matrix_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)matrix_pp = new matrix(mul_matrix(*matl, *matr));
    create_matrix_table(L);
    return 1;
};

int inverse_matrix_lua(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    if(mat->row!=mat->col){
        luaL_error(L, "inverse a matrix of size (%zu, %zu)", mat->col, mat->row);
    }
    void* matrix_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)matrix_pp = new matrix(inverse_matrix(*mat));
    create_matrix_table(L);
    return 1;
};

int transpose_matrix_lua(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    void* matrix_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)matrix_pp = new matrix(transpose_matrix(*mat));
    create_matrix_table(L);
    return 1;
}




