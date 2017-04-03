#version 420

layout(binding = 0) uniform sampler2D u_depth;
layout(binding = 1) uniform sampler2D u_tex;
uniform vec2 u_cameraParam;

in vec2 texcoord;

out vec4 color;

const int numSample = 64;

void offsetCalculation(float angle, out vec2 offsets[numSample])
{
	float radius = 0.5;

	float aspectRatio = u_cameraParam.y;

	vec2 pt = vec2(radius * cos(angle), radius * sin(angle));

	pt.x /= aspectRatio;

	for (int i = 0; i < numSample; i++)
	{
		float t = i / (numSample - 1.0);
		offsets[i] = mix(-pt, pt, t);
	}
}

vec4 bokeh(vec2 offsets[numSample])
{
	float bleedBias = 2.0;
	float bleedMult = 30.0;

	vec4 centerPixel = texture(u_tex, texcoord.xy);
	float centerDepth = texture(u_depth, texcoord.xy).r;

	vec4 colour = vec4(0.0);
	float totalWeight = 0.0;

	for (int i = 0; i < numSample; i++)
	{
		vec2 offset = offsets[i];

		vec2 sampleCoords = texcoord.xy + offset * centerPixel.a;

		vec4 samplePixel = texture(u_tex, sampleCoords);
		float sampleDepth = texture(u_depth, sampleCoords).r;

		float weight = sampleDepth < centerDepth ? samplePixel.a * bleedMult : 1.0f;
		weight = (centerPixel.a > samplePixel.a + bleedBias) ? weight : 1.0f;
		weight = clamp(weight, 0.0, 1.0);
		
		color += samplePixel * weight;
		totalWeight += weight;
	}

	return color/totalWeight;
}

void main()
{
	float angle = u_cameraParam.x;

	vec2 offsets[numSample];
	offsetCalculation(angle, offsets);

	color = bokeh(offsets);
}
