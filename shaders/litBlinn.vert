#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

layout (std140, binding = 0) uniform FloorBlock
{
	mat4 PV;
};

out VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} vs_out;

void main()
{
	vs_out.position = position;
	vs_out.normal = normal;
	vs_out.texCoord = texCoord;
	gl_Position = PV * vec4(position, 1.0);
}