//产生几个opengl常用的矩阵
#include <tiny_glm_printer.hpp>
#include <glm/gtc/matrix_transform.hpp>
int main(void){
    /*注意,opengl是左手系(x向右,y向上时,z向内,因此z坐标-1.0最靠前,1.0最靠后),而通常我们习惯使用右手系(x向右,y向上时,z向外),因此该ortho函数的语义在z轴有所不同(实际上翻转了z轴)
     * x轴:0.0->-1.0;800->1.0;y轴:0.0->-1.0;600.0->1.0;z轴:-0.1->-1.0;-100->1.0
     * glm::ortho会自动为我们的近平面和远平面加上负号(即假定我们传入的平面的z坐标都是位于屏幕后方的坐标的相反数)
    */
    auto m1 = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    auto m2 = glm::perspective(glm::radians(54.0f), 16.0f/9.0f, 0.1f, 100.0f);


    std::cout<<"m1:"<<std::endl;
    std::cout<<mat2str<glm::mat4,4,4>(m1)<<std::endl;
    std::cout<<"m2:"<<std::endl;
    std::cout<<mat2str<glm::mat4,4,4>(m2)<<std::endl;
    //std::cout<<vec2str<glm::vec4,4>(v1*m1)<<std::endl;
}
