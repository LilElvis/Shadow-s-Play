#version 420

uniform sampler2D uScene;

in vec2 texcoord;

out vec3 outColor;

void main()
{
	vec3 col = texture(uScene, texcoord).rgb;

	//col += (col - 1.0);

	float luminence = (col.r * 0.3 + col.g * 0.59 + col.b * 0.11);
	luminence = luminence + luminence - 1.0f;
	outColor = col * luminence;
}