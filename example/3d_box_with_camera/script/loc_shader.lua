local location_vertex_shader_code = [[
    #version 330 core
    layout (location = 0) in vec3 LocPos;
    layout (location = 1) in vec2 inTexPos;
    uniform mat4 trans;
    out vec2 TexPos;
    void main()
    {
       gl_Position = trans*vec4(LocPos, 1.0);
       TexPos = inTexPos;
    }
]]
local location_fragment_shader_code = [[
    #version 330 core
    out vec4 FragColor;
    in vec2 TexPos;
    uniform sampler2D Tex0;
    void main()
    {
        FragColor = texture(Tex0, TexPos);
    }
]]

local loc_shader=shader.new_shader(location_vertex_shader_code, location_fragment_shader_code)
loc_shader:use()
loc_shader:set_int("Tex"..(0), 0)

return loc_shader
