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
matrix diag_mat(size_t len, float diag){
    matrix res(len, len);
    for(int i=0; i<len; i++){
        res.content[i*len+i]=diag;
    }
    return res;
};

//创建一个正视投影矩阵,该函数的语义与glm::ortho一致,
//把右手系下的[left,right],[top, bottom],[-near, -far]空间内的坐标线性映射到opengl空间[-1,1],[-1,1],[-1,1](opengl使用左手系,-1位于最前面)
matrix ortho_mat(double left, double right, double top, double bottom, double near, double far){
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
matrix perspective_mat(double fov, double aspect, double near, double far){
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

matrix add_mat_mat(const matrix& matl, const matrix& matr){
    if(matl.col!=matr.col||matl.row!=matr.row){
        fatal("add matrix of size (%zu, %zu) and (%zu, %zu)\n", matl.col, matl.row, matr.col, matr.row);
    }
    matrix res(matl.col, matl.row);
    for(size_t c=0; c<matl.col; c++){
        for(size_t r=0; r<matl.row; r++){
            res.content[c*matl.row+r] = matl.content[c*matl.row+r]+matr.content[c*matl.row+r];
        }
    }
    return res;
}

matrix sub_mat_mat(const matrix& matl, const matrix& matr){
    if(matl.col!=matr.col||matl.row!=matr.row){
        fatal("sub matrix of size (%zu, %zu) and (%zu, %zu)\n", matl.col, matl.row, matr.col, matr.row);
    }
    matrix res(matl.col, matl.row);
    for(size_t c=0; c<matl.col; c++){
        for(size_t r=0; r<matl.row; r++){
            res.content[c*matl.row+r] = matl.content[c*matl.row+r]-matr.content[c*matl.row+r];
        }
    }
    return res;
}

matrix mul_mat_mat(const matrix& matl, const matrix& matr){
    if(matl.col!=matr.row){
        fatal("multiply matrix of size (%zu, %zu) and (%zu, %zu)\n", matl.col, matl.row, matr.col, matr.row);
    }
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

matrix mul_mat_num(const matrix& mat, double num){
    matrix res(mat);
    for(size_t i=0; i<res.row*res.col; i++){
        res.content[i]*=num;
    }
    return res;
}

vector mul_mat_vec(const matrix& mat, const vector& vec){
    if(vec.size!=mat.col){
        fatal("mul matrix of size (%zu, %zu) to vector of size %zu to \n",mat.col, mat.row, vec.size);
    }
    vector res(mat.row);
    for(size_t r=0; r<mat.row; r++){
        for(size_t c=0; c<mat.col; c++)
        res.content[r]+=vec.content[c]*mat.content[c*mat.row+r];
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


matrix inverse_mat(const matrix& mat){
    if(mat.col!=mat.row){
        fatal("inverse matrix of size (%zu, %zu)\n", mat.col, mat.row);
    }
    size_t len = mat.col;
    matrix res(diag_mat(len, 1.0f));
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

matrix transpose_mat(const matrix& mat){
    matrix res(mat.row, mat.col);
    for(size_t res_c=0; res_c<res.col; res_c++){
        for(size_t res_r=0; res_r<res.row; res_r++){
            res.content[res_c*res.row+res_r]=mat.content[res_r*mat.row+res_c];
        }
    }
    return res;
}

matrix move_mat(const vector& pos){
    if(pos.size!=3){
        fatal("create a rotate matrix from vector of length %zu\n", pos.size);
    }
    matrix res(diag_mat(4, 1));
    res.content[12]=pos.content[0];
    res.content[13]=pos.content[1];
    res.content[14]=pos.content[2];
    return res;
}

//推导参见https://zhuanlan.zhihu.com/p/56587491
matrix rotate_mat(float angle, const vector& axis){
    if(axis.size!=3){
        fatal("create a rotate matrix from vector of length %zu\n", axis.size);
    }
    vector nom_axis = normalize_vec(axis);
    float x = nom_axis.content[0];float y = nom_axis.content[1];float z = nom_axis.content[2];
    matrix res(4,4);
    double angle_cos = std::cos(angle);
    double angle_sin = std::sin(angle);
    res.content[0]=x*x*(1-angle_cos)+angle_cos;
    res.content[1]=x*y*(1-angle_cos)+z*angle_sin;
    res.content[2]=x*z*(1-angle_cos)-y*angle_sin;
    res.content[4]=x*y*(1-angle_cos)-z*angle_sin;
    res.content[5]=y*y*(1-angle_cos)+angle_cos;
    res.content[6]=y*z*(1-angle_cos)+x*angle_sin;
    res.content[8]=x*z*(1-angle_cos)+y*angle_sin;
    res.content[9]=y*z*(1-angle_cos)-x*angle_sin;
    res.content[10]=z*z*(1-angle_cos)+angle_cos;
    res.content[15]=1.0;
    return res;
}

matrix lookat_mat(const vector& campos, const vector& target, const vector& world_up){
    if(campos.size!=3||target.size!=3||world_up.size!=3){
        fatal("create lookat matrix with vectors of size %zu, %zu, %zu\n", campos.size, target.size, world_up.size)
    }
    vector cam_d = normalize_vec(sub_vec_vec(campos,target));
    vector cam_r = normalize_vec(cross_vec_vec(world_up,cam_d));
    //d和r一定正交,因此这里不用normalize
    vector cam_up = cross_vec_vec(cam_d,cam_r);
    //该矩阵未考虑相机的平移
    matrix look_rotate(4,4);
    look_rotate.content[0]=cam_r.content[0];
    look_rotate.content[1]=cam_up.content[0];
    look_rotate.content[2]=cam_d.content[0];
    look_rotate.content[4]=cam_r.content[1];
    look_rotate.content[5]=cam_up.content[1];
    look_rotate.content[6]=cam_d.content[1];
    look_rotate.content[8]=cam_r.content[2];
    look_rotate.content[9]=cam_up.content[2];
    look_rotate.content[10]=cam_d.content[2];
    look_rotate.content[15]=1.0;
    //需要额外乘以一个平移矩阵
    return mul_mat_mat(look_rotate, move_mat(neg_vec(campos)));
}

matrix scale_mat(float x, float y, float z){
    matrix res(4,4);
    res.content[0]=x;
    res.content[5]=y;
    res.content[10]=z;
    res.content[15]=1.0;
    return res;
}

//该函数应配合new_mat使用,设置栈顶上的元素的元表为mesh
void create_mat_table(lua_State* L){
    //该函数可以避免元表被重复注册,并把元表放在栈顶
    if(luaL_newmetatable(L, "mat")){
        static const luaL_Reg functions[] =
        {
            {"__gc", delete_mat_lua},
            {"__tostring", mat_to_string_lua},
            {"__add", add_mat_lua},
            {"__mul", mul_mat_lua},
            {"__index",index_mat_lua},
            {"__newindex", modify_mat_lua},
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

int id_mat_lua(lua_State* L){
    size_t len = luaL_checkinteger(L, 1);
    float diag = luaL_checknumber(L, 2);
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)mat_pp = new matrix(diag_mat(len, diag));
    create_mat_table(L);
    return 1;
};

int col_mat_lua(lua_State* L){
    size_t col = luaL_checkinteger(L, 1);
    size_t row = luaL_checkinteger(L, 2);
    size_t len = col*row;
    luaL_checktype(L, 3, LUA_TTABLE);
    size_t list_len = lua_objlen(L,3);
    if(list_len!=len){
        luaL_error(L, "fail to create matrix, expect %zu elements, but get %zu", len, list_len);
    }
    std::vector<float> list(len);
    for(int i=1; i<=len; i++){
        lua_pushinteger(L, i);
        lua_gettable(L, 3);
        list[i-1]=luaL_checknumber(L, -1);
    }
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)mat_pp = new matrix(col, row, list, false);
    create_mat_table(L);
    return 1;
};

int row_mat_lua(lua_State* L){
    size_t col = luaL_checkinteger(L, 1);
    size_t row = luaL_checkinteger(L, 2);
    size_t len = col*row;
    luaL_checktype(L, 3, LUA_TTABLE);
    size_t list_len = lua_objlen(L,3);
    if(list_len<len){
        luaL_error(L, "fail to create matrix, expect %zu elements, only get %zu", len, list_len);
    }
    std::vector<float> list(len);
    for(int i=1; i<=len; i++){
        lua_pushinteger(L, i);
        lua_gettable(L, 3);
        list[i-1]=luaL_checknumber(L, -1);
    }
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)mat_pp = new matrix(col, row, list , true);
    create_mat_table(L);
    return 1;
};

int ortho_mat_lua(lua_State* L){
    double left = luaL_checknumber(L, 1);
    double right = luaL_checkinteger(L, 2);
    double top = luaL_checkinteger(L, 3);
    double bottom = luaL_checkinteger(L, 4);
    double near = luaL_checkinteger(L, 5);
    double far = luaL_checkinteger(L, 6);
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)mat_pp = new matrix(ortho_mat(left, right, top, bottom, near, far));
    create_mat_table(L);
    return 1;
};

int perspective_mat_lua(lua_State* L){
    double fov = luaL_checknumber(L, 1);
    double aspect = luaL_checknumber(L, 2);
    double near = luaL_checknumber(L, 3);
    double far = luaL_checknumber(L, 4);
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    //fov:field of view,视野
    *(matrix**)mat_pp = new matrix(perspective_mat(fov, aspect, near, far));
    create_mat_table(L);
    return 1;
};

int move_mat_lua(lua_State* L){
    vector* pos = *(vector**)(luaL_checkudata(L, 1, "vec"));
    matrix* res = new matrix(move_mat(*pos));
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)mat_pp = res;
    create_mat_table(L);
    return 1;
}

int rotate_mat_lua(lua_State* L){
    double angle = luaL_checknumber(L, 1);
    vector* axis = *(vector**)(luaL_checkudata(L, 2, "vec"));
    matrix* res = new matrix(rotate_mat(angle, *axis));
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)mat_pp = res;
    create_mat_table(L);
    return 1;
}

int lookat_mat_lua(lua_State* L){
    vector* campos = *(vector**)(luaL_checkudata(L, 1, "vec"));
    vector* target = *(vector**)(luaL_checkudata(L, 2, "vec"));
    vector* worldup = *(vector**)(luaL_checkudata(L, 3, "vec"));
    matrix* res = new matrix(lookat_mat(*campos, *target, *worldup));
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)mat_pp = res;
    create_mat_table(L);
    return 1;
}


int scale_mat_lua(lua_State* L){
    size_t par_num = lua_gettop(L);
    matrix* res;
    if(par_num==1){
        double scale_size = luaL_checknumber(L, 1);
        res = new matrix(scale_mat(scale_size, scale_size, scale_size));
    }else{
        double scale_x = luaL_checknumber(L, 1);
        double scale_y = luaL_checknumber(L, 2);
        double scale_z = luaL_checknumber(L, 3);
        res = new matrix(scale_mat(scale_x, scale_y, scale_z));
    }
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)mat_pp = res;
    create_mat_table(L);
    return 1;
}


int delete_mat_lua(lua_State* L){
    delete *(matrix**)(luaL_checkudata(L, 1, "mat"));
    return 0;
};

int mat_to_string_lua(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    lua_pushstring(L, mat->tostr().c_str());
    return 1;
};

int get_mat_col_lua(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    lua_pushinteger(L, mat->col);
    return 1;
};
int get_mat_row_lua(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    lua_pushinteger(L, mat->row);
    return 1;
};

int add_mat_lua(lua_State* L){
    matrix* matl = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    matrix* matr = *(matrix**)(luaL_checkudata(L, 2, "mat"));
    matrix* res = new matrix(add_mat_mat(*matl, *matr));
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)mat_pp = res;
    create_mat_table(L);
    return 1;
}

