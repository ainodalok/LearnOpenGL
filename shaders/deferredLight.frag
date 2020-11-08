#version 460

layout (location = 0) in vec2 texCoord;

layout (std140, binding = 2) uniform LightUBO
{
	vec4 viewPos;
	vec4 lightPos[32];
	vec4 lightCol[32];
};

layout (binding = 0) uniform sampler2D gPosition;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D gAlbedoSpec;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

void main()
{
	vec3 position = texture(gPosition, texCoord).rgb;
	vec3 normal = normalize(texture(gNormal, texCoord).rgb);
	vec4 albedoSpec = texture(gAlbedoSpec, texCoord);
	vec3 albedo = albedoSpec.rgb;
	float specular = albedoSpec.a;
	
	vec3 ambient = 0.02 * albedo;

	vec3 lighting = vec3(0.0);
	for (int i = 0; i < 32; i++)
	{
		vec3 lightDir = normalize(lightPos[i].xyz - position);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = lightCol[i].xyz * diff * albedo;
		vec3 result = diffuse;

		float dist = length(position - lightPos[i].xyz);
		result *= 1.0 / (dist * dist);
		lighting += result;
	}
	vec3 result = ambient + lighting;
	float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0)
		brightColor = vec4(result, 1.0);
	else
		brightColor = vec4(0.0, 0.0, 0.0, 1.0);

	fragColor = vec4(result, 1.0);
}

