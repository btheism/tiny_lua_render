--这个着色器用于渲染物体
--正确实现了法线矩阵

local object_vertex_shader_code = [[
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 2) in vec3 aNormal;//注意法线位置是2!!!
    uniform mat4 cameraM;
    uniform mat4 modelM;
    uniform mat3 normalM;//=transpose(inverse(modelM)),为了速度,在cpu端计算
    out vec3 TexPos;
    out vec3 Normal;
    out vec3 FragPos;//顶点的世界坐标
    void main()
    {
       gl_Position = cameraM*modelM*vec4(aPos, 1.0);
       TexPos = aPos;
       Normal = normalize(normalM*aNormal);
       FragPos = vec3(modelM * vec4(aPos, 1.0));
    }
]]
local object_fragment_shader_code = [[
    #version 330 core
    out vec4 FragColor;
    in vec3 TexPos;
    in vec3 Normal;
    in vec3 FragPos;
    uniform float AmbientStrength;
    uniform float DiffStrength;
    uniform float SpecularStrength;
    uniform vec3 LightColor;
    uniform vec3 LightPos;
    uniform vec3 ViewPos;
    uniform samplerCube Tex0;
    void main()
    {
    vec4 ObjectColor=texture(Tex0, TexPos);
    vec3 LightDir = normalize(LightPos - FragPos);
    vec3 ViewDir = normalize(ViewPos - FragPos);
    vec3 ReflectDir = reflect(-LightDir, Normal);
    float spec = pow(max(dot(ViewDir, ReflectDir), 0.0), 32);
    float diff = max(dot(Normal, LightDir), 0.0);
    FragColor = (AmbientStrength+diff*DiffStrength+spec*SpecularStrength)*vec4(LightColor,1.0)*ObjectColor;
    }
]]

local object_shader=shader.new_shader({
    {"vertex", object_vertex_shader_code},
    {"frag", object_fragment_shader_code}
    })

object_shader:use()
object_shader:set_int("Tex"..(0), 0)
print("successfully load cube texture shader")
return object_shader
