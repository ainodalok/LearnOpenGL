#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureUV;
layout (location = 3) in mat4 instanceM;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 PV;
    mat4 rot;
};

layout (location = 0) out vec2 fragTextureUV;

void main()
{
    fragTextureUV = textureUV;
    gl_Position = PV * instanceM * rot * vec4(position, 1.0f); 
}