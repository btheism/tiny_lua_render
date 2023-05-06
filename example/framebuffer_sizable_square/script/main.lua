key_newly_pressed = require("script/key_newly_pressed")

square={}

square_vertex_code = [[
    #version 330 core
    layout (location = 0) in vec2 LocPos;
    layout (location = 1) in vec2 inTexPos;
    out vec2 TexPos;
    uniform float xpos;
    uniform float ypos;
    uniform float scale;
    void main()
    {
       gl_Position = vec4((LocPos.x-xpos)*scale, (LocPos.y-ypos)*scale, 0.0, 1.0);
       TexPos = inTexPos;
    }
]]
square_frag_code = [[
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
--[[
--变换颜色
screen_frag_code = [[
    #version 330 core
    out vec4 FragColor;
    in vec2 TexPos;
    uniform sampler2D Tex0;
    uniform bool which;
    void main()
    {
        //FragColor = vec4(1.0-texture(Tex0, TexPos).rgb, 1.0);//反色
        //FragColor = vec4(0.0, 0.0, texture(Tex0, TexPos).b, 1.0);//提取颜色
        //灰度
        FragColor = texture(Tex0, TexPos);
        float average;
        if(which)
            average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
        else
            average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
        FragColor = vec4(average, average, average, 1.0);


    }
]]
--]]



square.shader = shader.new_shader({
{"vertex" ,square_vertex_code},
{"frag", square_frag_code}
})

--根据https://stackoverflow.com/questions/42357380/why-must-i-use-a-shader-program-before-i-can-set-its-uniforms,修改uinform之前必须激活着色器(opengl的历史遗留问题?)(后来在库中使用了4.5引入的DSA接口,现在不需要了)
square.shader:set_int("Tex"..(0), 0)

--{图像格式,纹理格式,图像的swrap,twrap,图像的min过滤器,图像的max过滤器}
square.tex = texture.new_texture_2d("image/sakura_tomoyo.jpg", "rgb", "rgba", "repeat", "repeat", "nearest", "nearest")

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

kernels={
    blur=
        {1.0/16, 2.0/16, 1.0/16,
        2.0/16, 4.0/16, 2.0/16,
        1.0/16, 2.0/16, 1.0/16},
    --sharpen与edge的区别在于sharpen叠加了像素自身的颜色与edge的值
    sharpen=
        {-1, -1, -1,
        -1,  9, -1,
        -1, -1, -1},
    edge=
        {1, 1, 1,
        1, -8, 1,
        1, 1, 1},

}

screen={}

screen.shader = require("shader/filter_kernel")
screen.shader:set_float("offset", 1.0/450)
screen.shader:set_floats("kernel", 9, kernels.edge)


--根据https://stackoverflow.com/questions/42357380/why-must-i-use-a-shader-program-before-i-can-set-its-uniforms,修改uinform之前必须激活着色器(opengl的历史遗留问题?)(后来在库中使用了4.5引入的DSA接口,现在不需要了)
screen.shader:set_int("Tex"..(0), 0)

fb = framebuffer.new_2d(1400,1200)
--fb = framebuffer.new_2d(140,120)

screen.tex=fb

screen.mesh = mesh.new_mesh(
    "triangles",
    6,
    {
     0.95,  0.95, 0.95, 0.95, --top right
     0.95, -0.95, 0.95, 0.0, --bottom right
    -0.95, -0.95, 0.0, 0.0, --bottom left
    -0.95,  0.95, 0.0, 0.95 --top left
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

screen.which=1

function square.draw()
    square.tex:active(0)
    square.shader:set_float("xpos", xpos)
    square.shader:set_float("ypos", ypos)
    square.shader:set_float("scale", scale)
    square.shader:use()
    square.mesh:draw()
end

function screen.draw()
    screen.tex:active(0)
    screen.shader:use()
    screen.mesh:draw()
end

function process_input()
    if(window.key_pressed("escape"))
    then
        window.set_close_bit()
    end
    if(key_newly_pressed("space"))
    then
        if screen.which==1
        then
            screen.which=0
        else
            screen.which=1
        end
        screen.shader:set_int("which", screen.which)
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

    fb:use()
    window.set_clear_color(red, green, blue, 1)
    window.clear()
    square.draw()
    window.reset_fb()

    window.set_clear_color(1.0-red, 1.0-green, 1.0-blue, 1)
    window.clear()
    screen.draw()
end

function sleep(n)
  os.execute("sleep " .. tonumber(n))
end

last_time = os.clock()
scale=1.0
xpos=0.0
ypos=0.0
scale_step=1.1
move_step=0.1
window.set_clear_color(0, 0, 0, 1)
while(not window.should_close())
do
    window.fresh()
    process_input()
    current_time = os.clock()
    update((current_time-last_time)*10) --乘10后单位正好变为秒
    last_time = current_time

end