int sub_mat_lua(lua_State* L){
    matrix* matl = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    matrix* matr = *(matrix**)(luaL_checkudata(L, 2, "mat"));
    matrix* res = new matrix(sub_mat_mat(*matl, *matr));
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)mat_pp = res;
    create_mat_table(L);
    return 1;
}

//不加括号时,lua的二元运算符的顺序不定(不能假定lua会优先查找哪个操作数的元方法,全写一遍算了)
//例如a+b+c居然会先调用c的元方法!!!
int mul_mat_lua(lua_State* L){
    //如果第一个参数是vec
    if(luaL_testudata (L, 1, "mat")&&luaL_testudata(L, 2, "vec")){
        matrix* mat = *(matrix**)lua_touserdata(L, 1);
        vector* vec = *(vector**)lua_touserdata(L, 2);
        vector* res = new vector(mul_mat_vec(*mat, *vec));
        void* vec_pp = lua_newuserdata(L, sizeof(void*));
        *(vector**)vec_pp = res;
        create_vec_table(L);
        return 1;
    }else if(luaL_testudata (L, 1, "vec")&&luaL_testudata(L, 2, "mat")){
        vector* vec = *(vector**)lua_touserdata(L, 1);
        matrix* mat = *(matrix**)lua_touserdata(L, 2);
        vector* res = new vector(mul_vec_mat(*vec, *mat));
        void* vec_pp = lua_newuserdata(L, sizeof(void*));
        *(vector**)vec_pp = res;
        create_vec_table(L);
        return 1;
    }else if(luaL_testudata (L, 1, "mat")&&luaL_testudata(L, 2, "mat")){
        matrix* matl = *(matrix**)lua_touserdata(L, 1);
        matrix* matr = *(matrix**)lua_touserdata(L, 2);
        matrix* res = new matrix(mul_mat_mat(*matl, *matr));
        void* mat_pp = lua_newuserdata(L, sizeof(void*));
        *(matrix**)mat_pp = res;
        create_mat_table(L);
        return 1;
    }else if(luaL_testudata (L, 1, "mat")&&lua_type(L,2)==LUA_TNUMBER){
        matrix* mat = *(matrix**)lua_touserdata(L, 1);
        double num = lua_tonumber(L, 2);
        matrix* res = new matrix(mul_mat_num(*mat, num));
        void* mat_pp = lua_newuserdata(L, sizeof(void*));
        *(matrix**)mat_pp = res;
        create_mat_table(L);
        return 1;
    }else if(lua_type(L,1)==LUA_TNUMBER&&luaL_testudata(L, 2, "mat")){
        double num = lua_tonumber(L, 1);
        matrix* mat = *(matrix**)lua_touserdata(L, 2);
        matrix* res = new matrix(mul_mat_num(*mat, num));
        void* mat_pp = lua_newuserdata(L, sizeof(void*));
        *(matrix**)mat_pp = res;
        create_mat_table(L);
        return 1;
    }else{
        return luaL_error(L, "mul mat with unsupported type");
    }
};

