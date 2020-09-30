#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 M;
	mat4 cofactorM; //transpose(adjugate)
};

layout (std140, binding = 1) uniform CameraUBO
{
	mat4 cameraPV;
};

out VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} vs_out;

void main()
{
	vec4 Mposition = M * vec4(position, 1.0);

	vs_out.position = vec3(Mposition);
	vs_out.normal = mat3(cofactorM) * normal;
	vs_out.texCoord = texCoord;
	gl_Position = cameraPV * Mposition;
}