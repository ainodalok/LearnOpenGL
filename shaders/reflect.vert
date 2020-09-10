#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 PVM;
	mat4 M;
	mat4 transposedInvertedM;
	vec3 camera;
};

layout (location = 0) out vec3 fragPostionModel;
layout (location = 1) out vec3 fragNormalModel;

void main()
{
	gl_Position = PVM * vec4(position, 1.0f);
	fragNormalModel = mat3(transposedInvertedM) * normal;
	fragPostionModel = vec3(M * vec4(position, 1.0));
}