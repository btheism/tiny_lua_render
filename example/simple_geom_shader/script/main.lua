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

simple_2d_geom = [[
    #version 450 core
    layout (points) in;
    layout (triangle_strip, max_vertices = 6) out;

    void main() {
        gl_Position = gl_in[0].gl_Position-vec4(0.0,0.1,0.0,0.0);
        EmitVertex();
        gl_Position = gl_in[0].gl_Position-vec4(0.1,0.0,0.0,0.0);
        EmitVertex();
        gl_Position = gl_in[0].gl_Position+vec4(0.1,0.0,0.0,0.0);
        EmitVertex();
        EndPrimitive();
        gl_Layer = 1;

        gl_Position = gl_in[0].gl_Position-vec4(0.1,0.0,0.0,0.0);
        EmitVertex();
        gl_Position = gl_in[0].gl_Position-vec4(0.0,0.1,0.0,0.0);
        EmitVertex();
        gl_Position = gl_in[0].gl_Position+vec4(0.0,0.1,0.0,0.0);
        EmitVertex();
        EndPrimitive();

    }
]]

global_shader= shader.new_shader({
    {"vertex", simple_2d_vertex},
    {"frag", simple_2d_frag},
    {"geom", simple_2d_geom}
})

key_newly_pressed = require("script/key_newly_pressed")
global_shader:use()

triangles={}
triangles.mesh = require("script/triangles_mesh")
triangle_fan={}
triangle_fan.mesh = require("script/triangle_fan_mesh")
triangle_strip={}
triangle_strip.mesh = require("script/triangle_strip_mesh")
points={}
points.mesh = require("script/points_mesh")


last_time = os.clock()
window.set_clear_color(30/256,49/256,36/256, 1)

function process_input()
    if(window.key_pressed("escape"))
    then
        window.set_close_bit()
    end
    if(key_newly_pressed("q"))
    then
        cur_obj=triangles
    end
    if(key_newly_pressed("w"))
    then
        cur_obj=triangle_fan
    end
    if(key_newly_pressed("e"))
    then
        cur_obj=triangle_strip
    end
    if(key_newly_pressed("r"))
    then
        cur_obj=points
    end
--[[
    if(key_newly_pressed("t"))
    then

    end
    --]]
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
    cur_obj.mesh:draw()
end

cur_obj=points

last_time = os.clock()
while(not window.should_close())
do
    window.fresh()
    process_input()
    current_time = os.clock()
    update((current_time-last_time)*10) --乘10后单位正好变为秒
    last_time = current_time
end
