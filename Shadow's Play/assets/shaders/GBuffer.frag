#version 420

layout (binding = 0) uniform sampler2D uAlbedoTex;
layout (binding = 1) uniform sampler2D uNormalTex;
layout (binding = 2) uniform sampler2D uSpecularTex;
layout (binding = 3) uniform sampler2D uEmissiveTex;

//Diffuse Light (Add diffuse light reflected off the object)
uniform vec3 uDiffuseAdd = vec3(0.0f);
uniform vec3 uDiffuseMult = vec3(1.0f);

//Ambient Light (Basically brightness of the object)
uniform vec3 uAmbientAdd = vec3(0.0f);
uniform vec3 uAmbientMult = vec3(1.0f);

//Specular Light
uniform vec3 uSpecularAdd = vec3(0.0f);
uniform vec3 uSpecularMult = vec3(1.0f);

//Emissive Light
uniform vec3 uEmissiveAdd = vec3(0.0f);
uniform vec3 uEmissiveMult = vec3(1.0f);

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outSpecular;
layout (location = 3) out vec3 outEmissive;
layout (location = 4) out vec3 outAmbient;

void main()
{
	vec3 normal = normalize(norm);

	outColor.rgb = (texture(uAlbedoTex, texcoord).rgb + uDiffuseAdd) * uDiffuseMult;

	//Make normals from 0 to 1 for optimization
	outNormal.rgb = (normal * 0.5 + 0.5);

	outAmbient.rgb = uAmbientAdd * uAmbientMult;

	outSpecular.rgb = (texture(uSpecularTex, texcoord).rgb + uSpecularAdd) * uSpecularMult;

	outEmissive.rgb = (texture(uEmissiveTex, texcoord).rgb + uEmissiveAdd) * uEmissiveMult;

	outColor.a = 1.0;
}