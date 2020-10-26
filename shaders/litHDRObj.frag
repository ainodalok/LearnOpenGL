#version 460

in VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} fs_in;

layout (std140, binding = 2) uniform LightUBO
{
	vec4 viewPos;
	vec4 lightPos[4];
	vec4 lightCol[4];
};

layout (binding = 0) uniform sampler2D diffuse;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

void main()
{
	vec3 color = texture(diffuse, fs_in.texCoord).xyz;
	vec3 normal = normalize(fs_in.normal);

	vec3 ambient = 0.02 * color;

	vec3 lighting = vec3(0.0);
	for (int i = 0; i < 4; i++)
	{
		vec3 lightDir = normalize(lightPos[i].xyz - fs_in.position);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = lightCol[i].xyz * diff * color;
		vec3 result = diffuse;

		float dist = length(fs_in.position - lightPos[i].xyz);
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