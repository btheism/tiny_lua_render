#include <tiny_shader.hpp>

/*
shader* shader_from_file(const char* vertex_shader_path, const char* fragment_shader_path){
        file_buffer vertex_shader_code = read_from_PHYSFS(vertex_shader_path);
        file_buffer fragment_shader_code = read_from_PHYSFS(fragment_shader_path);
        shader* new_shader = new shader(vertex_shader_code.content, fragment_shader_code.content);
        delete vertex_shader_code.content;
        delete fragment_shader_code.content;
        return new_shader;
};
*/

const std::unordered_map<GLenum,const char*> shadertype2str_table{
    {GL_VERTEX_SHADER, "vertex"},
    {GL_TESS_CONTROL_SHADER, "tess control"},
    {GL_TESS_EVALUATION_SHADER, "tess eval"},
    {GL_GEOMETRY_SHADER, "geometry"},
    {GL_FRAGMENT_SHADER, "fragment"},
    {GL_COMPUTE_SHADER, "compute"}
};

const std::unordered_map<const std::string, GLenum, std::hash<std::string>> str2shadertype_table{
    {"vertex", GL_VERTEX_SHADER},
    {"tess_c", GL_TESS_CONTROL_SHADER},
    {"tess_e", GL_TESS_EVALUATION_SHADER},
    {"geom", GL_GEOMETRY_SHADER},
    {"frag", GL_FRAGMENT_SHADER},
    {"comp", GL_COMPUTE_SHADER}
};

GLuint compile_shader_code(const char* shader_code, GLenum shaderType){
    GLuint shaderID;
    GL_CHECK(shaderID = glCreateShader(shaderType));
    GL_CHECK(glShaderSource(shaderID, 1, &shader_code, NULL));
    GL_CHECK(glCompileShader(shaderID));
    int success;
    char infoLog[1024];
    GL_CHECK(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
        fatal("opengl %s shader error:\n%s\n", shadertype2str_table.at(shaderType), infoLog)
    }
    return shaderID;
}

shader::shader(const std::vector<std::pair<const char*, GLenum>> & codes){
    std::vector<GLuint> shaders;
    for(auto code:codes){
        shaders.push_back(compile_shader_code(code.first, code.second));
    }
    ID = glCreateProgram();
    for(GLuint shader:shaders){
        GL_CHECK(glAttachShader(ID, shader));
    }
    GL_CHECK(glLinkProgram(ID));
    int success;
    char infoLog[1024];
    GL_CHECK(glGetProgramiv(ID, GL_LINK_STATUS, &success));
    if (!success)
    {
        glGetProgramInfoLog(ID, 1024, NULL, infoLog);
        fatal("opengl program error:\n%s\n", infoLog)
    }
    for(GLuint shader:shaders){
        GL_CHECK(glDeleteShader(shader));
    }
}

static const std::unordered_map<int, void(**)(GLuint, GLint, GLsizei, const GLfloat*)>setvec_func_table{
    {1, &glProgramUniform1fv},
    {2, &glProgramUniform2fv},
    {3, &glProgramUniform3fv},
    {4, &glProgramUniform4fv}
};
//使用col*16+row计算每个矩阵对应的函数的key值,从而把不同维度的函数映射到不同的函数
//这些函数指针的值会在运行时改变,因此需要存储指针的地址,不能直接存储指针(作为全局变量时未初始化)的值
static const std::unordered_map<int, void(**)(GLuint, GLint, GLsizei, GLboolean, const GLfloat*)>setmat_func_table{
    //这些函数的第一个数表示列,第二个表示行
    {0x22, &glProgramUniformMatrix2fv},
    {0x33, &glProgramUniformMatrix3fv},
    {0x44, &glProgramUniformMatrix4fv},
    {0x23, &glProgramUniformMatrix2x3fv},
    {0x32, &glProgramUniformMatrix3x2fv},
    {0x24, &glProgramUniformMatrix2x4fv},
    {0x42, &glProgramUniformMatrix4x2fv},
    {0x34, &glProgramUniformMatrix3x4fv},
    {0x43, &glProgramUniformMatrix4x3fv}
};

void shader::setMat(const char* name, const matrix &mat, GLboolean transpose) const{
    if(!setmat_func_table.count((mat.col<<4)+mat.row)){
        fatal("do not support set matrix of size (%zu, %zu) in shader\n", mat.col, mat.row)
    }
    GL_CHECK((**setmat_func_table.at((mat.col<<4)+mat.row))(ID, get_uniform_loc(name), 1, transpose, mat.content));
}

void shader::setVec(const char* name, const vector &vec) const{
    if(!setvec_func_table.count(vec.size)){
        fatal("do not support set vector of size %zu in shader\n", vec.size)
    }
    GL_CHECK((**setvec_func_table.at(vec.size))(ID, get_uniform_loc(name), 1, vec.content));
}

