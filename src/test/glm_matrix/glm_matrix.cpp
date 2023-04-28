//与example/matrix/对照
#include <tiny_glm_printer.hpp>

int main(void){
    glm::mat4 m1={2,2,2,2,3,13,17,19,3,4,7,8,3,5,7,3};
    glm::vec4 v1={2,3,5,7};
    //行优先打印与列优先打印
    std::cout<<"m1 row major:"<<std::endl;
    std::cout<<mat2str<glm::mat4,4,4>(m1)<<std::endl;
    std::cout<<"m1 col major:"<<std::endl;
    std::cout<<mat2str_col_major<glm::mat4,4,4>(m1)<<std::endl;
    std::cout<<"m1 inverse:"<<std::endl;
    std::cout<<mat2str<glm::mat4,4,4>(inverse(m1))<<std::endl;
    //左乘和右乘
    std::cout<<"v1:"<<std::endl;
    std::cout<<vec2str<glm::vec4,4>(v1)<<std::endl;
    std::cout<<"m1 * v1:"<<std::endl;
    std::cout<<vec2str<glm::vec4,4>(m1*v1)<<std::endl;
    std::cout<<"v1 * m1:"<<std::endl;
    std::cout<<vec2str<glm::vec4,4>(v1*m1)<<std::endl;
}
