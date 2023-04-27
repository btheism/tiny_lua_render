function sleep(n)
  os.execute("sleep " .. tonumber(n))
end

square={}

simple_vertex_shader_code = [[
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
       gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
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

square.shader = shader.new_shader(simple_vertex_shader_code, simple_fragment_shader_code)
square.mesh = mesh.new_mesh(
    "triangles",
    6,
    {
     0.5,  0.5,  --top right
     0.5, -0.5,  --bottom right
    -0.5, -0.5, --bottom left
    -0.5,  0.5 --top left
    },
    {
    {0,2,2,0}
    },
    {
    0,2,1,
    0,2,3
    }
    )


function square.draw()
    square.shader:use()
    square.mesh:draw()
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

window.set_clear_color(0, 0, 0, 1)
while(not window.should_close())
do
    window.fresh()
    process_input()
    current_time = os.clock()
    update((current_time-last_time)*10) --乘10后单位正好变为秒
    last_time = current_time

end
