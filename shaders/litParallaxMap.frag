#version 460

in VS_OUT
{
	vec2 texCoord;
	vec3 tangentLightPos;
	vec3 tangentViewPos;
	vec3 tangentPosition;
	vec3 normal;
} fs_in;

layout (binding = 0) uniform sampler2D diffuse;
layout (binding = 1) uniform sampler2D normalMap;
layout (binding = 2) uniform sampler2D heightMap;

layout (location = 0) uniform float heightScale;

layout (location = 0) out vec4 fragColor;

vec2 parallaxMapping(vec2 texCoord, vec3 viewDir)
{
	//const float heightScale = 0.5;
	const float numLayers = mix(heightScale * 500.0, 1.0, max(dot(vec3(0.0, 0.0, 1.0), viewDir) - 0.35, 0.0));
	//const float numLayers = heightScale * 500.0;
	const float layerDepth = 1.0 / numLayers;
	float currentLayerDepth = 0.0;
	vec2 P = viewDir.xy * heightScale;
	vec2 deltaTexCoord = P / numLayers;

	vec2 currentTexCoord = texCoord;
	float currentHeightMapValue = texture(heightMap, currentTexCoord).r;

	while(currentLayerDepth < currentHeightMapValue)
	{
		currentTexCoord -= deltaTexCoord;
		currentHeightMapValue = texture(heightMap, currentTexCoord).r;
		currentLayerDepth += layerDepth;
	}

	vec2 prevTexCoord = currentTexCoord + deltaTexCoord;
	float afterDepth = currentHeightMapValue - currentLayerDepth;
	float beforeDepth = texture(heightMap, prevTexCoord).r - currentLayerDepth + layerDepth;
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoord = prevTexCoord * weight + currentTexCoord * (1.0 - weight);

	return finalTexCoord;
}

void main()
{
	vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentPosition);
	vec2 texCoord = fs_in.texCoord;
	texCoord = parallaxMapping(fs_in.texCoord, viewDir);
	if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
		discard;

    vec3 normal = texture(normalMap, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 diffuseColor = texture(diffuse, texCoord).rgb;

	vec3 ambientOut = diffuseColor * 0.1;

	vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.tangentPosition);
	float diff = max(0.0, dot(lightDir, normal));
	vec3 diffuseOut = diffuseColor * diff;

	vec3 halfway = normalize(viewDir + lightDir);
	float spec = pow(max(0.0, dot(normal, halfway)), 32.0);
	vec3 specularOut = spec * vec3(0.2);

	float dist = length(fs_in.tangentLightPos - fs_in.tangentPosition);
	dist = min(1.5, dist);
	float attenuation = 1.0;// / (dist * dist);

	fragColor = vec4((diffuseOut + specularOut + ambientOut) * attenuation, 1.0);

	float gamma = 2.2;
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}