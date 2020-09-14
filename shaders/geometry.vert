#version 460

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out VS_OUT
{
	vec3 color;
} vs_out;

void main()
{
	//gl_PointSize = 20.0f;
	gl_Position = vec4(position, 0.0, 1.0);
	vs_out.color = color;
}