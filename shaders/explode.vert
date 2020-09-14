#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 PV;
	mat4 M;
	mat4 cofactorM;
	vec3 camera;
	float time;
};

out VS_OUT
{
	vec3 normal;
} vs_out;

void main()
{
	gl_Position = M * vec4(position, 1.0);
	vs_out.normal = mat3(cofactorM) * normal;
}