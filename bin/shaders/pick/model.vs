#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragPos;
out vec3 vViewPos;
out mat3 vTBN;

uniform mat4 uMatrix;
uniform mat4 uModel;
uniform mat3 uNormalMatrix;
uniform vec3 uViewPos;
uniform int uNormalMapping;

void main(void)
{
    gl_Position = uMatrix * vec4(aPos, 1.0f);
    vFragPos = vec3(uModel * vec4(aPos, 1.0f));
    vTexCoord = aTexCoord;
    vNormal = uNormalMatrix * aNormal;

    if (uNormalMapping != 0) {
        vec3 T = normalize(uNormalMatrix * aTangent);
        vec3 B = normalize(uNormalMatrix * aBitangent);
        vec3 N = normalize(uNormalMatrix * aNormal);

        vTBN = transpose(mat3(T, B, N));
        vViewPos  = vTBN * uViewPos;
        vFragPos  = vTBN * vFragPos;
    }
}
