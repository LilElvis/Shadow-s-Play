#version 420

uniform mat4 uModel = mat4(1.0);
uniform mat4 uView = mat4(1.0);
uniform mat4 uProj = mat4(1.0);

layout(location = 0) in vec3 in_vert;
//layout(location = 4) in vec3 in_vert2;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

//uniform float uTime;

out vec2 texcoord;
out vec3 norm;
out vec3 pos;

void main()
{
	texcoord = in_uv;
	norm = mat3(uView) * mat3(uModel) * in_normal;

	vec4 viewSpace = (uView * uModel * vec4(in_vert, 1.0f));
	//vec4 viewSpace = (uView * uModel * vec4(mix(in_vert, in_vert2, uTime), 1.0f));

	gl_Position = uProj * viewSpace;

	pos = viewSpace.xyz;
}