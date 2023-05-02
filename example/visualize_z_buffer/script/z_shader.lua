--这个着色器用于渲染物体

local z_vertex_shader_code = [[
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 cameraM;
    uniform mat4 modelM;
    void main()
    {
       gl_Position = cameraM*modelM*vec4(aPos, 1.0);
    }
]]
local z_fragment_shader_code = [[
    #version 330 core
    out vec4 FragColor;
    float near = 0.1;
    float far  = 100.0;
    float LinearizeDepth(float depth){
        // 转换回NDC,即[-1,1]
        float z = depth * 2.0 - 1.0;
        /*
        假设实际的坐标为z0,则-1/near=>-1;-1/far=>1;1/z0=>z
        有:(z-(-1))/(1/z0-(-1/near))=(z-1)/(1/z0-(-1/far))
        =>z0=2*near*far/(z*(far-near)-(far+near)),这个坐标位于世界系(右手系),为负,需要反转
        */
        return (2.0 * near * far) / (far + near - z * (far - near));
    };
    float gamma = 2.2;
    float factor = 20.0;//这个场景中的物体的距离多在20以内,因此把[0,20]内的距离映射均匀到[0,1]
    void main()
    {
        //离得越近越亮
        float depth = 1.0-(1.0*LinearizeDepth(gl_FragCoord.z) / factor); // 归一化
        FragColor = vec4(vec3(depth), 1.0);
        //gamma校正以增加区分度
        //FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
    }
]]

local object_shader=shader.new_shader(z_vertex_shader_code, z_fragment_shader_code)
object_shader:use()
return object_shader
