#version 420

uniform sampler2D uTex;

in vec2 texcoord;

out vec4 color;

void main()
{
	color = texture(uTex, vec2(texcoord.x, texcoord.y));
	color.a = 1.0f;
}