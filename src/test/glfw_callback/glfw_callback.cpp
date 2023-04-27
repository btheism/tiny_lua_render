//该代码旨在测试glfw的回调函数如何处理事件,同类事件会积累吗?
#include<tiny_common.hpp>
#include<unistd.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    log("framebuffer size become %d, %d\n", width, height);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    log("get key %s be %s\n", key2str_table.at(key).c_str(), key_action2str_table.at(action).c_str());
}

void character_callback(GLFWwindow* window, unsigned int codepoint){
    log("get char %d\n", codepoint);
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
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCharCallback(window, character_callback);

    while(true){
        sleep(1);
        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}




