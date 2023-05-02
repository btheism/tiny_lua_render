camera = require("script/camera")

light={}
box={}
floor={}

box.shader = require("script/z_shader")
box.mesh = require("script/cube_mesh")

floor.shader = box.shader
floor.mesh = require("script/square_mesh")


box.pos = linear.new_vec(3, {0,0,0})
box.rotate = {math.rad(45),linear.new_vec(3, {1,0,0})}
box.scale = {1.0}

floor.pos = linear.new_vec(3, {0,-2,0})
floor.rotate = {0.0,linear.new_vec(3, {1,0,0})}
floor.scale = {40.0}

function draw(object)
    object.shader:use()
    object.shader:set_mat("cameraM", camera.look)
    local modelM =
    linear.move_mat(object.pos)*
    linear.rotate_mat(unpack(object.rotate))*
    linear.scale_mat(unpack(object.scale))
    object.shader:set_mat("modelM",modelM)
    object.mesh:draw()
end

last_press={}
function key_newly_pressed(key_name)
    if(window.key_pressed(key_name))
    then
        if(not last_press[key_name]==true)
        then
            last_press[key_name]=true
            return true
        else
            return false
        end
    else
        last_press[key_name]=false
        return false
    end
end

function process_input()
    if(window.key_pressed("escape"))
    then
        window.set_close_bit()
    end
    camera:process_input()
end

function update(dt)
    camera:update()
    box.rotate[1]=box.rotate[1]+dt*10
    draw(box)
    draw(floor)
end


last_time = os.clock()
window.set_clear_color(30/256,49/256,36/256, 1)
while(not window.should_close())
do
    window.fresh()
    process_input()
    current_time = os.clock()
    update((current_time-last_time)*10) --乘10后单位正好变为秒
    last_time = current_time

end
