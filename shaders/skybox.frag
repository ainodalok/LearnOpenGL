#version 460

layout (location = 0) in vec3 fragTextureUV;

layout (binding = 0) uniform samplerCube skybox;

layout (location = 0) out vec4 fragColor;

void main()
{    
    fragColor = texture(skybox, fragTextureUV);
}