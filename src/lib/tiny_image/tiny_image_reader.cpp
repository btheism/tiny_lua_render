#define STB_IMAGE_IMPLEMENTATION
#include<stb/stb_image.h>
#include<tiny_image.hpp>
#include<common.hpp>

void set_image_flip(bool value){
    stbi_set_flip_vertically_on_load_thread(value);
    return;
}

image_8bit::image_8bit(const char* image_name, bool flip){
    set_image_flip(flip);
    file_buffer encoded_image_data = read_from_PHYSFS(image_name);
    int image_width, image_height, image_channel;
    //传入0表示总是加载所有通道
    content = stbi_load_from_memory((unsigned char*)encoded_image_data.content, (int)encoded_image_data.len, &width, &height, &channel, 0);
    delete[] encoded_image_data.content;
    if(content==nullptr){
        fatal("cannot load image %s\n", image_name);
    }
};

image_8bit::image_8bit(const char* image_name, bool flip, int exp_channel){
    set_image_flip(flip);
    file_buffer encoded_image_data = read_from_PHYSFS(image_name);
    int image_width, image_height, image_channel;
    //传入0表示总是加载所有通道
    content = stbi_load_from_memory((unsigned char*)encoded_image_data.content, (int)encoded_image_data.len, &width, &height, &channel, exp_channel);
    delete[] encoded_image_data.content;
    if(content==nullptr){
        fatal("cannot load image %s\n", image_name);
    }
};

image_8bit::~image_8bit(void){
    stbi_image_free(content);
}



