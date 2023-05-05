#include <cstdio>
#include<tiny_window.hpp>

//全局变量,指向当前窗口
static GLFWwindow* window = NULL;

int key_pressed(lua_State *L){
    const char* key_name = luaL_checkstring(L, 1);
    if(!str2key_table.count(key_name)){
        luaL_error(L, "key name %s is illegal", key_name);
        return 1;
    }
    lua_pushboolean(L, glfwGetKey(window, str2key_table.at(key_name)) == GLFW_PRESS);
    return 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
/*
原计划把回调函数包装为C,感觉没什么用,放弃
int set_keyboard_callback(lua_State *L){
    lua_keyboard_callback_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    if(lua_keyboard_callback_ref==LUA_REFNIL){
        fatal("set a nil value as keyboard callback!\n");
    }
    glfwSetKeyCallback(window, key_callback);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
}
*/

void init(const char* window_title, int window_width, int window_height){
    //版本高一些无所谓
    //例如只有4.0及以上才支持对立方体贴图glGenerateMipmap
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if(!glfwInit()){
        fatal("Failed to initialize glfw\n");
        return;
    }

    window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        fatal("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        fatal("Failed to initialize GLAD\n");
        return;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glEnable(GL_DEPTH_TEST);

    return;
};

void init(const char* window_title, const char* window_icon_path, int window_width, int window_height){
    init(window_title, window_width, window_height);
    //根据注释"This function sets the icon of the specified window. If passed an array of candidate images, those of or closest to the sizes desired by the system are selected",似乎可以传入一组图标
    GLFWimage images[1];
    image_8bit icon_image(window_icon_path, false, 4); //必须加载为rgba,故传入4
    images[0].pixels = icon_image.content;
    images[0].width = icon_image.width;
    images[0].height = icon_image.height;
    glfwSetWindowIcon(window, 1, images);
    return;
};


void clean(void){
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
}

int set_clear_color(lua_State *L){
    float red = luaL_checknumber(L, 1);
    float green = luaL_checknumber(L, 2);
    float blue = luaL_checknumber(L, 3);
    float alpha = luaL_checknumber(L, 4);
    glClearColor(red, green, blue, alpha);
    return 0;
}

int clear(lua_State *L){
    //注意清除缓冲位
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return 0;
}

//用户需要自行调用clear
int fresh(lua_State *L){
    glfwSwapBuffers(window);
    glfwPollEvents();
    return 0;
}

int should_close(lua_State *L){
    lua_pushboolean(L, glfwWindowShouldClose(window));
    return 1;
}

int set_close_bit(lua_State *L){
    glfwSetWindowShouldClose(window, true);
    return 0;
}

int enable_depth(lua_State *L){
    GL_CHECK(glEnable(GL_DEPTH_TEST));
    return 0;
}

int disable_depth(lua_State *L){
    GL_CHECK(glDisable(GL_DEPTH_TEST));
    return 0;
}

int enable_stencial(lua_State *L){
    GL_CHECK(glEnable(GL_STENCIL_TEST));
    return 0;
}

int disable_stencial(lua_State *L){
    GL_CHECK(glDisable(GL_STENCIL_TEST));
    return 0;
}

int reset_framebuffer_lua(lua_State* L){
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_CHECK(glViewport(0, 0, width, height));
    return 0;
}

/*
int check_framebuffer_status(lua_State *L){
    GLuint fb_code;
    GL_CHECK(fb_code = glCheckFramebufferStatus(GL_FRAMEBUFFER))
    if(fb_code != GL_FRAMEBUFFER_COMPLETE)
    log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!")
    return 0;
};
*/
