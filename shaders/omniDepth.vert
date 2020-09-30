#version 460

layout (location = 0) in vec3 position;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 M;
	mat4 cofactorM; //transpose(adjugate)
};

void main()
{
	gl_Position = M * vec4(position, 1.0);
}