return mesh.new_mesh(
    "triangles",
    6,
    {
    -0.5,-0.5,-0.5,-0.7,-0.7,-0.5, --左上角
    0.5, 0.5, 0.5, 0.7, 0.7, 0.5  --右下角
    },
    --编号,数量,间隔,起始偏移量
    {
    {0,2,2,0}
    }
)
