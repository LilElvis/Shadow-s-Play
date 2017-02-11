#version 420

uniform sampler2D uTex;

in vec2 texcoord;

out vec4 color;

void main()
{
	color = texture(uTex, vec2(texcoord.x, 1 - texcoord.y ));
	//color = vec4(1.0, 1.0, 1.0, 1.0);
}