#version 420

uniform sampler2D uTex;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

uniform float uTime;

uniform vec2 uvOffset;

out vec4 outColor;

void main()
{
	//account for rasterizer interpolating
	vec3 normal = normalize(norm);

	vec4 textureColor = texture(uTex, texcoord + uvOffset);
	
	outColor.rgb = textureColor.rgb;
	outColor.a = textureColor.a;
	//outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}