shadow_fb = framebuffer.new_depth_cube(1200)
shadow_fb:active(7)
shadow_shader_table = require("script/point_shadow_shader")
shadow_shader = shadow_shader_table[1]
update_shadow_shader = shadow_shader_table[2]


light={}
box={}
wall={}

actobj=box

box.shader = require("script/shadowed_cube_tex_object_shader")
wall.shader = require("script/shadowed_2d_tex_object_shader")
light.shader = require("script/light_shader")

box.mesh = require("script/cube_mesh")
light.mesh = box.mesh
wall.mesh = box.mesh

camera = require("script/camera")

light.color=linear.new_vec(3, {0.5,0.5,0.5})
light.scale={0.2}
light.step=0.02
light.speed=0.1
light.pos = linear.new_vec(3, {3,3,-3})


box.tex = texture.new_texture_cube(
{
front="image/genshin/traveler_male.jpg",
back="image/genshin/traveler_female.jpg",
up="image/genshin/klee.jpg",
down="image/genshin/qiqi.jpg",
left="image/genshin/jean.jpg",
right="image/genshin/keqing.jpg",
}
,"rgb", "rgba", "linear", "linear")
box.ambientstrength=0.1
box.ambient_step=0.05
box.diffstrength=0.5
box.diff_step=0.05
box.specularstrength=0.5
box.spec_step=0.05
box.pos = linear.new_vec(3, {0,0,0})
box.rotate = {0,linear.new_vec(3, {1,0,0})}
box.scale = {0.8,0.5,1.3}

wall.tex = texture.new_texture_2d("image/colorful_grid.png", "rgba", "rgba", "repeat", "repeat", "linear", "linear")
wall.ambientstrength=0.1
wall.ambient_step=0.05
wall.diffstrength=0.5
wall.diff_step=0.05
wall.specularstrength=0.5
wall.spec_step=0.05
wall.pos = linear.new_vec(3, {0,-1,0})
wall.rotate = {0.0,linear.new_vec(3, {1,0,0})}
wall.scale = {15.0}


function draw(object)
    object.shader:use()
    object.shader:set_mat("cameraM", camera.look)
    local modelM =
    linear.move_mat(object.pos)*
    linear.rotate_mat(unpack(object.rotate))*
    linear.scale_mat(unpack(object.scale))
    object.shader:set_mat("modelM",modelM)
    object.shader:set_mat("normalM",linear.normal_mat(modelM))
    object.shader:set_vec("LightColor", light.color)
    object.shader:set_vec("LightPos", light.pos)
    object.shader:set_vec("ViewPos", camera.pos)
    object.shader:set_float("AmbientStrength", object.ambientstrength)
    object.shader:set_float("DiffStrength", object.diffstrength)
    object.shader:set_float("SpecularStrength", object.specularstrength)
    object.tex:active(0)
    object.mesh:draw()
end

function draw_shadow(object)
    local modelM =
    linear.move_mat(object.pos)*
    linear.rotate_mat(unpack(object.rotate))*
    linear.scale_mat(unpack(object.scale))
    shadow_shader:set_mat("modelM", modelM)
    object.mesh:draw()
end


function light:draw()
    self.shader:use()
    self.shader:set_mat("cameraM", camera.look)
    self.shader:set_mat("modelM", linear.move_mat(self.pos)*linear.scale_mat(unpack(self.scale)))
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
    if key_newly_pressed("space")
    then
        if(actobj==box)
        then
            actobj=wall
        else
            actobj=box
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
    if(window.key_pressed("1")) and actobj.ambientstrength+actobj.ambient_step<=1.0
    then
	    actobj.ambientstrength=actobj.ambientstrength+actobj.ambient_step
    end
    if(window.key_pressed("2")) and actobj.ambientstrength-actobj.ambient_step>=0.0
    then
	    actobj.ambientstrength=actobj.ambientstrength-actobj.ambient_step
    end
    if(window.key_pressed("3")) and actobj.diffstrength+actobj.diff_step<=2.0
    then
	    actobj.diffstrength=actobj.diffstrength+actobj.diff_step
    end
    if(window.key_pressed("4")) and actobj.diffstrength-actobj.diff_step>=0.0
    then
	    actobj.diffstrength=actobj.diffstrength-actobj.diff_step
    end
    if(window.key_pressed("5")) and actobj.specularstrength+actobj.spec_step<=2.0
    then
	    actobj.specularstrength=actobj.specularstrength+actobj.spec_step
    end
    if(window.key_pressed("6")) and actobj.specularstrength-actobj.spec_step>=0.0
    then
	    actobj.specularstrength=actobj.specularstrength-actobj.spec_step
    end

    camera:process_input()
end

function update(dt)
    box.rotate[1]=box.rotate[1]+dt*10
    shadow_fb:use()
    shadow_shader:use()
    window.clear()
    update_shadow_shader(shadow_shader, light.pos)
    draw_shadow(box)
    window.reset_fb()
    window.clear()
    camera:update()
    draw(box)
    draw(wall)
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
