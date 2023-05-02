light={}
box={}
box.shader = require("script/object_shader")
light.shader = require("script/light_shader")
box.mesh = require("script/cube_mesh")
light.mesh = box.mesh
camera = require("script/camera")

light.color=linear.new_vec(3, {0.5,0.5,0.5})
light.scale=0.2
light.step=0.02
light.speed=0.1

tex_keqing = texture.new_texture_2d("image/genshin/keqing.jpg", "rgb", "rgba", "repeat", "repeat", "linear", "linear")

tex_klee = texture.new_texture_2d("image/genshin/klee.jpg", "rgb", "rgba", "repeat", "repeat", "linear", "linear")
box.tex=tex_keqing
box.ambientstrength=0.1
box.ambient_step=0.05
box.diffstrength=0.5
box.diff_step=0.05
box.specularstrength=0.5
box.spec_step=0.05

light.pos = linear.new_vec(3, {3,3,-3})

box.pos = linear.new_vec(3, {0,0,0})
box.rotate = {math.rad(45),linear.new_vec(3, {1,0,0})}

function box:draw()
    self.shader:use()
    self.shader:set_mat("cameraM", camera.look)
    self.shader:set_mat("modelM", linear.move_mat(self.pos)*linear.rotate_mat(unpack(self.rotate)))
    self.shader:set_vec("LightColor", light.color)
    self.shader:set_vec("LightPos", light.pos)
    self.shader:set_vec("ViewPos", camera.pos)
    self.shader:set_float("AmbientStrength", self.ambientstrength)
    self.shader:set_float("DiffStrength", self.diffstrength)
    self.shader:set_float("SpecularStrength", self.specularstrength)
    self.tex:active(0)
    self.mesh:draw()
end

function light:draw()
    self.shader:use()
    self.shader:set_mat("cameraM", camera.look)
    self.shader:set_mat("modelM", linear.move_mat(self.pos)*linear.scale_mat(self.scale))
    self.shader:set_vec("LightColor", self.color)
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
            box.tex=tex_klee
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
    if(window.key_pressed("kp_8"))
    then
	    light.pos=light.pos+camera.up*light.speed
    end
    if(window.key_pressed("kp_2"))
    then
	    light.pos=light.pos-camera.up*light.speed
    end
    if(window.key_pressed("kp_4"))
    then
	    light.pos=light.pos-camera.right*light.speed
    end
    if(window.key_pressed("kp_6"))
    then
	    light.pos=light.pos+camera.right*light.speed
    end
    if(window.key_pressed("kp_1"))
    then
	    light.pos=light.pos-camera.front*light.speed
    end
    if(window.key_pressed("kp_3"))
    then
	    light.pos=light.pos+camera.front*light.speed
    end
    if(window.key_pressed("1")) and box.ambientstrength+box.ambient_step<=1.0
    then
	    box.ambientstrength=box.ambientstrength+box.ambient_step
    end
    if(window.key_pressed("2")) and box.ambientstrength-box.ambient_step>=0.0
    then
	    box.ambientstrength=box.ambientstrength-box.ambient_step
    end
    if(window.key_pressed("3")) and box.diffstrength+box.diff_step<=2.0
    then
	    box.diffstrength=box.diffstrength+box.diff_step
    end
    if(window.key_pressed("4")) and box.diffstrength-box.diff_step>=0.0
    then
	    box.diffstrength=box.diffstrength-box.diff_step
    end
    if(window.key_pressed("5")) and box.specularstrength+box.spec_step<=2.0
    then
	    box.specularstrength=box.specularstrength+box.spec_step
    end
    if(window.key_pressed("6")) and box.specularstrength-box.spec_step>=0.0
    then
	    box.specularstrength=box.specularstrength-box.spec_step
    end

    camera:process_input()
end

function update(dt)
    camera:update()
    box.rotate[1]=box.rotate[1]+dt*10
    box:draw()
    light:draw()
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
