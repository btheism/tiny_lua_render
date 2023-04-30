local camera={}
camera.pos=linear.new_vec(3, {0.0, 0.0, 3.0})
camera.front=linear.new_vec(3, {0.0, 0.0, -1.0})
camera.worldup=linear.new_vec(3, {0.0, 1.0, 0.0})
camera.right=linear.new_vec(3, {0.0, 0.0, -1.0})
--相机的两个角
camera.yaw=math.rad()
camera.pitch=0.0
--相机的敏感度
camera.speed=0.1
camera.sensitive=
--视角
camera.zoom=math.rad(45.0)
--相机的方向向量

function camera:update()
    self.front=linear.norm_vec(
        linear.new_vec(
        3,{
            math.cos(math.rad(self.pitch)) * math.cos(math.rad(self.yaw));
            math.sin(math.rad(self.pitch));
            math.cos(math.rad(self.pitch)) * math.sin(math.rad(self.yaw));
        })
    self.lookat=linear.new_mat("lookat",
    self.pos,
    self.pos+self.front,
    self.worldup)
)

function camera:process(){
    if window.key_pressed("up")
    then
        camera.pos=camera.pos+camera.front*camera.speed
    end
    if window.key_pressed("down")
    then
        camera.pos=camera.pos-camera.front*camera.speed
    end
    if window.key_pressed("left")
    then
    end
    if window.key_pressed("right")
    then
    end

}

return camera

