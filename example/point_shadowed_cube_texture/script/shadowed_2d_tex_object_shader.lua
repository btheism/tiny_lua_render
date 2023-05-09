--由于箱子被用作墙壁,上色时对于diff反转了法向量
--对于specular, 由于反射光的方向在反转法向量后不变,因此无需修正


local object_vertex_shader_code = [[
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 2) in vec3 aNormal;
    layout (location = 1) in vec2 aTexPos;
    uniform mat4 cameraM;
    uniform mat4 modelM;
    uniform mat3 normalM;
    out vec2 TexPos;
    out vec3 Normal;
    out vec3 FragPos;//顶点的世界坐标
    void main()
    {
       gl_Position = cameraM*modelM*vec4(aPos, 1.0);
       TexPos = aTexPos;
       Normal = normalize(normalM*aNormal);
       FragPos = vec3(modelM * vec4(aPos, 1.0));
    }
]]
local object_fragment_shader_code = [[
    #version 330 core
    out vec4 FragColor;
    in vec2 TexPos;
    in vec3 Normal;
    in vec3 FragPos;
    uniform float AmbientStrength;
    uniform float DiffStrength;
    uniform float SpecularStrength;
    uniform vec3 LightColor;
    uniform vec3 LightPos;
    uniform vec3 ViewPos;
    uniform sampler2D Tex0;
    uniform samplerCube depthMap;
    float far_plane = 100.0;
    void main()
    {
    vec4 ObjectColor=texture(Tex0, TexPos);

    vec3 fragToLight = FragPos - LightPos;
    float closestDepth = texture(depthMap, fragToLight).r;
    closestDepth *= far_plane;
    float currentDepth = length(fragToLight);
    float bias = 0.05;
    bool in_shadow = currentDepth - bias > closestDepth ? true: false;

    vec3 LightDir = normalize(-fragToLight);
    vec3 ViewDir = normalize(ViewPos - FragPos);
    vec3 ReflectDir = reflect(-LightDir, Normal);
    float spec = in_shadow? 0.0: pow(max(dot(ViewDir, ReflectDir), 0.0), 32);
    float diff = in_shadow? 0.0: max(-dot(Normal, LightDir), 0.0);
    FragColor = (AmbientStrength+diff*DiffStrength+spec*SpecularStrength)*vec4(LightColor,1.0)*ObjectColor;
    }
]]

local object_shader=shader.new_shader({
    {"vertex", object_vertex_shader_code},
    {"frag", object_fragment_shader_code}})
object_shader:use()
object_shader:set_int("Tex"..(0), 0)
object_shader:set_int("depthMap", 7)
print("successfully load 2d texture shader")

return object_shader
