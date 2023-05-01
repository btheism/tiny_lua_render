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

class vector{
public:
    size_t size;
    float* content;
    vector(const vector & mirror);
    vector(vector && mirror);
    vector(size_t len);
    vector(size_t len, const std::vector<float>& list);
    std::string tostr(void);
    ~vector(void){
        //content可能被移动到了别的矩阵
        if(content!=nullptr){
            delete []content;
        }
    }
};

void luaopen_tiny_linearlib(lua_State *L);
void create_vec_table(lua_State* L);
void create_mat_table(lua_State* L);

int mat_to_string_lua(lua_State* L);
int delete_mat_lua(lua_State* L);
int get_mat_col_lua(lua_State* L);
int get_mat_row_lua(lua_State* L);

int id_mat_lua(lua_State* L);
int col_mat_lua(lua_State* L);
int row_mat_lua(lua_State* L);
int ortho_mat_lua(lua_State* L);
int perspective_mat_lua(lua_State* L);
int move_mat_lua(lua_State* L);
int rotate_mat_lua(lua_State* L);
int lookat_mat_lua(lua_State* L);
int scale_mat_lua(lua_State* L);

int add_mat_lua(lua_State* L);
int sub_mat_lua(lua_State* L);
int mul_mat_lua(lua_State* L);
int inverse_mat_lua(lua_State* L);
int transpose_mat_lua(lua_State* L);
int index_mat_lua(lua_State* L);
int modify_mat_lua(lua_State* L);

vector mul_vec_mat(const vector& vec, const matrix& mat);
vector mul_mat_vec(const matrix& mat, const vector& vec);
int new_vec(lua_State* L);
int vec_to_string_lua(lua_State* L);
int delete_vec_lua(lua_State* L);
int get_vec_size_lua(lua_State* L);

int add_vec_lua(lua_State* L);
int sub_vec_lua(lua_State* L);
int mul_vec_lua(lua_State* L);
int dot_vec_lua(lua_State* L);
int neg_vec_lua(lua_State* L);
int normalize_vec_lua(lua_State* L);
int cross_vec_lua(lua_State* L);
int index_vec_lua(lua_State* L);
int modify_vec_lua(lua_State* L);

vector normalize_vec(const vector& vec);
vector neg_vec(const vector& vec);
float vec_len(const vector& vec);
vector cross_vec_vec(const vector& lvec, const vector& rvec);
vector sub_vec_vec(const vector& lvec, const vector& rvec);

//这是一些关于角度的函数,其实也可以直接调用lua的math库中的函数
int degree_to_radians(lua_State* L);
int radians_to_degree(lua_State* L);


#endif
