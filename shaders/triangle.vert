#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

layout (location = 0) out vec4 vertexColor;
layout (location = 1) out vec2 vertexTexCoord;

layout (location = 0) uniform mat4 M;

void main()
{
	gl_Position = M * vec4(position, 1.0f);
	vertexColor = vec4(color, 1.0f);
	vertexTexCoord = texCoord;
}