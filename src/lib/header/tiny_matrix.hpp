//由于opengl使用列主序,因此该矩阵也使用列主序存储
#include <tiny_common.hpp>
class matrix{
public:
    size_t row;
    size_t col;
    float* content;
    matrix(size_t row, size_t col);
    matrix(size_t row, size_t col, float diag);
    std::string tostr(void);
    ~matrix(void){
        delete []content;
    }
};

void luaopen_tiny_matrixlib(lua_State *L);

int new_matrix(lua_State* L);
int matrix_to_string(lua_State* L);
int delete_matrix(lua_State* L);

