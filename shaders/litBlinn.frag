#version 460

in VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} fs_in;

layout (std140, binding = 1) uniform LightBlock
{
	vec4 lightPos;
	vec4 viewPos;
};

layout (binding = 0) uniform sampler2D tex;

layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 color = texture(tex, fs_in.texCoord).rgb;

	vec3 ambient = color * 0.05;

	vec3 lightDir = normalize(lightPos.xyz - fs_in.position);
	vec3 normal = normalize(fs_in.normal);
	float diff = max(0.0, dot(lightDir, normal));
	vec3 diffuse = color * diff;

	vec3 viewDir = normalize(viewPos.xyz - fs_in.position);
	vec3 halfway = normalize(viewDir + lightDir);
	float spec = pow(max(0.0, dot(normal, halfway)), 32.0);
	vec3 specular = vec3(0.3) * spec;

	float dist = length(lightPos.xyz - fs_in.position);
	//dist = min(1.5, dist);
	float attenuation = 1.0 / (dist * dist);

	fragColor = vec4((ambient + diffuse + specular) * attenuation, 1.0);

	float gamma = 2.2;
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}