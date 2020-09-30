#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 M;
	mat4 cofactorM; //transpose(adjugate)
};

layout (std140, binding = 1) uniform DirectShadowUBO
{
	mat4 PV;
};

layout (std140, binding = 2) uniform CameraUBO
{
	mat4 cameraPV;
};

out VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	vec4 positionLightSpace;
} vs_out;

void main()
{
	vs_out.position = vec3(M * vec4(position, 1.0));
	vs_out.normal = mat3(cofactorM) * normal;
	vs_out.texCoord = texCoord;
	vs_out.positionLightSpace = PV * vec4(vs_out.position, 1.0);
	gl_Position = cameraPV * vec4(vs_out.position, 1.0);
}