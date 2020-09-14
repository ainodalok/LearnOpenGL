#version 460

in GS_OUT
{
    vec3 position;
    vec3 normal;
} fs_in;

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 PV;
	mat4 M;
	mat4 cofactorM;
	vec3 camera;
    float time;
};

layout (binding = 0) uniform samplerCube tex;

layout (location = 0) out vec4 fragColor;

void main()
{
	//Diamond refractive index
	float ratio = 1.0 / 2.42;
	vec3 incoming = normalize(fs_in.position - camera);
	vec3 refracted = refract(incoming, normalize(fs_in.normal), ratio);
	fragColor = vec4(texture(tex, normalize(refracted)).rgb, 1.0);
}