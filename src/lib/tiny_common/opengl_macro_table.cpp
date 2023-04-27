#include<unordered_map>
#include<string>
#include<glad/glad.h>

extern const std::unordered_map<const std::string, GLint, std::hash<std::string>> str2texture_wrap_table{
    {"repeat", GL_REPEAT}, {"mirror", GL_MIRRORED_REPEAT},
    {"edge", GL_CLAMP_TO_EDGE}, {"broder", GL_CLAMP_TO_BORDER}
};

extern const std::unordered_map<GLint, const std::string> texture_wrap2str_table{
    {GL_REPEAT, "repeat"}, {GL_MIRRORED_REPEAT, "mirror"},
    {GL_CLAMP_TO_EDGE, "edge"}, {GL_CLAMP_TO_BORDER, "broder"}
};

extern const std::unordered_map<const std::string, GLint, std::hash<std::string>> str2texture_filter_table{
    {"nearest", GL_NEAREST}, {"linear", GL_LINEAR}
};

extern const std::unordered_map<GLint, const std::string> texture_filter2str_table{
    {GL_NEAREST, "nearest"}, {GL_LINEAR, "linear"}
};

extern const std::unordered_map<const std::string, GLint, std::hash<std::string>> str2image_mode_table{
    {"rgb", GL_RGB}, {"rgba", GL_RGBA}
};

extern const std::unordered_map<GLint, const std::string> image_mode2str_table{
    {GL_RGBA, "rgba"}, {GL_RGB, "rgb"}
};

extern const std::unordered_map<GLint, int> image_mode_channel_table{
    {GL_RGB, 3}, {GL_RGBA, 4}
};

extern const std::unordered_map<const std::string, GLenum, std::hash<std::string>> str2gl_primitive_table{
    {"points", GL_POINTS}, {"triangles", GL_TRIANGLES}, {"lines", GL_LINES}
};


