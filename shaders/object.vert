#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureUV;

layout (location = 0) uniform mat4 PVM;
layout (location = 1) uniform mat4 VM;
layout (location = 2) uniform mat3 transposedInvertedVM;

layout (location = 0) out vec4 fragNormalView;
layout (location = 1) out vec4 fragPositionView;
layout (location = 2) out vec2 fragTextureUV;

void main()
{
	gl_Position = PVM * vec4(position, 1.0f);
	//Multiplied by normal matrix, corrected angles even for non-uniform scaling
	fragNormalView = vec4(transposedInvertedVM * normal, 0.0);
	fragPositionView = VM * vec4(position, 1.0f);
	fragTextureUV = textureUV;
}
