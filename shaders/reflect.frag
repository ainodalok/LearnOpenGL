#version 460

layout (location = 0) in vec3 fragPostionModel;
layout (location = 1) in vec3 fragNormalModel;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 PVM;
	mat4 M;
	mat4 transposedInvertedM;
	vec3 camera;
};

layout (binding = 0) uniform samplerCube tex;

layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 incoming = normalize(fragPostionModel - camera);
	vec3 reflected = reflect(incoming, normalize(fragNormalModel));
	fragColor = vec4(texture(tex, reflected).rgb, 1.0);
}