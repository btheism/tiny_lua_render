local camera={}
camera.pos=linear.new_vec(3, {0.0, 0.0, 3.0})
camera.front=linear.new_vec(3, {0.0, 0.0, -1.0})
camera.worldup=linear.new_vec(3, {0.0, 1.0, 0.0})
camera.right=camera.front^camera.worldup
--相机的两个角
camera.yaw=math.rad(-90.0)
camera.pitch=0.0
--视角
camera.zoom=math.rad(45.0)
--相机的敏感度
camera.speed=0.05
camera.sensitive=0.05
camera.zoom_sen=0.01
--相机的方向向量

function camera:update()
    self.front=linear.norm_vec(
        linear.new_vec(
        3,{
            math.cos(self.pitch) * math.cos(self.yaw);
            math.sin(self.pitch);
            math.cos(self.pitch) * math.sin(self.yaw);
        }))
    self.right=camera.front^camera.worldup
    self.look=
    linear.new_mat("persp",self.zoom, 1.0, 0.1, 100)
    *linear.new_mat("lookat",
    self.pos,
    self.pos+self.front,
    self.worldup)
end

function camera:show_info()
    print("yaw:"..self.yaw)
    print("pitch:"..self.pitch)
    print("front vector: "..tostring(self.front))
    print("camear pos: "..tostring(self.pos))
    print("look at:\n"..tostring(
    linear.new_mat("lookat",
    self.pos,
    self.pos+self.front,
    self.worldup)))
end

function camera:process_input()
    if window.key_pressed("up")
    then
        self.pos=self.pos+(self.front*self.speed)
        self:show_info()
    end
    if window.key_pressed("down")
    then
        self.pos=self.pos-(self.front*self.speed)
        self:show_info()
    end
    if window.key_pressed("left")
    then
        self.pos=self.pos-(self.right*self.speed)
        self:show_info()
    end
    if window.key_pressed("right")
    then
        self.pos=self.pos+(self.right*self.speed)
        self:show_info()
    end
    if window.key_pressed("w") and self.pitch+self.sensitive<math.rad(88.0)
    then
        self.pitch=self.pitch+self.sensitive
        self:show_info()
    end
    if window.key_pressed("s") and self.pitch-self.sensitive>math.rad(-88.0)
    then
        self.pitch=self.pitch-self.sensitive
        self:show_info()
    end
    if window.key_pressed("a")
    then
        self.yaw=self.yaw-self.sensitive
        self:show_info()
    end
    if window.key_pressed("d")
    then
        self.yaw=self.yaw+self.sensitive
        self:show_info()
    end
    if window.key_pressed("kp_add") and self.zoom-self.zoom_sen>math.rad(3.0)
    then
        self.zoom=self.zoom-self.zoom_sen
    end
    if window.key_pressed("kp_subtract") and self.zoom+self.zoom_sen<math.rad(75.0)
    then
        self.zoom=self.zoom+self.zoom_sen
    end
end
camera:update()
return camera

