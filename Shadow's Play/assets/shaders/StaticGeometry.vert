#version 420

uniform mat4 uModel = mat4(1.0);
uniform mat4 uView = mat4(1.0);
uniform mat4 uProj = mat4(1.0);

uniform vec3 uScale = vec3(1.0f, 5.0f, 1.0f);

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec3 in_biTangent;

//uniform float uTime;

out vec2 texcoord;
out vec3 norm;
out vec3 pos;
out vec3 tang;
out vec3 biTang;

void main()
{
	texcoord = in_uv;

	pos = (uModel * vec4(in_vert,1)).xyz;

	norm = mat3(uModel) * in_normal;

	tang = mat3(uModel) * in_tangent;

	biTang = mat3(uModel) * in_biTangent;

	gl_Position = uProj * uView * uModel * vec4(in_vert * uScale,1);
}