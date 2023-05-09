#include<tiny_texture.hpp>

texture_cube::texture_cube(const char* image_path[], GLint image_mode, GLint texture_mode, GLint min_filter, GLint max_filter){
    GL_CHECK(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &ID));
    //立方体贴图的wrap模式仅影响接缝处的点,没有自定义的价值
    GL_CHECK(glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTextureParameteri(ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, min_filter));
    GL_CHECK(glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, max_filter));
    int img_width, img_height;

    //image_8bit会自动释放内存
    for(int img_serial=0; img_serial<6; img_serial++){
        image_8bit image(image_path[img_serial], true);
        if(image.channel!=image_mode_channel_table.at(image_mode)){
            fatal("image has %d channels, but image mode %s expects %d channels\n",
                image.channel,
                image_mode2str_table.at(image_mode).c_str(),
                image_mode_channel_table.at(image_mode)
            );
        };
        if(img_serial==0){
            //根据图片的大小初始化纹理内存
            img_width=image.width;
            img_height=image.height;
            GL_CHECK(glTextureStorage2D(ID, 1 + std::floor(std::log2(std::max(img_width, img_height))), texture_mode, image.width, image.height))
        }
        if(image.height!=img_height||image.width!=img_width){
            fatal("create cube map with inconsistent size images")
        }

        //顺序:右,左,上,下,后,前
        //img_serial指定开始的偏移,1表示只上传一面图片
        GL_CHECK(glTextureSubImage3D(ID, 0, 0, 0, img_serial, image.width, image.height, 1, image_mode, GL_UNSIGNED_BYTE, image.content));

    }
    GL_CHECK(glGenerateTextureMipmap(ID));
}

//该函数应配合new_texture_cube_lua使用,设置栈顶上的元素的元表为texture_cube
void create_texture_cube_table(lua_State* L){
    //该函数可以避免元表被重复注册,并把元表放在栈顶
    if(luaL_newmetatable(L, "texture_cube")){
        static const luaL_Reg functions[] =
        {
            {"__gc", delete_texture_cube_lua},
            {"active", active_texture_cube_lua},
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

int new_texture_cube_lua(lua_State* L)
{
    const char* faces_pos_list[6] = {"right", "left", "up", "down", "back", "front"};
    const char* faces_name[6];
    luaL_checktype(L, 1, LUA_TTABLE);
    for(int i=0; i<6; i++){
        lua_pushstring(L, faces_pos_list[i]);
        lua_gettable(L, 1);
        faces_name[i] = luaL_checkstring(L, -1);lua_pop(L, 1);
    }

    const char* image_mode = luaL_checkstring(L, 2);
    if(!str2image_mode_table.count(image_mode)){
        return luaL_error(L, "image mode %s is invalid", image_mode);
    }

    const char* texture_mode = luaL_checkstring(L, 3);
    if(!str2texture_mode_table.count(texture_mode)){
        return luaL_error(L, "texture mode %s is invalid", texture_mode);
    }

    const char* min_filter = luaL_checkstring(L, 4);
    if(!str2texture_filter_table.count(min_filter)){
        return luaL_error(L, "texture filter mode %s is invalid", min_filter);
    }

    const char* max_filter = luaL_checkstring(L, 5);
    if(!str2texture_filter_table.count(max_filter)){
        return luaL_error(L, "texture filter %s is invalid", max_filter);
    }

    //创建userdata并把原表填充到栈顶
    void* texture_cube_pp = lua_newuserdata(L, sizeof(void*));
    *(texture_cube**)texture_cube_pp = new texture_cube(
        faces_name,
        str2image_mode_table.at(image_mode),
        str2texture_mode_table.at(texture_mode), str2texture_filter_table.at(min_filter),
        str2texture_filter_table.at(max_filter)
    );
    create_texture_cube_table(L);
    return 1;
}

int delete_texture_cube_lua(lua_State* L){
    delete *(texture_cube**)(luaL_checkudata(L, 1, "texture_cube"));
    return 0;
}

int active_texture_cube_lua(lua_State* L){
    texture_cube* current_texture =  *(texture_cube**)(luaL_checkudata(L, 1, "texture_cube"));
    int slot = luaL_checkinteger(L, 2);
    current_texture->active(slot);
    return 0;
}