int inverse_mat_lua(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    matrix* res = new matrix(inverse_mat(*mat));
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)mat_pp = res;
    create_mat_table(L);
    return 1;
};

int transpose_mat_lua(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    matrix* res = new matrix(transpose_mat(*mat));
    void* mat_pp = lua_newuserdata(L, sizeof(void*));
    *(matrix**)mat_pp = res;
    create_mat_table(L);
    return 1;
}

int index_mat_lua(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    luaL_checktype(L, 2, LUA_TTABLE);
    lua_pushinteger(L, 1);
    lua_gettable(L, 2) ; size_t col = luaL_checkinteger(L, -1); lua_pop(L,1);
    lua_pushinteger(L, 2);
    lua_gettable(L, 2) ; size_t row = luaL_checkinteger(L, -1); lua_pop(L,1);
    if(col>mat->col||row>mat->row||col<1||row<1){
        luaL_error(L, "access matrix index (%zu, %zu), but matrix size is (%zu, %zu)", col, row, mat->col, mat->row);
    }
    lua_pushnumber(L, mat->content[(col-1)*mat->row+(row-1)]);
    return 1;
}

int modify_mat_lua(lua_State* L){
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    luaL_checktype(L, 2, LUA_TTABLE);
    lua_pushinteger(L, 1);
    lua_gettable(L, 2) ; size_t col = luaL_checkinteger(L, -1); lua_pop(L,1);
    lua_pushinteger(L, 2);
    lua_gettable(L, 2) ; size_t row = luaL_checkinteger(L, -1); lua_pop(L,1);
    if(col>mat->col||row>mat->row||col<1||row<1){
        luaL_error(L, "modify matrix index (%zu, %zu), but matrix size is (%zu, %zu)", col, row, mat->col, mat->row);
    }
    mat->content[(col-1)*mat->row+(row-1)]=luaL_checknumber(L, 3);
    return 0;
}
