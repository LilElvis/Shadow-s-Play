#version 420

uniform sampler2D uScene;

uniform sampler2D uBloomScene;

in vec2 texcoord;

out vec3 outColor;

void main()
{
	outColor = (1.0 - (1.0 - texture(uScene, texcoord).rgb) * (1.0 - texture(uBloomScene, texcoord).rgb));
}