#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

layout (std140, binding = 0) uniform CameraUBO
{
	mat4 cameraPV;
};

layout (std140, binding = 1) uniform ObjectUBO
{
	mat4 M;
	mat4 cofactorM;
};

out VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} vs_out;

void main()
{
	vs_out.position = vec3(M * vec4(position, 1.0));
	vs_out.normal = mat3(cofactorM) * normal;
	vs_out.texCoord = texCoord;

	gl_Position = cameraPV * M * vec4(position, 1.0);
}