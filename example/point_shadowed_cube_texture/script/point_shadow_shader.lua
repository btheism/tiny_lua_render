--把顶点坐标转换为世界坐标
local shadow_cal_vertex_code=[[
    #version 450 core
    layout (location = 0) in vec3 position;
    uniform mat4 modelM;
    void main()
    {
        gl_Position = modelM * vec4(position, 1.0);
    }
]]

--把世界坐标转换为以光源为中心的坐标
local shadow_cal_geom_code=[[
    #version 450 core
    layout (triangles) in;
    layout (triangle_strip, max_vertices=18) out;

    uniform mat4 shM[6];
    uniform mat4 perspective;
    out vec4 FragPos;

    void main()
    {
        for(int face = 0; face < 6; ++face)
        {
            gl_Layer = face;
            //顺序:右,左,上,下,后,前
            for(int i = 0; i < 3; ++i) // for each triangle's vertices
            {
                FragPos = gl_in[i].gl_Position;
                gl_Position = perspective*shM[face]*FragPos;
                EmitVertex();
            }
            EndPrimitive();
        }
    }
]]

--计算z缓冲
local shadow_cal_frag_code=[[
    #version 450 core
    in vec4 FragPos;

    uniform vec3 lightPos;
    float far_plane = 100.0;

    void main()
    {
        float lightDistance = length(FragPos.xyz - lightPos);

        lightDistance = lightDistance / far_plane;

        gl_FragDepth = lightDistance;
    }
]]

local point_shadow_shader=shader.new_shader({
{"vertex", shadow_cal_vertex_code},
{"geom", shadow_cal_geom_code},
{"frag", shadow_cal_frag_code},
})
--顺序:右,左,上,下,后,前
point_shadow_shader:set_mat("perspective", linear.persp_mat(math.rad(90.0), 1.0, 0.1, 1000))

--更新光源位置
local vec_table={
    {linear.new_vec(3, {1.0, 0.0, 0.0}), linear.new_vec(3, {0.0,-1.0, 0.0})},--右
    {linear.new_vec(3, {-1.0,0.0, 0.0}), linear.new_vec(3, {0.0,-1.0, 0.0})},--左
    {linear.new_vec(3, {0.0, 1.0, 0.0}), linear.new_vec(3, {0.0, 0.0, 1.0})},--上
    {linear.new_vec(3, {0.0,-1.0, 0.0}), linear.new_vec(3, {0.0, 0.0,-1.0})},--下
    {linear.new_vec(3, {0.0, 0.0, 1.0}), linear.new_vec(3, {0.0,-1.0, 0.0})},--后
    {linear.new_vec(3, {0.0, 0.0,-1.0}), linear.new_vec(3, {0.0,-1.0, 0.0})}--前
}

function update_shadow_shader(shader, light_pos)
    local dir_vecs=vec_table
    for i=1,6 do
        shader:set_mat("shM["..(i-1).."]", linear.lookat_mat(light_pos, light_pos+dir_vecs[i][1], dir_vecs[i][2]));
    end
end

return {point_shadow_shader,update_shadow_shader}

