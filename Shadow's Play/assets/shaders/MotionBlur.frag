#version 420

layout(binding = 0) uniform sampler2D uTex;
layout(binding = 1) uniform sampler2D uPrevScene;
uniform float uBlurAmount = 0.8f;

in vec2 texcoord;

out vec4 color;

void main()
{
	color = mix(texture(uTex, texcoord), texture(uPrevScene, texcoord), uBlurAmount);
	color.a = 1.0f;
}