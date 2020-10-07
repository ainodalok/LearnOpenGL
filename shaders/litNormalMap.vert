#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

layout (std140, binding = 0) uniform ObjectUBO
{
	mat4 M;
	mat4 cofactorM;
};

layout (std140, binding = 2) uniform CameraUBO
{
	mat4 cameraPV;
};

layout (std140, binding = 3) uniform LightUBO
{
	vec4 viewPos;
	vec3 lightPos;
};

out VS_OUT
{
	vec2 texCoord;
	vec3 tangentLightPos;
	vec3 tangentViewPos;
	vec3 tangentPosition;
	vec3 normal;
} vs_out;

void main()
{
	vec4 Mposition = M * vec4(position, 1.0);

	vec3 T = normalize(mat3(cofactorM) * tangent);
	vec3 B = normalize(mat3(cofactorM) * bitangent);
	vec3 N = normalize(mat3(cofactorM) * normal);
	//Fix for mirrored UVs
	if (dot(cross(T, B), N) < 0.0)
		T *= -1.0;
	mat3 TBN = mat3(T, B, N);
	TBN = transpose(TBN);

	vs_out.texCoord = texCoord;
	vs_out.tangentLightPos = TBN * lightPos;
    vs_out.tangentViewPos  = TBN * viewPos.xyz;
    vs_out.tangentPosition  = TBN * Mposition.xyz;
	vs_out.normal = TBN*N;
	gl_Position = cameraPV * Mposition;
}