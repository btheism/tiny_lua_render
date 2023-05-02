light={}
light.color=linear.new_vec(3, {0.5,0.5,0.5})
light.step=0.02

box={}
box.shader = require("script/shader")
box.mesh = require("script/box_mesh")
box.camera = require("script/camera")
tex_keqing = texture.new_texture_2d("image/genshin/keqing.jpg", "rgb", "rgba", "repeat", "repeat", "linear", "linear")
tex_keli = texture.new_texture_2d("image/genshin/klee.jpg", "rgb", "rgba", "repeat", "repeat", "linear", "linear")
box.tex=tex_keqing


box.pos = linear.new_vec(3, {0,0,0})
box.rotate = {math.rad(45),linear.new_vec(3, {1,2,3})}

function box:draw()
    local final_mat =
    self.camera.look
    *linear.move_mat(self.pos)
    *linear.rotate_mat(unpack(self.rotate))
    self.shader:set_mat("trans", final_mat)
    self.shader:set_vec("lightColor", light.color)
    self.tex:active(0)
    self.mesh:draw()
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
    if(window.key_pressed("r")) and light.color[1]+light.step<=1.0
    then
        light.color[1]=light.color[1]+light.step;
    end
    if(window.key_pressed("t")) and light.color[1]-light.step>=0.0
    then
        light.color[1]=light.color[1]-light.step;
    end
    if(window.key_pressed("g")) and light.color[2]+light.step<=1.0
    then
        light.color[2]=light.color[2]+light.step;
    end
    if(window.key_pressed("h")) and light.color[2]-light.step>=0.0
    then
        light.color[2]=light.color[2]-light.step;
    end
    if(window.key_pressed("b")) and light.color[3]+light.step<=1.0
    then
        light.color[3]=light.color[3]+light.step;
    end
    if(window.key_pressed("n")) and light.color[3]-light.step>=0.0
    then
        light.color[3]=light.color[3]-light.step;
    end

    box.camera:process_input()
end

acc_time=0
function update(dt)
    acc_time = acc_time+dt/20
    if(acc_time>1.0)
    then
    acc_time=acc_time-1.0
    end
    box.camera:update()
    local green = math.sin(180*acc_time)
    box.rotate[1]=box.rotate[1]+dt*10
    box:draw()
    window.set_clear_color(0, green, 0, 1)
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

