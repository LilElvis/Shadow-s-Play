#version 420

layout(binding = 0) uniform sampler2D u_texA;
layout(binding = 1) uniform sampler2D u_texB;

in vec2 texcoord;

out vec4 color;

void main()
{
	vec4 texA = texture(u_texA, texcoord.xy);
	vec4 texB = texture(u_texB, texcoord.xy);

	color = min(texA, texB);

	color.a - 1.0;
}