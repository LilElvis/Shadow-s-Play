#version 420


uniform vec4 LightPosition = vec4(-27.88f, 0.0f, 5.0f, 1.0);

//colors
uniform vec3 LightAmbient = vec3(0.1f);
uniform vec3 LightDiffuse = vec3(1.0f);
uniform vec3 LightSpecular = vec3(1.0f);

//scalars
uniform float LightSpecularExponent= 24.0f;
uniform float Attenuation_Constant = 0.01;
uniform float Attenuation_Linear= 0.01;
uniform float Attenuation_Quadratic= 0.001875;

//Diffuse Light (Add diffuse light reflected off the object)
uniform vec3 uDiffuseAdd = vec3(0.0f);
uniform vec3 uDiffuseMult = vec3(1.0f);

//Ambient Light (Basically brightness of the object)
uniform vec3 uAmbientAdd = vec3(0.0f);
uniform vec3 uAmbientMult = vec3(1.0f);

//Emissive Light
uniform vec3 uEmissiveAdd = vec3(0.0f);
uniform vec3 uEmissiveMult = vec3(1.0f);

uniform float uInvisible = 1.0f;

uniform sampler2D uTex;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

uniform float uTime;

out vec4 outColor;

float rand(vec2 num)
{
	return fract(sin(dot(num.x * 9934.12389791, num.y * 43269453.24123) * 6758223.23482));
}

void main()
{
	if(rand(pos.xy + uTime) >= uInvisible)
	{
		discard;
	}

	//outColor.rgb = vec3(1.0, 0.0, 1.0);
	//outColor.a = 1.0;

	//account for rasterizer interpolating
	vec3 normal = normalize(norm);
	
	vec3 lightVec = LightPosition.xyz - pos;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;
	
	float NdotL = dot(normal, lightDir);
	
	if (NdotL > 0.0)
	{
		//the light contributes to this surface
	
		//Calculate attenuation (falloff)
		float attenuation = 1.0 / (Attenuation_Linear + (Attenuation_Linear * dist) + (Attenuation_Quadratic * dist * dist));
	
		//calculate diffuse contribution
		outColor.rgb += LightDiffuse * NdotL * attenuation;
	
		//Blinn Phong half vector
		float NdotHV = max(dot(normal, normalize(lightDir + normalize(-pos))), 0.0);
		
		//calculate specular contribution
		outColor.rgb += LightSpecular * pow(NdotHV, LightSpecularExponent) * attenuation;
	}

	vec4 textureColor = texture(uTex, vec2(texcoord.x, 1.0 -texcoord.y));
	//vec4 textureColor = vec4(1.0, 1.0, 1.0, 1.0);
	textureColor.xyz = textureColor.xyz + uDiffuseAdd * uDiffuseMult;
	outColor.rgb += uAmbientAdd * uAmbientMult;
	outColor.rgb *= textureColor.rgb;
	outColor.rgb += uEmissiveAdd * uEmissiveMult;
	outColor.a = textureColor.a;
}