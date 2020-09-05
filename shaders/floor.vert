#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureUV;

layout (std140, binding = 0) uniform FloorBlock
{
	mat4 PVM;
	vec3 pos;
};

layout (location = 0) out vec2 fragTextureUV;

void main()
{
	gl_Position = PVM * vec4(position, 1.0f);
	float speedCoef = 10.0f/20.0f;
	fragTextureUV = textureUV + vec2(pos.x, -pos.z) * speedCoef;
}