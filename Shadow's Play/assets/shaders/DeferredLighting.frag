#version 420

uniform vec4 LightPosition = vec4(-27.88f, 0.0f, 5.0f, 1.0);

//colors
uniform vec3 LightAmbient = vec3(0.1f);
uniform vec3 LightDiffuse = vec3(1.0f);
uniform vec3 LightSpecular = vec3(1.0f);

//Sample
layout(binding = 0) uniform sampler2D uAlbedoMap;
layout(binding = 1) uniform sampler2D uNormalMap;
layout(binding = 2) uniform sampler2D uSpecularMap;
layout(binding = 3) uniform sampler2D uEmissiveMap;
layout(binding = 4) uniform sampler2D uAmbientMap;
layout(binding = 5) uniform sampler2D uDepthMap;

uniform vec3 cameraPos;
//uniform mat4 uProjectionBiasMatrixInverse;

uniform mat4 uinverseViewMatrix;
uniform mat4 uinversePerspectiveMatrix;

struct Light
{
	vec3 position;
	vec4 color;
	float aConstant;// = 0.01;
	float aLinear;// = 0.01;
	float aQuadratic;// = 0.001875;
};

const int maxNumberOfLights = 64;

uniform int uNumLights = 1;

uniform Light lights[maxNumberOfLights];

in vec2 texcoord;
in vec3 pos;

uniform float uTime;

out vec4 outColor;

vec4 getPosition()
{
	float z = (texture(uDepthMap, texcoord).r) * 2.0 - 1.0;

	vec4 clipSpace = vec4(texcoord * 2.0 - 1.0, z, 1.0);

	vec4 viewSpace = uinversePerspectiveMatrix * clipSpace;

	viewSpace /= viewSpace.w;

	vec4 worldSpace = uinverseViewMatrix * viewSpace;

	return worldSpace;
}

void main()
{
	float depth = texture(uDepthMap, texcoord).r;

	vec4 position = getPosition();
	
	//Make Normals from -1 to 1 again
	vec3 normal = texture(uNormalMap, texcoord).rgb;
	normal = normal * 2.0 - 1.0;
	
	vec3 albedo = texture(uAlbedoMap, texcoord).rgb;

	vec3 specular = texture(uSpecularMap, texcoord).rgb;

	vec3 emissive = texture(uEmissiveMap, texcoord).rgb;

	vec3 outDiffuse = vec3(0.0);

	vec3 outSpecular = vec3(0.0);

	vec3 viewDirection = normalize(-position.xyz - cameraPos);

	vec3 distanceFromLight;

	outColor.rgb = vec3(0,0,0);

	for (int i = 0; i < uNumLights; ++i)
	{
		float distance = length(lights[i].position.rgb - position.xyz);
		
		vec3 lightDirection = normalize(lights[i].position.rgb - position.xyz);
		
		vec3 diffuseLight = max(dot(normal, lightDirection), 0.0) * lights[i].color.rgb * lights[i].color.a;
		
		vec3 halfVector = normalize(lightDirection + viewDirection);
		
		vec3 specLight = lights[i].color.rgb * lights[i].color.a * vec3(pow(max(dot(normal, halfVector), 0.0), 8.0));
		
		float attenuation = 1.0/(lights[i].aConstant + lights[i].aLinear * distance + lights[i].aQuadratic * distance);
		
		outDiffuse += diffuseLight * attenuation;
		
		outSpecular += specLight * attenuation;
		
		distanceFromLight = position.xyz * 0.01;
	}


	outColor.rgb = (texture(uAmbientMap, texcoord).rgb + outDiffuse) * albedo;
	outColor.rgb += specular * outSpecular;
	outColor.rgb += emissive;

	outColor.a = 1.0;
}