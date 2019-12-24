#version 460 core
layout (location = 0) out vec4 fragColor;

layout (location = 3) uniform vec3 objectColor;
layout (location = 4) uniform vec3 lightColor;

void main()
{
	fragColor = vec4(lightColor * objectColor, 1.0f);
}