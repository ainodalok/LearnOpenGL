#version 460

layout (location = 0) out vec2 texCoord;

void main()
{
	texCoord = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
    gl_Position = vec4(texCoord * 2.0f + -1.0f, 0.0f, 1.0f);
}