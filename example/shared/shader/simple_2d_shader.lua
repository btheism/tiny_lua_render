simple_2d_vertex = [[
    #version 450 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
       gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    }
]]

simple_2d_frag = [[
    #version 450 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
]]

return shader.new_shader({
    {"vertex", simple_2d_vertex},
    {"frag", simple_2d_frag}
})
