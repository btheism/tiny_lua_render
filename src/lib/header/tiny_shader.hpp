#ifndef __TINY_SHADER
#define __TINY_SHADER
#include <tiny_common.hpp>

//完全由自己定义的着色器
class shader{
public:
    //传入按照physfs表示的路径
    GLuint ID;
    shader(const char* vertex_shader_code, const char* fragment_shader_code);
    ~shader(void){
        glDeleteProgram(ID);
    };
    void use(){
        glUseProgram(ID);
    }
    void setInt(const char* name, GLint value) const{
        int uni_loc = glGetUniformLocation(ID, name);
        if(uni_loc==-1){
            fatal("cannot find uniform %s in shader\n", name)
        }
        glUniform1i(uni_loc, value);
    }
    void setFloat(const char* name, GLfloat value) const{
        int uni_loc = glGetUniformLocation(ID, name);
        if(uni_loc==-1){
            fatal("cannot find uniform %s in shader\n", name)
        }
        glUniform1f(uni_loc, value);
    }
    /*
    void setMat4(const char* name, const glm::mat4& mat, GLboolean transpose) const{
        int uni_loc = glGetUniformLocation(ID, name);
        if(uni_loc==-1){
            fatal("cannot find uniform %s in shader\n", name)
        }
        glUniformMatrix4fv(uni_loc, 1, transpose, &mat[0][0]);
    }
    */

    private:
    static void checkCompileErrors(unsigned int shader, const char* type);
};

shader* shader_from_file(const char* vertex_shader_path, const char* fragment_shader_path);

int new_shader(lua_State* L);
int delete_shader(lua_State* L);

int use_shader(lua_State* L);
int set_shader_int(lua_State* L);
int set_shader_float(lua_State* L);

void luaopen_tiny_shaderlib(lua_State *L);

#endif
