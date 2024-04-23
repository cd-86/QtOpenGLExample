#version 330 core
// 平行光
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 点光(暂不使用)
//struct PointLight {
//    vec3 position;

//    float constant;
//    float linear;
//    float quadratic;

//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};

// 投射光
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;
in vec3 vViewPos;
in mat3 vTBN;

out vec4 FragColor;

uniform vec3 uViewPos;
uniform DirLight dirLight;
//uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform sampler2D texture_diffuse1; // 漫反射
uniform sampler2D texture_specular1; // 镜面反射
uniform sampler2D texture_normals1; // 法线贴图
uniform vec4 uDiffuseColor;
uniform vec4 uSpecularColor;

uniform float uShininess;
uniform int uNormalMapping;
uniform int uTextureMapping;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuse, vec3 specular);
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse, vec3 specular);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse, vec3 specular);

void main()
{
    vec3 norm, viewDir;
    vec4 diffuse, specular;
    if (uTextureMapping == 1) {
        diffuse = uDiffuseColor;
        specular = uSpecularColor;
    } else {
        diffuse = texture(texture_diffuse1, vTexCoord);
        specular = texture(texture_specular1, vTexCoord);
    }
    if (uNormalMapping != 0) {
        norm = texture(texture_normals1, vTexCoord).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        viewDir = normalize(vViewPos - vFragPos);
    } else {
        norm = normalize(vNormal);
        viewDir = normalize(uViewPos - vFragPos);
    }
    // 计算平行光
    vec3 result = CalcDirLight(dirLight, norm, viewDir, diffuse.rgb, specular.rgb);
    // 计算点光
//    for(int i = 0; i < 4; i++)
//            result += CalcPointLight(pointLight, norm, vFragPos, viewDir, diffuse.rgb, specular.rgb);
    // 计算投射光
    result += CalcSpotLight(spotLight, norm, vFragPos, viewDir, diffuse.rgb, specular.rgb);
    FragColor = vec4(result, 1.0);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuse, vec3 specular)
{
    // 漫反射
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面反射
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), uShininess);
    // 计算合并最终光照结果
    vec3 a = light.ambient * diffuse;
    vec3 d = light.diffuse * diff * diffuse;
    vec3 s = light.specular * spec * specular;
    return (a + d + s);
}

//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse, vec3 specular)
//{
//    // 漫反射
//    vec3 lightDir = uNormalMapping == 0 ? normalize(light.position - fragPos) : normalize(vTBN * light.position - fragPos);
//    float diff = max(dot(normal, lightDir), 0.0);
//    // 镜面反射
////    vec3 reflectDir = reflect(-lightDir, normal);
////    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);
//    vec3 halfwayDir = normalize(lightDir + viewDir);
//    float spec = pow(max(dot(normal, halfwayDir), 0.0), uShininess);
//    // 计算光强
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//    // 计算合并最终光照结果
//    vec3 a = light.ambient * diffuse;
//    vec3 d = light.diffuse * diff * diffuse;
//    vec3 s = light.specular * spec * specular;
//    a *= attenuation;
//    d *= attenuation;
//    s *= attenuation;
//    return (a + d + s);
//}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuse, vec3 specular)
{
    // 漫反射
    vec3 lightDir = uNormalMapping == 0 ? normalize(light.position - fragPos) : normalize(vTBN * light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面反射
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), uShininess);
   // 计算光强
   float distance = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
   float theta = dot(lightDir, normalize(-light.direction));
   float epsilon = light.cutOff - light.outerCutOff;
   float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // 计算合并最终光照结果
    vec3 a = light.ambient * diffuse;
    vec3 d = light.diffuse * diff * diffuse;
    vec3 s = light.specular * spec * specular;
    a *= attenuation * intensity;
    d *= attenuation * intensity;
    s *= attenuation * intensity;
    return (a + d + s);
}
