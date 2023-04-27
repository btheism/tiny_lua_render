#ifndef __TINY_MESH
#define __TINY_MESH

#include<tiny_common.hpp>
#include <cstdio>
typedef struct{
    GLuint index;
    GLint size;
    GLsizei stride;
    size_t offset;
}VAO_par;

class mesh{
public:
    GLenum type;
    GLsizei count;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    mesh(GLenum type, GLsizei count, const std::vector<float>& vbo_data, const std::vector<VAO_par>& vaos);
    mesh(GLenum type, GLsizei count, const std::vector<float>& vbo_data, const std::vector<VAO_par>& vaos, const std::vector<unsigned int>& ebo_data);
    ~mesh(void);
    void draw(void);
};

int new_mesh(lua_State* L);
int delete_mesh(lua_State* L);

int draw_mesh(lua_State* L);

void luaopen_tiny_meshlib(lua_State *L);
#endif

