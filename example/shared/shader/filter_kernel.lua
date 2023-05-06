vertex_code = [[
    #version 450 core
    layout (location = 0) in vec2 LocPos;
    layout (location = 1) in vec2 inTexPos;
    out vec2 TexPos;
    void main()
    {
       gl_Position = vec4(LocPos.x, LocPos.y, 0.0, 1.0);
       TexPos = inTexPos;
    }
]]

--两个参数:采样距离,核

frag_code = [[
    #version 450 core
    uniform float offset;
    uniform sampler2D Tex0;
    uniform float kernel[9];
    in vec2 TexPos;
    out vec4 FragColor;

    void main()
    {
        vec2 offsets[9] = vec2[](
            vec2(-offset,  offset), // 左上
            vec2( 0.0f,    offset), // 正上
            vec2( offset,  offset), // 右上
            vec2(-offset,  0.0f),   // 左
            vec2( 0.0f,    0.0f),   // 中
            vec2( offset,  0.0f),   // 右
            vec2(-offset, -offset), // 左下
            vec2( 0.0f,   -offset), // 正下
            vec2( offset, -offset)  // 右下
        );

        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(Tex0, TexPos.st + offsets[i]));
        }
        vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++)
            col += sampleTex[i] * kernel[i];

        FragColor = vec4(col, 1.0);
    }
]]


return shader.new_shader({
{"vertex" ,vertex_code},
{"frag", frag_code}
})
