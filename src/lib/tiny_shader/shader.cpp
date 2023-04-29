#include <tiny_shader.hpp>
#include <tiny_linear.hpp>
#include <unordered_map>

shader* shader_from_file(const char* vertex_shader_path, const char* fragment_shader_path){
        file_buffer vertex_shader_code = read_from_PHYSFS(vertex_shader_path);
        file_buffer fragment_shader_code = read_from_PHYSFS(fragment_shader_path);
        shader* new_shader = new shader(vertex_shader_code.content, fragment_shader_code.content);
        delete vertex_shader_code.content;
        delete fragment_shader_code.content;
        return new_shader;
};

//vertex_shader_code和fragment_shader_code由调用者自行管理
shader::shader(const char* vertex_shader_code, const char* fragment_shader_code){
        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertex_shader_code, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragment_shader_code, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        glDeleteShader(vertex);
        glDeleteShader(fragment);
}

void shader::checkCompileErrors(unsigned int shader, const char* type)
{
    int success;
    char infoLog[1024];
    if (std::string(type) != "PROGRAM"){
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            fatal("ERROR::SHADER_COMPILATION_ERROR of type:%s\n%s\n", type, infoLog)
        }
    }else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success){
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            fatal("ERROR::PROGRAM_LINKING_ERROR of type:%s\n%s\n", type, infoLog)
        }
    }
}

static const std::unordered_map<size_t, void(**)(GLint, GLsizei, const GLfloat*)>setvec_func_table{
    {1, &glUniform1fv},
    {2, &glUniform2fv},
    {3, &glUniform3fv},
    {4, &glUniform4fv}
};
//使用col<<8+row计算每个矩阵对应的函数的key值,从而把不同维度的函数映射到不同的函数
//这些函数指针的值会在运行时改变,因此需要存储指针的地址,不能直接存储指针(作为全局变量时未初始化)的值
static const std::unordered_map<size_t, void(**)(GLint, GLsizei, GLboolean, const GLfloat*)>setmat_func_table{
    {(2<<8)+2, &glUniformMatrix2fv},
    {(3<<8)+3, &glUniformMatrix3fv},
    {(4<<8)+4, &glUniformMatrix4fv},
    {(2<<8)+3, &glUniformMatrix2x3fv},
    {(3<<8)+2, &glUniformMatrix3x2fv},
    {(2<<8)+4, &glUniformMatrix2x4fv},
    {(4<<8)+2, &glUniformMatrix4x2fv},
    {(3<<8)+4, &glUniformMatrix3x4fv},
    {(4<<8)+3, &glUniformMatrix4x3fv}
};

void shader::setMat(const char* name, const matrix &mat, GLboolean transpose) const{
    int uni_loc = glGetUniformLocation(ID, name);
    if(uni_loc==-1){
        fatal("cannot find uniform %s in shader\n", name)
    }
    if(!setmat_func_table.count((mat.col<<8)+mat.row)){
        fatal("do not support set matrix of size (%zu, %zu) in shader\n", mat.col, mat.row)
    }
    (**setmat_func_table.at((mat.col<<8)+mat.row))(uni_loc, 1, transpose, mat.content);
}

void shader::setVec(const char* name, const matrix &mat) const{
    int uni_loc = glGetUniformLocation(ID, name);
    if(uni_loc==-1){
        fatal("cannot find uniform %s in shader\n", name)
    }
    if(std::min(mat.col,mat.row)!=1||!setvec_func_table.count(std::max(mat.col,mat.row))){
        fatal("do not support set vector of size (%zu, %zu) in shader\n", mat.col, mat.row)
    }
    (**setvec_func_table.at(std::max(mat.col,mat.row)))(uni_loc, 1, mat.content);
}

//该函数应配合new_shader使用,设置栈顶上的元素的元表为shader
void create_shader_table(lua_State* L){
    //该函数可以避免元表被重复注册,并把元表放在栈顶
    //元表位于registry表中,无须担心名称冲突
    if(luaL_newmetatable(L, "shader")){
        static const luaL_Reg functions[] =
        {
            {"__gc", delete_shader},
            {"use", use_shader},
            {"set_int", set_shader_int},
            {"set_float", set_shader_float},
            {"set_mat", set_shader_mat},
            {"set_vec", set_shader_vec},
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

int new_shader(lua_State* L)
{
    const char* vertex_shader_code = luaL_checkstring(L, 1);
    const char* fragment_shader_code = luaL_checkstring(L, 2);
    //创建userdata并把原表填充到栈顶
    void* shader_pp = lua_newuserdata(L, sizeof(void*));
    *(shader**)shader_pp = new shader(vertex_shader_code, fragment_shader_code);
    create_shader_table(L);
    return 1;
}

int delete_shader(lua_State* L){
    delete *(shader**)(luaL_checkudata(L, 1, "shader"));
    return 0;
}

int use_shader(lua_State* L){
    shader* current_shader = *(shader**)(luaL_checkudata(L, 1, "shader"));
    current_shader->use();
    return 0;
}

int set_shader_int(lua_State* L){
    shader* current_shader = *(shader**)(luaL_checkudata(L, 1, "shader"));
    const char* name = luaL_checkstring(L, 2);
    GLint value = luaL_checkinteger(L, 3);
    current_shader->setInt(name, value);
    return 0;
}

int set_shader_float(lua_State* L){
    shader* current_shader = *(shader**)(luaL_checkudata(L, 1, "shader"));
    const char* name = luaL_checkstring(L, 2);
    GLfloat value = luaL_checknumber(L, 3);
    current_shader->setFloat(name, value);
    return 0;
}

int set_shader_mat(lua_State* L){
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

int set_shader_vec(lua_State* L){
    int par_num = lua_gettop(L);
    shader* current_shader = *(shader**)(luaL_checkudata(L, 1, "shader"));
    const char* name = luaL_checkstring(L, 2);
    matrix* mat = *(matrix**)(luaL_checkudata(L, 1, "mat"));
    GLboolean transpose = false;
    current_shader->setVec(name, *mat);
    return 0;
}
