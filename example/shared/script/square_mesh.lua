--三维空间中的地面
return mesh.new_mesh(
    "triangles",
    6,
    {
     0.5, 0.0, 0.5, 1.0,  1.0, 0.0, 1.0, 0.0,
     0.5, 0.0, -0.5, 1.0, 0.0, 0.0, 1.0, 0.0,
    -0.5, 0.0,-0.5, 0.0,  0.0, 0.0, 1.0, 0.0,
    -0.5, 0.0, 0.5, 0.0,  1.0, 0.0, 1.0, 0.0,
    },
    {
    {0,3,8,0},--顶点坐标(0,1,2)
    {1,2,8,3}, --纹理坐标(保持为1,以向前兼容)(3,4)
    {2,3,8,5}--顶点法线(5,6,7)
    },
    {
    --EBO
    0,2,1,
    0,2,3
    }
    )
