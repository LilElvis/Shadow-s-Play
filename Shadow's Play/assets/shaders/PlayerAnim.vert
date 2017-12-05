#version 420

uniform mat4 uModel = mat4(1.0);
uniform mat4 uView = mat4(1.0);
uniform mat4 uProj = mat4(1.0);

layout(location = 0) in vec3 in_vertA;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normalA;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec3 in_biTangent;
layout(location = 5) in vec3 in_vertB;
layout(location = 6) in vec3 in_normalB;

//uniform float uTime;
uniform float uInterpolationParameter;

out vec2 texcoord;
out vec3 norm;
out vec3 pos;
out vec3 tang;
out vec3 biTang;

void main()
{
	texcoord = in_uv;

	pos = (uModel * vec4(mix(in_vertA, in_vertB, uInterpolationParameter),1)).xyz;

	norm = mat3(uModel) * mix(in_normalA, in_normalB, uInterpolationParameter);

	tang = mat3(uModel) * in_tangent;

	biTang = mat3(uModel) * in_biTangent;

	gl_Position = uProj * uView * uModel * vec4(mix(in_vertA, in_vertB, uInterpolationParameter),1);
}