#ifndef __TINY_LINEAR
#define __TINY_LINEAR

//由于opengl使用列主序,因此该矩阵也使用列主序存储
#include <tiny_common.hpp>
class matrix{
public:
    size_t row;
    size_t col;
    float* content;
    matrix(const matrix & mirror);
    matrix(matrix && mirror);
    matrix(size_t row, size_t col);
    //row_major=true,假定输入数据为行优先;row_major=false,假定输入数据为列优先
    matrix(size_t col, size_t row, const std::vector<float>& list, bool row_major);
    std::string tostr_col_major(void);
    std::string tostr(void);
    ~matrix(void){
        //content可能被移动到了别的矩阵
        if(content!=nullptr){
            delete []content;
        }
    }
};

void luaopen_tiny_linearlib(lua_State *L);

//不带lua后缀的lua函数是对C++成员函数的封装
int new_matrix(lua_State* L);
int matrix_to_string(lua_State* L);
int delete_matrix(lua_State* L);
int get_matrix_col(lua_State* L);
int get_matrix_row(lua_State* L);

//带lua后缀的lua函数是对C++非成员函数的封装
int add_matrix_lua(lua_State* L);
int sub_matrix_lua(lua_State* L);
int mul_matrix_lua(lua_State* L);
int inverse_matrix_lua(lua_State* L);
int transpose_matrix_lua(lua_State* L);

//这是一些关于角度的函数,其实也可以直接调用lua的math库中的函数
int degree_to_radians(lua_State* L);
int radians_to_degree(lua_State* L);


#endif
