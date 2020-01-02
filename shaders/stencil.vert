#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 0) uniform mat4 PVM;

void main()
{
	gl_Position = PVM * vec4(position + normal * 0.06, 1.0f); 
}
