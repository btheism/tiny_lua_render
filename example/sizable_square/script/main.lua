function sleep(n)
  os.execute("sleep " .. tonumber(n))
end

square={}

simple_vertex_shader_code = [[
    #version 330 core
    layout (location = 0) in vec2 LocPos;
    layout (location = 1) in vec2 inTexPos;
    out vec2 TexPos;
    uniform float mix_factor;
    uniform float xpos;
    uniform float ypos;
    uniform float scale;
    void main()
    {
       gl_Position = vec4((LocPos.x-xpos)*scale, (LocPos.y-ypos)*scale, 0.0, 1.0);
       TexPos = inTexPos;
    }
]]
simple_fragment_shader_code = [[
    #version 330 core
    out vec4 FragColor;
    in vec2 TexPos;
    uniform sampler2D Tex0;
    uniform float mix_factor;
    void main()
    {
        FragColor = texture(Tex0, TexPos);
    }
]]

square.shader = shader.new_shader({
{"vertex" ,simple_vertex_shader_code},
{"frag", simple_fragment_shader_code}
})
--根据https://stackoverflow.com/questions/42357380/why-must-i-use-a-shader-program-before-i-can-set-its-uniforms,修改uinform之前必须激活着色器(opengl的历史遗留问题?)
square.shader:use()
square.shader:set_int("Tex"..(0), 0)


square.tex = {}
--{图像格式,纹理格式,图像的swrap,twrap,图像的min过滤器,图像的max过滤器}
square.tex[1] = texture.new_texture_2d("image/yuri.jpg", "rgb", "rgba", "repeat", "repeat", "nearest", "nearest")

square.mesh = mesh.new_mesh(
    "triangles",
    6,
    {
     0.5,  0.5, 1.0, 1.0, --top right
     0.5, -0.5, 1.0, 0.0, --bottom right
    -0.5, -0.5, 0.0, 0.0, --bottom left
    -0.5,  0.5, 0.0, 1.0 --top left
    },
    --顶点属性指针:{序号, 每次读取的坐标数量, 间隔, 首项的偏置}
    {
    {0,2,4,0},
    {1,2,4,2}
    },
    {
    0,2,1,
    0,2,3
    }
    )


function square.draw()
    for i=1,#square.tex do
        square.tex[i]:active(i-1)
    end
    square.shader:set_float("xpos", xpos)
    square.shader:set_float("ypos", ypos)
    square.shader:set_float("scale", scale)
    square.mesh:draw()
end

scale_step=1.1
move_step=0.1

function process_input()
    if(window.key_pressed("escape"))
    then
        window.set_close_bit()
    end
    if(window.key_pressed("up"))
    then
        ypos = ypos+move_step/scale
    end
    if(window.key_pressed("down"))
    then
        ypos = ypos-move_step/scale
    end
    if(window.key_pressed("left"))
    then xpos = xpos-move_step/scale
    end
    if(window.key_pressed("right"))
    then
        xpos = xpos+move_step/scale
    end
    if(window.key_pressed("kp_add"))
    then
        scale = scale*scale_step
    end
    if(window.key_pressed("kp_subtract"))
    then
        scale = scale/scale_step
        if(scale<0.1)
        then
        scale=0.1
        end
    end
end


acc_time=0

function update(dt)
    --print(acc_time)
    acc_time = acc_time+dt/20
    if(acc_time>1.0)
    then
    acc_time=acc_time-1.0
    end
    local red = math.sin(180*acc_time)
    local green = math.sin(180*acc_time)
    local blue = math.sin(180*acc_time)
    window.set_clear_color(red, green, blue, 1)
    window.clear()
    square.draw()
end

function sleep(n)
  os.execute("sleep " .. tonumber(n))
end

last_time = os.clock()
scale=1.0
xpos=0.0
ypos=0.0
window.set_clear_color(0, 0, 0, 1)
while(not window.should_close())
do
    window.fresh()
    process_input()
    current_time = os.clock()
    update((current_time-last_time)*10) --乘10后单位正好变为秒
    last_time = current_time

end

