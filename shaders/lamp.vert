#version 460 core
layout (location = 0) in vec3 position;

layout (location = 0) uniform mat4 PVM;

void main()
{
	gl_Position = PVM * vec4(position, 1.0f); 
}
