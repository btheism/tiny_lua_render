function sleep(n)
  os.execute("sleep " .. tonumber(n))
end

triangle={}

simple_vertex_shader_code = [[
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
       gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
]]
simple_fragment_shader_code = [[
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
]]

--顶点属性指针:{序号, 每次读取的坐标数量, 间隔, 首项的偏置}
triangle.shader = shader.new_shader(simple_vertex_shader_code, simple_fragment_shader_code)
triangle.mesh = mesh.new_mesh(
    "triangles",
    3,
    {
    -0.5, -0.5, 0.0,  --left
    0.5, -0.5, 0.0, --right
    0.0,  0.5, 0.0  --top
    },
    {
    {0,3,3,0}
    }
    )

function triangle.draw()
    triangle.shader:use()
    triangle.mesh:draw()
end


function process_input()
    if(window.key_pressed("escape"))
    then
        window.set_close_bit()
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
    local green = math.sin(180*(acc_time+2))
    local blue = math.sin(180*(acc_time+5))
    window.set_clear_color(red, green, blue, 1.0)
    window.clear()
    triangle.draw()
end

function sleep(n)
  os.execute("sleep " .. tonumber(n))
end

last_time = os.clock()

window.set_clear_color(0, 0, 0, 1)
while(not window.should_close())
do
    process_input()
    current_time = os.clock()
    update((current_time-last_time)*10) --乘10后单位正好变为秒
    last_time = current_time
    window.fresh()

end
