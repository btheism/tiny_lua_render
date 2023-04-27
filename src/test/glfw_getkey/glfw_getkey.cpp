//该代码旨在测试glfw的回调函数如何处理事件,同类事件会积累吗?
#include<cstdio>
#include<tiny_common.hpp>
#include<unistd.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    log("framebuffer size become %d, %d\n", width, height);
}

int main(int argc, char * argv[])
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if(!glfwInit()){
        fatal("Failed to initialize glfw\n");
    }
    int window_width = 1920;
    int window_height = 1080;
    const char* window_title = "test glfw callback";
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        fatal("Failed to create GLFW window\n");
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    while(true){
        sleep(10);
        glfwPollEvents();
        log("q gets %s\n", key_action2str_table.at(glfwGetKey(window, str2key_table.at("q"))).c_str());
        log("w gets %s\n", key_action2str_table.at(glfwGetKey(window, str2key_table.at("w"))).c_str());
    }


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
