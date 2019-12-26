#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 0) uniform mat4 PVM;
layout (location = 1) uniform mat4 VM;
layout (location = 2) uniform mat3 transposedInvertedVM;

layout (location = 0) out vec3 vertexNormalView;
layout (location = 1) out vec3 vertexPositionView;

void main()
{
	gl_Position = PVM * vec4(position, 1.0f);
	//Multiplied by normal matrix, corrected angles even for non-uniform scaling
	vertexNormalView = transposedInvertedVM * normal;
	vertexPositionView = vec3(VM * vec4(position, 1.0f));
}
