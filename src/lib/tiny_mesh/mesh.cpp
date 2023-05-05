#include<tiny_mesh.hpp>

mesh::mesh(GLenum type, GLsizei count, const std::vector<float>& vbo_data, const std::vector<VAO_par>& vas):type(type), count(count), have_ebo(false){
    GLuint VBO;
    GL_CHECK(glCreateVertexArrays(1, &VAO));
    GL_CHECK(glCreateBuffers(1, &VBO));//不能使用glGenBuffers!!
    GL_CHECK(glNamedBufferData(VBO, vbo_data.size()*sizeof(float), &(vbo_data[0]), GL_STATIC_DRAW));

    for(const VAO_par & va:vas){
        //log("get vao (%d, %d, %d, %zu)\n", va.index, va.size, va.stride, va.offset)
        //启用顶点
        GL_CHECK(glEnableVertexArrayAttrib(VAO, va.index));
        //生成一个视图
        GL_CHECK(glVertexArrayVertexBuffer(VAO, va.index, VBO, va.offset*sizeof(float), va.stride*sizeof(float)));
        //绑定视图到顶点
        GL_CHECK(glVertexArrayAttribBinding(VAO, va.index, va.index));
        //设置顶点属性,不明白这里为什么又要定义偏移量
        GL_CHECK(glVertexArrayAttribFormat(VAO, va.index, va.size, GL_FLOAT, GL_FALSE, 0));
    }
    //GL_CHECK(glBindVertexArray(0));
    GL_CHECK(glDeleteBuffers(1, &VBO));
};

mesh::mesh(GLenum type, GLsizei count, const std::vector<float> &vbo_data, const std::vector<VAO_par>& vas, const std::vector<unsigned int>& ebo_data):mesh(type, count, vbo_data, vas){
    have_ebo=true;
    GLuint EBO;
    GL_CHECK(glCreateBuffers(1, &EBO));
    GL_CHECK(glNamedBufferData(EBO, ebo_data.size()*sizeof(unsigned int), &(ebo_data[0]), GL_STATIC_DRAW));
    glVertexArrayElementBuffer(VAO, EBO);
    //opengl的一般规则:删除一个子对象时先解绑其父对象,一般而言,opengl删除子对象时如果其与当前父对象绑定,则自动解绑,并删除子对象;如果其与其他父对象绑定,则只减小引用计数,framebuffer和VAO都是如此。但着色器是个例外,它不会触发对当前激活的program的自动detach;此外,删除其他对象时会自动把绑定置0,但program不会(似乎都是由于没有默认的program)
    GL_CHECK(glDeleteBuffers(1, &EBO));
};

mesh::~mesh(void){
    glDeleteVertexArrays(1, &VAO);
    return;
};

void mesh::draw(void){
    glBindVertexArray(VAO);
    if(have_ebo){
        //这里规定EBO中的数据类型
        glDrawElements(type, count, GL_UNSIGNED_INT, 0);
    }else{
        glDrawArrays(type, 0, count);
    }
    glBindVertexArray(0);
};

//该函数应配合new_mesh使用,设置栈顶上的元素的元表为mesh
void create_mesh_table(lua_State* L){
    //该函数可以避免元表被重复注册,并把元表放在栈顶
    if(luaL_newmetatable(L, "mesh")){
        static const luaL_Reg functions[] =
        {
            {"__gc", delete_mesh},
            {"draw", draw_mesh},
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

int new_mesh(lua_State* L)
{
    bool has_ebo = lua_gettop(L)==5;
    const char* primitive = luaL_checkstring(L, 1);
    if(!str2gl_primitive_table.count(primitive)){
        fatal("opengl primitive %s is invaild\n", primitive);
    }
    GLsizei count = luaL_checkinteger(L, 2);
    luaL_checktype(L, 3, LUA_TTABLE);
    //log("parse vbo\n")
    size_t vbo_size =  lua_objlen(L, 3);
    std::vector<float> vbo_cache(vbo_size);
    for(int i=1; i<=vbo_size; i++){
        lua_pushinteger(L, i);
        lua_gettable(L, 3);
        vbo_cache[i-1]=luaL_checknumber(L, -1);
        lua_pop(L, 1);
    }
    luaL_checktype(L, 4, LUA_TTABLE);
    size_t vao_size =  lua_objlen(L, 4);
    //log("parse vao, size is %zu\n", vao_size)
    std::vector<VAO_par> vao_cache(vao_size);
    for(int i=1; i<=vao_size; i++){
        //log("parse vao %d\n", i);
        lua_pushinteger(L, i);
        lua_gettable(L, 4);

        lua_pushinteger(L, 1);lua_gettable(L, -2);
        GLuint va_index = luaL_checkinteger(L, -1);lua_pop(L, 1);
        lua_pushinteger(L, 2);lua_gettable(L, -2);
        GLint va_size = luaL_checkinteger(L, -1);lua_pop(L, 1);
        lua_pushinteger(L, 3);lua_gettable(L, -2);
        GLsizei va_stride = luaL_checkinteger(L, -1);lua_pop(L, 1);
        lua_pushinteger(L, 4);lua_gettable(L, -2);
        size_t va_offset = luaL_checkinteger(L, -1);lua_pop(L, 1);
        vao_cache[i-1]={va_index, va_size, va_stride, va_offset};

        lua_pop(L, 1);
    }
    void* mesh_pp = lua_newuserdata(L, sizeof(void*));
    //如果有EBO参数
    if(has_ebo){
        luaL_checktype(L, 5, LUA_TTABLE);
        size_t ebo_size =  lua_objlen(L, 5);
        //log("parse ebo, size is %zu\n", ebo_size)
        std::vector<unsigned int> ebo_cache(ebo_size);
        for(int i=1; i<=ebo_size; i++){
            lua_pushinteger(L, i);
            lua_gettable(L, 5);
            ebo_cache[i-1]=luaL_checknumber(L, -1);
            lua_pop(L, 1);
        }
        //创建userdata并把原表填充到栈顶
        *(mesh**)mesh_pp = new mesh(str2gl_primitive_table.at(primitive), count, vbo_cache, vao_cache, ebo_cache);
    }else{
        //创建userdata并把原表填充到栈顶
        *(mesh**)mesh_pp = new mesh(str2gl_primitive_table.at(primitive), count, vbo_cache, vao_cache);
    }
    create_mesh_table(L);
    return 1;
}

int delete_mesh(lua_State* L){
    delete *(mesh**)(luaL_checkudata(L, 1, "mesh"));
    return 0;
}

int draw_mesh(lua_State* L){
    mesh* current_mesh =  *(mesh**)(luaL_checkudata(L, 1, "mesh"));
    current_mesh->draw();
    return 0;
}


