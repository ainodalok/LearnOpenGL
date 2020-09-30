#version 460

in VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	vec4 positionLightSpace;
} fs_in;

layout (std140, binding = 3) uniform LightBlock
{
	vec4 lightPos;
	vec4 viewPos;
};

layout (binding = 0) uniform sampler2D tex;
layout (binding = 1) uniform sampler2D shadowMap;

layout (location = 0) out vec4 fragColor;

float shadowCalculation(vec4 positionLightSpace)
{
	vec3 ndcCoords = positionLightSpace.xyz / positionLightSpace.w;
	vec3 normalizedCoords = ndcCoords * 0.5 + 0.5;
	
	if(normalizedCoords.z > 1.0)
        return 0.0;
	
	float currentLightDepth = normalizedCoords.z;

	float bias = max(0.05 * (1.0 - dot(normalize(fs_in.normal), normalize(lightPos.xyz - fs_in.position))), 0.005);  

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfClosestLightDepth = texture(shadowMap, normalizedCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentLightDepth - bias > pcfClosestLightDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	return shadow;
}

void main()
{
	vec3 color = texture(tex, fs_in.texCoord).rgb;
	vec3 lightColor = vec3(1.0);
	vec3 ambient = color * 0.3;

	vec3 lightDir = normalize(lightPos.xyz - fs_in.position);
	vec3 normal = normalize(fs_in.normal);
	float diff = max(0.0, dot(lightDir, normal));
	vec3 diffuse = lightColor * diff;

	vec3 viewDir = normalize(viewPos.xyz - fs_in.position);
	vec3 halfway = normalize(viewDir + lightDir);
	float spec = pow(max(0.0, dot(normal, halfway)), 64.0);
	vec3 specular = lightColor * spec;

	vec3 lighting = (ambient + (1.0 - shadowCalculation(fs_in.positionLightSpace)) * (diffuse + specular)) * color;

	float gamma = 2.2;
    fragColor = vec4(pow(lighting, vec3(1.0/gamma)), 1.0);
}