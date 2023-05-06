#ifndef __TINY_SHADER
#define __TINY_SHADER
#include <tiny_common.hpp>
#include <tiny_linear.hpp>

//完全由自己定义的着色器
class shader{
public:
    //传入按照physfs表示的路径
    GLuint ID;
    shader(const std::vector<std::pair<const char*, GLenum>> & codes);
    shader(const shader & mirror) = delete;
    shader(shader && mirror){
        ID = mirror.ID;
        mirror.ID=0;
    }
    ~shader(void){
        GL_CHECK(glDeleteProgram(ID));
    };
    void use(){
        GL_CHECK(glUseProgram(ID));
    }
    GLint inline get_uniform_loc(const char* name) const{
        GLint uni_loc = glGetUniformLocation(ID, name);
        if(uni_loc==-1){
            fatal("cannot find uniform %s in shader\n", name)
        }
        return uni_loc;
    }

    void set_int(const char* name, GLint value) const{
        //使用了DSA函数以避免binding
        GL_CHECK(glProgramUniform1i(ID, get_uniform_loc(name), value));
    }
    void set_float(const char* name, GLfloat value) const{
        GL_CHECK(glProgramUniform1f(ID, get_uniform_loc(name), value));
    }
    void set_ints(const char* name, const std::vector<GLint>& values) const{
        GL_CHECK(glProgramUniform1iv(ID, get_uniform_loc(name), values.size(), &(values[0])));
    };
    void set_floats(const char* name, const std::vector<GLfloat>& values) const{
        GL_CHECK(glProgramUniform1fv(ID, get_uniform_loc(name), values.size(), &(values[0])));
    };
    void setMat(const char* name, const matrix &mat, GLboolean transpose) const;
    void setVec(const char* name, const vector &vec) const;
};

//shader* shader_from_file(const char* vertex_shader_path, const char* fragment_shader_path);

int new_shader_lua(lua_State* L);
int delete_shader_lua(lua_State* L);

int use_shader_lua(lua_State* L);
int set_shader_int_lua(lua_State* L);
int set_shader_float_lua(lua_State* L);
int set_shader_ints_lua(lua_State* L);
int set_shader_floats_lua(lua_State* L);
int set_shader_mat_lua(lua_State* L);
int set_shader_vec_lua(lua_State* L);

void luaopen_tiny_shaderlib(lua_State *L);

#endif
