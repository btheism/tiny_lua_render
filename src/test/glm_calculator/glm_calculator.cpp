//使用glm计算矩阵,用来debug
#include <tiny_glm_printer.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <functional>
template<int par_num, void(*glm_func)(float*)>
void glm_wrap(void){
    int got_par=0;
    float pars[par_num];
    while(got_par<par_num){
        std::cin>>pars[got_par];
        got_par++;
    }
    glm_func(pars);
    return;
}

void glm_lookat(float* pars){
    glm::mat4 lookat = glm::lookAt(
        glm::vec3(pars[0], pars[1], pars[2]),
        glm::vec3(pars[3], pars[4], pars[5]),
        glm::vec3(pars[6], pars[7], pars[8]));
    std::cout<<mat2str<glm::mat4,4,4>(lookat)<<std::endl;
    return;
};

void perspective(float* pars){
    glm::mat4 persp = glm::perspective(glm::radians(pars[0]), pars[1], pars[2], pars[3]);
    std::cout<<mat2str<glm::mat4,4,4>(persp)<<std::endl;
    return;
};

void ortho(float* pars){
    glm::mat4 otrho = glm::ortho(pars[0], pars[1], pars[2], pars[3], pars[4], pars[5]);
    std::cout<<mat2str<glm::mat4,4,4>(otrho)<<std::endl;
    return;
};

std::unordered_map<const std::string, void(*)(void), std::hash<std::string>>glm_func_jump_table{
    {"lookat", glm_wrap<9, glm_lookat>},
    {"persp", glm_wrap<4, perspective>},
    {"ortho", glm_wrap<6, ortho>}
};


int main(void){
    /*注意,opengl是左手系(x向右,y向上时,z向内,因此z坐标-1.0最靠前,1.0最靠后),而通常我们习惯使用右手系(x向右,y向上时,z向外),因此该ortho函数的语义在z轴有所不同(实际上翻转了z轴)
     * x轴:0.0->-1.0;800->1.0;y轴:0.0->-1.0;600.0->1.0;z轴:-0.1->-1.0;-100->1.0
     * glm::ortho会自动为我们的近平面和远平面加上负号(即假定我们传入的平面的z坐标都是位于屏幕后方的坐标的相反数)
    */
    auto m1 = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    auto m2 = glm::perspective(glm::radians(54.0f), 16.0f/9.0f, 0.1f, 100.0f);
    auto e = glm::mat4(1.0f);
    auto m3 = glm::translate(e, glm::vec3(0.0f, 0.0f, -3.0f));
    auto m4 = glm::rotate(e, glm::radians(-55.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    auto m5 = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0f, 0.5f, 0.0f),glm::vec3(1.0f, 1.0f, 0.0f));
    while(true){
        std::string name;
        std::cin>>name;
        if(glm_func_jump_table.count(name)){
            (*glm_func_jump_table.at(name))();
        }
        else{
            std::cout<<"unsupported function name\n"<<std::endl;
        }
    }

}
