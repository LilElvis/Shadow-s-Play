//RAYMOND DEE - 100578667

#version 420

uniform vec4 LightPosition = vec4(0.0f, 2.0f, 0.0f, 1.0);

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

uniform vec3 uCameraPos;

uniform mat4 uinverseViewMatrix;
uniform mat4 uinversePerspectiveMatrix;

vec2 FragCoords;

struct Light
{
	vec3 position;
	vec4 color;
	float aConstant;// = 0.01;
	float aLinear;// = 0.01;
	float aQuadratic;// = 0.001875;
};

uniform Light uCurrentLight;

in vec2 texcoord;
in vec3 pos;

uniform float uTime;

out vec4 outColor;

vec4 getPosition()
{
	FragCoords = vec2(gl_FragCoord.x/1600 , gl_FragCoord.y/911);

	float depth = (texture(uDepthMap, FragCoords).r) * 2.0 - 1.0;

	vec4 clipSpace = vec4(FragCoords * 2.0 - 1.0, depth, 1.0);

	vec4 viewSpace = uinversePerspectiveMatrix * clipSpace;

	viewSpace /= viewSpace.w;

	vec4 worldSpace = uinverseViewMatrix * viewSpace;

	return worldSpace;
}

void main()
{
	FragCoords = vec2(gl_FragCoord.x/1600 , gl_FragCoord.y/911);
	
	vec4 position = getPosition();

	//Make Normals from -1 to 1 again
	vec3 normal = texture(uNormalMap, FragCoords).rgb;
	normal = normal * 2.0 - 1.0;
	
	vec3 albedo = texture(uAlbedoMap, FragCoords).rgb;

	vec3 specular = texture(uSpecularMap, FragCoords).rgb;

	vec3 emissive = texture(uEmissiveMap, FragCoords).rgb;

	vec3 outDiffuse = vec3(0.0);

	vec3 outSpecular = vec3(0.0);

	vec3 viewDirection = normalize(-position.xyz - uCameraPos);

	vec3 distanceFromLight;

	outColor.rgb = vec3(0.0, 0.0, 0.0);

	float distance = length(uCurrentLight.position.rgb - position.xyz);
	
	vec3 lightDirection = normalize(uCurrentLight.position.rgb - position.xyz);
	
	vec3 diffuseLight = max(dot(normal, lightDirection), 0.0) * uCurrentLight.color.rgb * uCurrentLight.color.a;
	
	vec3 halfVector = normalize(lightDirection + viewDirection);
	
	vec3 specLight = uCurrentLight.color.rgb * uCurrentLight.color.a * vec3(pow(max(dot(normal, halfVector), 0.0), 8.0));
	
	float attenuation = 1.0/(uCurrentLight.aConstant + uCurrentLight.aLinear * distance + uCurrentLight.aQuadratic * distance);
	
	outDiffuse += diffuseLight * attenuation;
	
	outSpecular += specLight * attenuation;
	
	distanceFromLight = position.xyz * 0.01;
	
	outColor.rgb = (texture(uAmbientMap, FragCoords).rgb + outDiffuse) * albedo;
	outColor.rgb += specular * outSpecular;
	
	outColor.a = 1.0;
}