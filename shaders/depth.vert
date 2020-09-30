#version 460

layout (location = 0) in vec3 position;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 M;
	mat4 cofactorM; //transpose(adjugate)
};

layout (std140, binding = 1) uniform directShadowUBO
{
	mat4 PV;
};

void main()
{
	gl_Position = PV * M * vec4(position, 1.0);
}