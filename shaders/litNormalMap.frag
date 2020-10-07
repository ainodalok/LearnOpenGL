#version 460

in VS_OUT
{
	vec2 texCoord;
	vec3 tangentLightPos;
	vec3 tangentViewPos;
	vec3 tangentPosition;
	vec3 normal;
} fs_in;

layout (std140, binding = 1) uniform MaterialUBO
{
	int diffuseUsed;
	int specularUsed;
	int normalMapUsed;
	int heightMapUsed;
};

layout (binding = 0) uniform sampler2D diffuse;
layout (binding = 1) uniform sampler2D specular;
layout (binding = 2) uniform sampler2D normalMap;

layout (location = 0) out vec4 fragColor;

void main()
{
    vec3 normal = fs_in.normal; 
	if (normalMapUsed == 1)
	{	
		normal = texture(normalMap, fs_in.texCoord).rgb;
		normal = normalize(normal * 2.0 - 1.0);
	}

	vec3 diffuseColor = vec3(0.0f, 1.0f, 0.0f);
	if (diffuseUsed == 1)
		diffuseColor = texture(diffuse, fs_in.texCoord).rgb;

	vec3 ambientOut = diffuseColor * 0.1;

	vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.tangentPosition);
	float diff = max(0.0, dot(lightDir, normal));
	vec3 diffuseOut = diffuseColor * diff;

	vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentPosition);
	vec3 halfway = normalize(viewDir + lightDir);
	float spec = pow(max(0.0, dot(normal, halfway)), 96.078431);
	vec3 specularColor = vec3(0.0);
	if (specularUsed == 1)
		specularColor = texture(specular, fs_in.texCoord).rgb;
	vec3 specularOut = specularColor * spec * 0.5;

	float dist = length(fs_in.tangentLightPos - fs_in.tangentPosition);
	//dist = min(1.5, dist);
	float attenuation = 1.0;// / (dist * dist);

	fragColor = vec4((ambientOut + diffuseOut + specularOut) * attenuation, 1.0);

	float gamma = 2.2;
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}