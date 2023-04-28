--与glm_matrix对照,两者的输出应该一致
--不打算实现vector了,请把一维矩阵当vector用
m1=linear.new_mat("col", 4, 4, {2,2,2,2,3,13,17,19,3,4,7,8,3,5,7,3})
print(m1)--行优先
print(linear.inverse(m1))
v1=linear.new_mat("row", 1, 4, {2,3,5,7})
v2=linear.new_mat("col", 4, 1, {2,3,5,7})
print(v1)
print(v2)
print(m1*v1)
print(v2*m1)