//该函数应配合new_shader使用,设置栈顶上的元素的元表为shader
void create_shader_table(lua_State* L){
    //该函数可以避免元表被重复注册,并把元表放在栈顶
    //元表位于registry表中,无须担心名称冲突
    if(luaL_newmetatable(L, "shader")){
        static const luaL_Reg functions[] =
        {
            {"__gc", delete_shader_lua},
            {"use", use_shader_lua},
            {"set_int", set_shader_int_lua},
            {"set_ints", set_shader_ints_lua},
            {"set_float", set_shader_float_lua},
            {"set_floats", set_shader_floats_lua},
            {"set_mat", set_shader_mat_lua},
            {"set_vec", set_shader_vec_lua},
            {nullptr, nullptr}
        };
        //这个函数把上面的函数填入表
        luaL_setfuncs(L, functions, 0);
        //复制元表自身
        lua_pushvalue(L, -1);
        //设置元表的"__index"指向自身
        lua_setfield(L, -2, "__index");//该函数把栈的[-1]元素的"__index"设为栈顶元素,这个函数会弹出栈顶元素,因此在设置设置元表的"__index"指向自身前需要复制元表,见https://stackoverflow.com/questions/35589708/how-to-register-c-class-constructor-to-lua-userdata-and-use-it-by-default
    }
    //把栈的-2处的元素的元表设为栈顶元素(即shader的元表),这也会自动弹出元表,现在栈顶元素是设置了元表的userdata
    lua_setmetatable(L, -2);
    return;
}

int new_shader_lua(lua_State* L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    std::vector<std::pair<const char*, GLenum>> shader_codes;
    for(int i=1; i<=lua_objlen(L,1); i++){
        lua_pushinteger(L, i);
        lua_gettable(L, 1);
        //获取着色器类型
        lua_pushinteger(L, 1);
        lua_gettable(L, -2);
        const char* shadertype = luaL_checkstring(L, -1);
        if(!str2shadertype_table.count(shadertype)){
            luaL_error(L, "cannot find shader of type %s", shadertype);
        }
        lua_pop(L, 1);
        //获取着色器字符串
        lua_pushinteger(L, 2);
        lua_gettable(L, -2);
        const char* shadercode = luaL_checkstring(L, -1);
        lua_pop(L, 1);
        lua_pop(L, 1);
        shader_codes.push_back({shadercode, str2shadertype_table.at(shadertype)});
    }
    shader* res =new shader(shader_codes);
    //创建userdata并把原表填充到栈顶
    void* shader_pp = lua_newuserdata(L, sizeof(void*));
    *(shader**)shader_pp = res;
    create_shader_table(L);
    return 1;
}

int delete_shader_lua(lua_State* L){
    delete *(shader**)(luaL_checkudata(L, 1, "shader"));
    return 0;
}

int use_shader_lua(lua_State* L){
    shader* current_shader = *(shader**)(luaL_checkudata(L, 1, "shader"));
    current_shader->use();
    return 0;
}

int set_shader_int_lua(lua_State* L){
    shader* current_shader = *(shader**)(luaL_checkudata(L, 1, "shader"));
    const char* name = luaL_checkstring(L, 2);
    GLint value = luaL_checkinteger(L, 3);
    current_shader->set_int(name, value);
    return 0;
}

int set_shader_ints_lua(lua_State* L){
    shader* current_shader = *(shader**)(luaL_checkudata(L, 1, "shader"));
    const char* name = luaL_checkstring(L, 2);
    int exp_num = luaL_checkinteger(L, 3);
    luaL_checktype(L, 4, LUA_TTABLE);
    int recvd_num = lua_objlen(L, 4);
    if(recvd_num!=exp_num){
        return luaL_error(L, "expected %d ints, but got %d", exp_num, recvd_num);
    }
    std::vector<GLint> pars(exp_num);
    for(size_t i=1; i<=exp_num; i++){
        lua_pushinteger(L, i);
        lua_gettable(L, 4);
        pars[i-1]=  luaL_checkinteger(L, -1);
        lua_pop(L, 1);
    }
    current_shader->set_ints(name ,pars);
    return 0;
}

int set_shader_float_lua(lua_State* L){
    shader* current_shader = *(shader**)(luaL_checkudata(L, 1, "shader"));
    const char* name = luaL_checkstring(L, 2);
    GLfloat value = luaL_checknumber(L, 3);
    current_shader->set_float(name, value);
    return 0;
}

int set_shader_floats_lua(lua_State* L){
    shader* current_shader = *(shader**)(luaL_checkudata(L, 1, "shader"));
    const char* name = luaL_checkstring(L, 2);
    int exp_num = luaL_checkinteger(L, 3);
    luaL_checktype(L, 4, LUA_TTABLE);
    int recvd_num = lua_objlen(L, 4);
    if(recvd_num!=exp_num){
        return luaL_error(L, "expected %d floats, but got %d", exp_num, recvd_num);
    }
    std::vector<GLfloat> pars(exp_num);
    for(size_t i=1; i<=exp_num; i++){
        lua_pushinteger(L, i);
        lua_gettable(L, 4);
        pars[i-1]=  luaL_checknumber(L, -1);
        lua_pop(L, 1);
    }
    current_shader->set_floats(name ,pars);
    return 0;
}

int set_shader_mat_lua(lua_State* L){
    int par_num = lua_gettop(L);
    shader* current_shader = *(shader**)(luaL_checkudata(L, 1, "shader"));
    const char* name = luaL_checkstring(L, 2);
    matrix* mat = *(matrix**)(luaL_checkudata(L, 3, "mat"));
    GLboolean transpose = false;
    if(par_num>=4){
        luaL_checktype(L, 4, LUA_TBOOLEAN);
        transpose = lua_toboolean(L, 4);
    }
    //log("set mat from lua\n")
    current_shader->setMat(name, *mat, transpose);
    return 0;
}

int set_shader_vec_lua(lua_State* L){
    int par_num = lua_gettop(L);
    shader* current_shader = *(shader**)(luaL_checkudata(L, 1, "shader"));
    const char* name = luaL_checkstring(L, 2);
    vector* vec = *(vector**)(luaL_checkudata(L, 3, "vec"));
    GLboolean transpose = false;
    current_shader->setVec(name, *vec);
    return 0;
}
