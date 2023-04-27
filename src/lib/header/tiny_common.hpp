#ifndef __TINY_COMMON
#define __TINY_COMMON

//见https://stackoverflow.com/questions/44345811/glad-h-giving-error-that-opengl-header-is-included
//需要先包含glad后包含glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <luajit-2.1/lua.hpp>
#include <physfs.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <stdexcept>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//tiny_common中的log的宏定义会使得先包含tiny_common再包含glm时clangd15.0.7-1报错(bug?),因此先包含后定义宏

#ifndef LOG_TO_FILE
#define fatal(args...) {      \
fprintf(stderr, args);        \
throw std::runtime_error("fatal error, see log");  \
}

#define log(args...) fprintf(stdout, args);
#else
#define fatal(args...) {fprintf(get_log_file(), args); exit(EXIT_FAILURE);}
#define log(args...) fprintf(get_log_file(), args);
#endif

void set_log_file(FILE* file);
FILE* get_log_file(void);
void dump_lua_stack(lua_State *L);
typedef struct {
    char * content;
    size_t len;
}file_buffer;

file_buffer read_from_PHYSFS(const char* file_name);
//与上个函数相比在末尾补充了'\0', 从而正确结束字符串,返回的长度比字符串文件大1
file_buffer read_str_from_PHYSFS(const char* file_name);


//变量必须被声明为extern,否则会被初始化为空变量!!!
extern const std::unordered_map <int, const std::string>key2str_table;
extern const std::unordered_map <const std::string, int, std::hash<std::string>>str2key_table;
extern const std::unordered_map <int, const std::string>key_action2str_table;
extern const std::unordered_map <const std::string, int, std::hash<std::string>>str2key_action_table;

extern const std::unordered_map<const std::string, GLint, std::hash<std::string>> str2texture_wrap_table;
extern const std::unordered_map<GLint, const std::string> texture_wrap2str_table;
extern const std::unordered_map<const std::string, GLint, std::hash<std::string>> str2texture_filter_table;
extern const std::unordered_map<GLint, const std::string> texture_filter2str_table;

extern const std::unordered_map<const std::string, GLint, std::hash<std::string>> str2image_mode_table;
extern const std::unordered_map<GLint, const std::string> image_mode2str_table;
extern const std::unordered_map<GLint, int> image_mode_channel_table;

extern const std::unordered_map<const std::string, GLenum, std::hash<std::string>> str2gl_primitive_table;
#endif
