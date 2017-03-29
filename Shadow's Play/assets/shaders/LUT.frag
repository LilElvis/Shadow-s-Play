#version 420

layout (binding = 0) uniform sampler2D uTex;
layout (binding = 1) uniform sampler3D uLUT;

in vec2 texcoord;

out vec4 color;

void main()
{
	vec4 fColor = texture(uTex, texcoord);

	vec4 newColor = texture(uLUT, fColor.xyz);

	color = newColor;
	//color.a = 1.0f;
}