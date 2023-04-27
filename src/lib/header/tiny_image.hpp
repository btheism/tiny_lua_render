#ifndef __TINY_IMAGE
#define __TINY_IMAGE
#include<string>
class image_8bit{
public:
    unsigned char * content;
    int len;
    int width;
    int height;
    int channel;
    image_8bit(const char* image_name, bool flip);
    image_8bit(const char* image_name, bool flip, int exp_channel);
    ~image_8bit(void);
};

//int write_image(std::string img_name);
#endif
