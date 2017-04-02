//RAYMOND DEE - 100578667

#version 420

//Colors
uniform vec3 LightAmbient = vec3(0.1f);

//Aspect Ratio
uniform int uWindowWidth = 1600;
uniform int uWindowHeight = 911;

//Sample
layout(binding = 0) uniform sampler2D uAlbedoMap;
layout(binding = 1) uniform sampler2D uLightSamples;

layout(binding = 2) uniform sampler2D uEmissiveMap;

in vec2 texcoord;

out vec4 outColor;

void main()
{
	vec2 FragCoords = vec2(gl_FragCoord.x/uWindowWidth , gl_FragCoord.y/uWindowHeight);
	
	outColor.rgb = vec3(0.0, 0.0, 0.0);

	vec3 emissive = texture(uEmissiveMap, FragCoords).rgb;
	
	vec3 albedo = (texture(uAlbedoMap, texcoord).rgb * LightAmbient);

	vec3 lightSamples = texture(uLightSamples, texcoord).rgb;

	outColor.rgb = albedo + lightSamples;
	outColor.rgb += emissive;

	outColor.a = 1.0;
}