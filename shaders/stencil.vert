#version 460 core
layout (location = 0) in vec3 position;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 PVM;
};

void main()
{
	gl_Position = PVM * vec4(position, 1.0f); 
}
