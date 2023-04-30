#include<tiny_linear.hpp>
int main(void){
    //测试矩阵的move语义
    matrix m1(1,2);
    matrix m2(std::move(m1));
    return 0;
}
