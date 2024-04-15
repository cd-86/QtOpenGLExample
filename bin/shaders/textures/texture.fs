#version 330 core
out vec4 FragColor;

in vec2 vTexCoord;

// texture samplers
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(uTexture1, vTexCoord), texture(uTexture2, vTexCoord), 0.2);
}