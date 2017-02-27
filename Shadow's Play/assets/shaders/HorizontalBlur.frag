#version 420

uniform sampler2D uScene;

uniform float texelWidth;

in vec2 texcoord;

out vec3 outColor;

void main()
{
	outColor =  texture(uScene, texcoord + vec2(texelWidth * 4.0, 0.0)).rgb * 0.06;
	outColor += texture(uScene, texcoord + vec2(texelWidth * 3.0, 0.0)).rgb * 0.09;
	outColor += texture(uScene, texcoord + vec2(texelWidth * 2.0, 0.0)).rgb * 0.12;
	outColor += texture(uScene, texcoord + vec2(texelWidth,		  0.0)).rgb * 0.15;
	outColor += texture(uScene, texcoord							  ).rgb * 0.16;
	outColor += texture(uScene, texcoord - vec2(texelWidth,		  0.0)).rgb * 0.15;
	outColor += texture(uScene, texcoord - vec2(texelWidth * 2.0, 0.0)).rgb * 0.12;
	outColor += texture(uScene, texcoord - vec2(texelWidth * 3.0, 0.0)).rgb * 0.09;
	outColor += texture(uScene, texcoord - vec2(texelWidth * 4.0, 0.0)).rgb * 0.06;
}