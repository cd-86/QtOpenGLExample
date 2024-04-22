#version 330 core
out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D uFontTexture;
uniform vec4 uFontColor;

void main()
{
	FragColor = uFontColor * texture(uFontTexture, vTexCoord);
}