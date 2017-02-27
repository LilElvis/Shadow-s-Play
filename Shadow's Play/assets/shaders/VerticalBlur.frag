#version 420

uniform sampler2D uScene;

uniform float texelHeight;

in vec2 texcoord;

out vec3 outColor;

void main()
{
	outColor =  texture(uScene, texcoord + vec2(0.0 ,texelHeight * 4.0)).rgb * 0.06;
	outColor += texture(uScene, texcoord + vec2(0.0 ,texelHeight * 3.0)).rgb * 0.09;
	outColor += texture(uScene, texcoord + vec2(0.0 ,texelHeight * 2.0)).rgb * 0.12;
	outColor += texture(uScene, texcoord + vec2(0.0,	   texelHeight)).rgb * 0.15;
	outColor += texture(uScene, texcoord							   ).rgb * 0.16;
	outColor += texture(uScene, texcoord - vec2(0.0,	   texelHeight)).rgb * 0.15;
	outColor += texture(uScene, texcoord - vec2(0.0, texelHeight * 2.0)).rgb * 0.12;
	outColor += texture(uScene, texcoord - vec2(0.0, texelHeight * 3.0)).rgb * 0.09;
	outColor += texture(uScene, texcoord - vec2(0.0, texelHeight * 4.0)).rgb * 0.06;
}