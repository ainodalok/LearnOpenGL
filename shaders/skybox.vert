#version 460
layout (location = 0) in vec3 position;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 PV;
};

layout (location = 0) out vec3 fragTextureUV;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    fragTextureUV = position;
    gl_Position = (PV * vec4(position, 1.0)).xyww;
}