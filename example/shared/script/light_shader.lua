--这个着色器用于渲染光源,因此直接显示光的颜色,实际上并未进行计算
--光线着色器比较通用,故放到shared文件夹

local light_vertex_shader_code = [[
    #version 330 core
    layout (location = 0) in vec3 LocPos;
    uniform mat4 cameraM;
    uniform mat4 modelM;
    void main()
    {
       gl_Position = cameraM*modelM*vec4(LocPos, 1.0);
    }
]]
local light_fragment_shader_code = [[
    #version 330 core
    out vec4 FragColor;
    uniform vec3 LightColor;
    void main()
    {
        FragColor = vec4(LightColor,1.0f);
    }
]]

print("successfully load shader for light")

return shader.new_shader({
{"vertex", light_vertex_shader_code},
{"frag", light_fragment_shader_code}}
)
