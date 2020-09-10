#version 460

layout (location = 0) in vec3 fragPostionModel;
layout (location = 1) in vec3 fragNormalModel;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 PVM;
	mat4 M;
	mat4 cofactorM;
	vec3 camera;
};

layout (binding = 0) uniform samplerCube tex;

layout (location = 0) out vec4 fragColor;

void main()
{
	//Diamond refractive index
	float ratio = 1.0 / 2.42;
	vec3 incoming = normalize(fragPostionModel - camera);
	vec3 refracted = refract(incoming, normalize(fragNormalModel), ratio);
	fragColor = vec4(texture(tex, normalize(refracted)).rgb, 1.0);
}