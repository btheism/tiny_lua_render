#include<tiny_texture.hpp>
#include<tiny_image.hpp>

texture_2d::texture_2d(const char* image_path, GLint image_mode, GLint texture_mode, GLint swrap, int twrap, GLint min_filter, GLint max_filter){
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, swrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, twrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter);
    //image_8bit会自动释放内存
    image_8bit image(image_path, true);

    if(image.channel!=image_mode_channel_table.at(image_mode)){
        fatal("image has %d channels, but image mode %s expects %d channels\n",
              image.channel,
              image_mode2str_table.at(image_mode).c_str(),
              image_mode_channel_table.at(image_mode)
        );
    };
    glTexImage2D(GL_TEXTURE_2D, 0, texture_mode, image.width, image.height, 0, image_mode, GL_UNSIGNED_BYTE, image.content);
    glGenerateMipmap(GL_TEXTURE_2D);
}

//该函数应配合new_texture_2d使用,设置栈顶上的元素的元表为shader
void create_texture_2d_table(lua_State* L){
    //该函数可以避免元表被重复注册,并把元表放在栈顶
    if(luaL_newmetatable(L, "texture_2d")){
        static const luaL_Reg functions[] =
        {
            {"__gc", delete_texture_2d},
            {"active", active_texture_2d},
            {nullptr, nullptr}
        };
        //这个函数把上面的函数填入表
        luaL_setfuncs(L, functions, 0);
        //复制元表自身
        lua_pushvalue(L, -1);
        //设置元表的"__index"指向自身
        lua_setfield(L, -2, "__index");//该函数把栈的[-1]元素的"__index"设为栈顶元素,这个函数会弹出栈顶元素,因此在设置设置元表的"__index"指向自身前需要复制元表,见https://stackoverflow.com/questions/35589708/how-to-register-c-class-constructor-to-lua-userdata-and-use-it-by-default
    }
    //把栈的-2处的元素的元表设为栈顶元素(即shader的元表),这也会自动弹出元表,现在栈顶元素是设置了元表的userdata
    lua_setmetatable(L, -2);
    return;
}

int new_texture_2d(lua_State* L)
{
    const char* image_path = luaL_checkstring(L, 1);

    const char* image_mode = luaL_checkstring(L, 2);
    if(!str2image_mode_table.count(image_mode)){
        return luaL_error(L, "image mode %s is invalid", image_mode);
    }

    const char* texture_mode = luaL_checkstring(L, 3);
    if(!str2image_mode_table.count(texture_mode)){
        return luaL_error(L, "texture mode %s is invalid", texture_mode);
    }

    const char* swrap = luaL_checkstring(L, 4);
    if(!str2texture_wrap_table.count(swrap)){
        return luaL_error(L, "texture wrap mode %s is invalid", swrap);
    }

    const char* twrap = luaL_checkstring(L, 5);
    if(!str2texture_wrap_table.count(twrap)){
        return luaL_error(L, "texture wrap mode %s is invalid", twrap);
    }

    const char* min_filter = luaL_checkstring(L, 6);
    if(!str2texture_filter_table.count(min_filter)){
        return luaL_error(L, "texture filter mode %s is invalid", min_filter);
    }

    const char* max_filter = luaL_checkstring(L, 7);
    if(!str2texture_filter_table.count(max_filter)){
        return luaL_error(L, "texture filter %s is invalid", max_filter);
    }

    //创建userdata并把原表填充到栈顶
    void* texture_2d_pp = lua_newuserdata(L, sizeof(void*));
    *(texture_2d**)texture_2d_pp = new texture_2d(image_path, str2image_mode_table.at(image_mode), str2image_mode_table.at(texture_mode), str2texture_wrap_table.at(swrap), str2texture_wrap_table.at(twrap), str2texture_filter_table.at(min_filter), str2texture_filter_table.at(max_filter));
    create_texture_2d_table(L);
    return 1;
}

int delete_texture_2d(lua_State* L){
    delete *(texture_2d**)(luaL_checkudata(L, 1, "texture_2d"));
    return 0;
}

int active_texture_2d(lua_State* L){
    texture_2d* current_texture_2d =  *(texture_2d**)(luaL_checkudata(L, 1, "texture_2d"));
    int slot = luaL_checkinteger(L, 2);
    current_texture_2d->active(slot);
    return 0;
}
