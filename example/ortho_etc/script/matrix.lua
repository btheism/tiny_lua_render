--与glm_ortho_etc对照,两者的输出应该一致
m1=linear.new_mat("ortho", 0.0, 800.0, 0.0, 600.0, 0.1, 100.0)
m2=linear.trans(m1)
m3=linear.inverse(m1)
print(m1)
print(m2)
print(m3)
m1=linear.new_mat("persp", math.rad(54.0), 16.0/9.0, 0.1, 100.0)
m2=linear.trans(m1)
m3=linear.inverse(m1)
print(m1)
print(m2)
print(m3)
m1=linear.new_mat("move", 1.0, 2.0, 3.0)
print(m1)
m1=linear.new_mat("rotate", math.rad(-55.0), 1.0, 0.0, 0.0)
print(m1)
m1=linear.new_mat("lookat", 0.0, 0.0, 3.0, 0.0, 0.5, 0.0, 1.0, 1.0, 0.0)
print(m1)
