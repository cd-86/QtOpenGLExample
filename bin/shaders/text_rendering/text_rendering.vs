#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;
uniform mat4 uMatrix;
uniform vec2 uFontTextureSize;

void main()
{
	gl_Position = uMatrix * vec4(aPos, 0.0, 1.0);
	vTexCoord = aTexCoord / uFontTextureSize;
}