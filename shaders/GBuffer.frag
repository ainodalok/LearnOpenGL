#version 460

in VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} fs_in;

layout (binding = 0) uniform sampler2D diffuse;
layout (binding = 1) uniform sampler2D specular;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

void main()
{
	gPosition = fs_in.position;
	gNormal = normalize(fs_in.normal);
	gAlbedoSpec.rgb = texture(diffuse, fs_in.texCoord).rgb;
	//Specular in alpha
	gAlbedoSpec.a = texture(specular, fs_in.texCoord).r;
}