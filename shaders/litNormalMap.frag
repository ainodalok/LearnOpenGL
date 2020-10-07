#version 460

in VS_OUT
{
	vec2 texCoord;
	vec3 tangentLightPos;
	vec3 tangentViewPos;
	vec3 tangentPosition;
} fs_in;

layout (binding = 0) uniform sampler2D diffuse;
layout (binding = 1) uniform sampler2D normal;

layout (location = 0) out vec4 fragColor;

void main()
{
    vec3 normal = texture(normal, fs_in.texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);

	vec3 color = texture(diffuse, fs_in.texCoord).rgb;

	vec3 ambient = color * 0.1;

	vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.tangentPosition);
	float diff = max(0.0, dot(lightDir, normal));
	vec3 diffuse = color * diff;

	vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentPosition);
	vec3 halfway = normalize(viewDir + lightDir);
	float spec = pow(max(0.0, dot(normal, halfway)), 32.0);
	vec3 specular = vec3(0.2) * spec;

	float dist = length(fs_in.tangentLightPos - fs_in.tangentPosition);
	//dist = min(1.5, dist);
	float attenuation = 1.0 / (dist * dist);

	fragColor = vec4((ambient + diffuse + specular) * attenuation, 1.0);

	float gamma = 2.2;
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}