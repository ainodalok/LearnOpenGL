#version 460 core

layout (location = 0) in vec4 vertexColor;
layout (location = 1) in vec2 vertexTexCoord;

layout (location = 0) out vec4 fragColor;

layout (binding = 0) uniform sampler2D triangleTex0;
layout (binding = 1) uniform sampler2D triangleTex1;

void main()
{
    fragColor = mix(texture(triangleTex0, vertexTexCoord), texture(triangleTex1, vertexTexCoord), 0.2);
}