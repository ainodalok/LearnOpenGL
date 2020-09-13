#version 460

layout (location = 0) in vec2 position;

void main()
{
	//gl_PointSize = 20.0f;
	gl_Position = vec4(position, 0.0, 1.0);
}