#version 420

layout (binding = 0) uniform sampler2D uTex;
layout (binding = 1) uniform sampler3D uLUT;

uniform float t = 1.0;

in vec2 texcoord;

out vec4 color;

void main()
{
	vec4 fColor = texture(uTex, texcoord);

	vec4 newColor = mix(fColor, texture(uLUT, fColor.xyz), t);

	color = newColor;
	//color.a = 1.0f;
}