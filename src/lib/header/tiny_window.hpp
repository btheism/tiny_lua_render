#ifndef __TINY_WINDOW
#define __TINY_WINDOW

#include <tiny_common.hpp>
#include <tiny_image.hpp>
void init(const char* window_title, int window_width, int window_height);
void init(const char* window_title, const char* window_icon_path, int window_width, int window_height);
void clean(void);

int key_pressed(lua_State *L);
int set_clear_color(lua_State *L);
int clear(lua_State *L);
int fresh(lua_State *L);
int should_close(lua_State *L);
int set_close_bit(lua_State *L);
int enable_depth(lua_State *L);
int disable_depth(lua_State *L);
int enable_stencial(lua_State *L);
int disable_stencial(lua_State *L);
int reset_framebuffer_lua(lua_State *L);
//int check_framebuffer_status(lua_State *L);

void luaopen_tiny_windowlib(lua_State *L);

#endif
