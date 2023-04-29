box={}
box.shader = require("script/loc_shader")
box.mesh = require("script/box_mesh")
tex_keqing = texture.new_texture_2d("image/keqing.jpg", "rgb", "rgba", "repeat", "repeat", "linear", "linear")
tex_keli = texture.new_texture_2d("image/keli.jpg", "rgb", "rgba", "repeat", "repeat", "linear", "linear")
box.tex=tex_keqing

box.pos = {0,0,-3}
box.rotate = {math.rad(45),1,2,3}

camera0={}
camera0.pos={0,0,0}
camera0.clip={"persp", math.rad(45),1.0,0.1,100}


function box:draw(camera)
    local final_mat =
    linear.new_mat(unpack(camera0.clip))
    *linear.new_mat("move", unpack(camera.pos))
    *linear.new_mat("move", unpack(self.pos))
    *linear.new_mat("rotate", unpack(self.rotate))
    box.shader:set_mat("trans", final_mat)
    box.tex:active(0)
    box.mesh:draw()
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
    if key_newly_pressed("c")
    then
        if(box.tex==tex_keqing)
        then
            box.tex=tex_keli
        else
            box.tex=tex_keqing
        end
    end
    if(window.key_pressed("up"))
    then
        camera0.clip[2]=camera0.clip[2]+0.04
        if(camera0.clip[2]>1.5)
        then
            camera0.clip[2]=1.5
        end
    end
    if(window.key_pressed("down"))
    then
        camera0.clip[2]=camera0.clip[2]-0.04
        if(camera0.clip[2]<0.05)
        then
            camera0.clip[2]=0.05
        end
    end

end

acc_time=0
function update(dt)
    acc_time = acc_time+dt/20
    if(acc_time>1.0)
    then
    acc_time=acc_time-1.0
    end
    local red = math.sin(180*acc_time)
    local green = math.sin(180*acc_time)
    local blue = math.sin(180*acc_time)
    box.rotate[1]=box.rotate[1]+dt*10
    box:draw(camera0)
    window.set_clear_color(red, green, blue, 1)
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

