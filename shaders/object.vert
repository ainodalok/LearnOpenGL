#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureUV;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 PVM;
};

layout (location = 0) out vec2 fragTextureUV;

void main()
{
	gl_Position = PVM * vec4(position, 1.0f);
	fragTextureUV = textureUV;
}