--增加了法线
--法线坐标也是针对局部坐标系的,指向面外
--比较通用,放到shared目录下
local box_mesh = mesh.new_mesh(
    "triangles",
    36,
    {
    -0.5, -0.5, -0.5,  0.0, 0.0, 0.0, 0.0, -1.0,
    0.5, -0.5, -0.5,  1.0, 0.0, 0.0, 0.0, -1.0,
    0.5,  0.5, -0.5,  1.0, 1.0, 0.0, 0.0, -1.0,
    0.5,  0.5, -0.5,  1.0, 1.0, 0.0, 0.0, -1.0,
    -0.5,  0.5, -0.5,  0.0, 1.0, 0.0, 0.0, -1.0,
    -0.5, -0.5, -0.5,  0.0, 0.0, 0.0, 0.0, -1.0,

    -0.5, -0.5,  0.5,  0.0, 0.0, 0.0, 0.0, 1.0,
    0.5, -0.5,  0.5,  1.0, 0.0, 0.0, 0.0, 1.0,
    0.5,  0.5,  0.5,  1.0, 1.0, 0.0, 0.0, 1.0,
    0.5,  0.5,  0.5,  1.0, 1.0, 0.0, 0.0, 1.0,
    -0.5,  0.5,  0.5,  0.0, 1.0, 0.0, 0.0, 1.0,
    -0.5, -0.5,  0.5,  0.0, 0.0, 0.0, 0.0, 1.0,

    -0.5,  0.5,  0.5,  1.0, 0.0, -1.0, 0.0, 0.0,
    -0.5,  0.5, -0.5,  1.0, 1.0, -1.0, 0.0, 0.0,
    -0.5, -0.5, -0.5,  0.0, 1.0, -1.0, 0.0, 0.0,
    -0.5, -0.5, -0.5,  0.0, 1.0, -1.0, 0.0, 0.0,
    -0.5, -0.5,  0.5,  0.0, 0.0, -1.0, 0.0, 0.0,
    -0.5,  0.5,  0.5,  1.0, 0.0, -1.0, 0.0, 0.0,

    0.5,  0.5,  0.5,  1.0, 0.0, 1.0, 0.0, 0.0,
    0.5,  0.5, -0.5,  1.0, 1.0, 1.0, 0.0, 0.0,
    0.5, -0.5, -0.5,  0.0, 1.0, 1.0, 0.0, 0.0,
    0.5, -0.5, -0.5,  0.0, 1.0, 1.0, 0.0, 0.0,
    0.5, -0.5,  0.5,  0.0, 0.0, 1.0, 0.0, 0.0,
    0.5,  0.5,  0.5,  1.0, 0.0, 1.0, 0.0, 0.0,

    -0.5, -0.5, -0.5,  0.0, 1.0, 0.0, -1.0, 0.0,
    0.5, -0.5, -0.5,  1.0, 1.0, 0.0, -1.0, 0.0,
    0.5, -0.5,  0.5,  1.0, 0.0, 0.0, -1.0, 0.0,
    0.5, -0.5,  0.5,  1.0, 0.0, 0.0, -1.0, 0.0,
    -0.5, -0.5,  0.5,  0.0, 0.0, 0.0, -1.0, 0.0,
    -0.5, -0.5, -0.5,  0.0, 1.0, 0.0, -1.0, 0.0,

    -0.5,  0.5, -0.5,  0.0, 1.0, 0.0, 1.0, 0.0,
    0.5,  0.5, -0.5,  1.0, 1.0, 0.0, 1.0, 0.0,
    0.5,  0.5,  0.5,  1.0, 0.0, 0.0, 1.0, 0.0,
    0.5,  0.5,  0.5,  1.0, 0.0, 0.0, 1.0, 0.0,
    -0.5,  0.5,  0.5,  0.0, 0.0, 0.0, 1.0, 0.0,
    -0.5,  0.5, -0.5,  0.0, 1.0, 0.0, 1.0, 0.0
    },
    {
    {0,3,8,0},--顶点坐标
    {1,2,8,3}, --纹理坐标(保持为1,以向前兼容)
    {2,3,8,5}--顶点法线
    }
    )
return box_mesh
