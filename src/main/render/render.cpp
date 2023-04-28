//把纹理,着色器等让lua管理后,退出lua时,如果让lua自行terminate window,则lua会在terminate glfw后再释放资源,从而引发错误,因此glfw的init和terminate必须在lua外进行

#include <tiny_common.hpp>
#include <filesystem>
namespace fs = std::filesystem;

#include <tiny_window.hpp>
#include <tiny_shader.hpp>
#include <tiny_texture.hpp>
#include <tiny_mesh.hpp>
#include <tiny_matrix.hpp>

void load_resource(char *argv[]){
    static const std::vector<std::string> resources = {"script", "image", "sound", "model", "shader"};
    PHYSFS_init(argv[0]);
    for(const std::string & resource: resources){
        if(!PHYSFS_mount((fs::path(argv[1])/fs::path(resource+std::string(".zip"))).c_str(), resource.c_str(), 0)){
            log("do not find resource pack of %s.\n", resource.c_str());
        }
    }
    for(const std::string & resource: resources){
        if(!PHYSFS_mount((fs::path(argv[1])/fs::path(resource)).c_str(), resource.c_str(), 0)){
            log("do not find resource folder of %s\n", resource.c_str());
        }
    }

    if(!PHYSFS_setWriteDir((fs::path(argv[1])/fs::path("UserData/")).c_str())){
        log("do not find UserData directory to write.\n");
    }
    return;
}

//允许lua使用require加载physfs中的脚本,见https://stackoverflow.com/questions/59353633/using-lua-require-from-inside-a-zip-using-physfs , https://stackoverflow.com/questions/72992713/adding-searcher-in-package-searchers-with-lua-c-api
int lua_load_from_physfs(lua_State *L)
{
    const char *module_name = luaL_checkstring(L, 1);
    std::string module_full_name(module_name);
    module_full_name+= ".lua";
    file_buffer module_buffer = read_from_PHYSFS(module_full_name.c_str());
    int load_state =  luaL_loadbuffer(L, module_buffer.content, module_buffer.len, module_full_name.c_str());//luaL_loadbuffer会自己放置错误信息
    delete[] module_buffer.content;
    if(load_state==0){
        return 1;
    }else{
        return lua_error(L);
    }
}

void register_loader(lua_State *L, int lua_f(lua_State *)){
    lua_getglobal(L, "package");luaL_checktype(L, -1, LUA_TTABLE);
    lua_getfield(L, -1, "loaders");luaL_checktype(L, -1, LUA_TTABLE);//在jit5.1中叫loaders不叫searchers
    lua_pushinteger(L, lua_objlen(L, -1)+1);
    lua_pushcfunction(L, lua_f);
    lua_settable(L, -3);
    lua_pop(L, 2);//弹出package
    return;
}


int main(int argc, char * argv[])
{
    if(argc<2){
        fatal("please specify the location of resource file\n");
        exit(-1);
    }
    load_resource(argv);
    file_buffer conf_buffer = read_from_PHYSFS("script/conf.lua");
    lua_State* conf_L = luaL_newstate();
    int conf_exec_state = luaL_loadbuffer(conf_L, conf_buffer.content, conf_buffer.len, "script/conf.lua") || lua_pcall(conf_L, 0, LUA_MULTRET, 0);
    delete[] conf_buffer.content;
    if(conf_exec_state!=LUA_OK){
        std::string lua_error(lua_tostring(conf_L,-1));
        lua_close(conf_L);
        fatal("conf error: %s\n", lua_error.c_str());
        return -1;
    }else{
        lua_getglobal(conf_L, "title");
        const char* window_title = luaL_checkstring(conf_L, -1);lua_pop(conf_L, 1);
        lua_getglobal(conf_L, "width");
        int window_width = luaL_checkinteger(conf_L, -1);lua_pop(conf_L, 1);
        lua_getglobal(conf_L, "height");
        int window_height = luaL_checkinteger(conf_L, -1);lua_pop(conf_L, 1);
        lua_getglobal(conf_L, "icon");
        if(lua_type(conf_L, -1) == LUA_TSTRING){
            const char* window_icon_path = luaL_checkstring(conf_L, -1);
            init(window_title, window_icon_path, window_width, window_height);
        }else{
            init(window_title, window_width, window_height);
        }
        lua_pop(conf_L, 1);
        lua_close(conf_L);
        log("successfully read config\n");
    }

    file_buffer script_buffer = read_from_PHYSFS("script/main.lua");
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    register_loader(L, lua_load_from_physfs);//使得lua可以使用physfs加载脚本
    luaopen_tiny_windowlib(L);
    luaopen_tiny_shaderlib(L);
    luaopen_tiny_texturelib(L);
    luaopen_tiny_meshlib(L);
    luaopen_tiny_matrixlib(L);
    //luaopen_tiny_glmlib(L);

    int lua_exec_state = luaL_loadbuffer(conf_L, script_buffer.content, script_buffer.len, "script/main.lua") || lua_pcall(conf_L, 0, LUA_MULTRET, 0);;
    delete[] script_buffer.content;
    if(lua_exec_state!=LUA_OK){
        std::string lua_error(lua_tostring(L,-1));
        lua_close(L);
        fatal("lua error: %s\n", lua_error.c_str());
        clean();
        return -1;
    }else{
        lua_close(L);
        clean();
        log("successfully exit\n")
        return 0;
    }
}



