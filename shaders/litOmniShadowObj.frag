#version 460

in VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} fs_in;

layout (std140, binding = 2) uniform LightBlock
{
	vec4 viewPos;
    vec3 lightPos;
    float farPlane;
};

layout (binding = 0) uniform sampler2D tex;
layout (binding = 1) uniform samplerCube shadowMap;

layout (location = 0) out vec4 fragColor;

float shadowCalculation(vec3 fragmentPosition)
{
	vec3 fragToLight = fs_in.position - lightPos;
	float currentLightDepth = length(fragToLight);
	
	//float bias = max(0.05 * (1.0 - dot(normalize(fs_in.normal), normalize(lightPos.xyz - fs_in.position))), 0.005);  
	vec3 sampleOffsetDirections[20] = vec3[]
	(
	   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
	   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
	   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
	   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
	   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
	);
	float shadow = 0.0;
	float bias   = 0.15;
	int samples  = 20;
	float viewDistance = length(viewPos.xyz - fs_in.position);
	float diskRadius = (1.0 + (viewDistance / farPlane)) / 25.0;
	for(int i = 0; i < samples; ++i)
	{
		float closestLighDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestLighDepth *= farPlane;   // undo mapping [0;1]
		if(currentLightDepth - bias > closestLighDepth)
			shadow += 1.0;
	}
	shadow /= samples;

	return shadow;
}

void main()
{
	vec3 color = texture(tex, fs_in.texCoord).rgb;
	vec3 lightColor = vec3(0.3);
	vec3 ambient = color * 0.3;

	vec3 lightDir = normalize(lightPos.xyz - fs_in.position);
	vec3 normal = normalize(fs_in.normal);
	float diff = max(0.0, dot(lightDir, normal));
	vec3 diffuse = lightColor * diff;

	vec3 viewDir = normalize(viewPos.xyz - fs_in.position);
	vec3 halfway = normalize(viewDir + lightDir);
	float spec = pow(max(0.0, dot(normal, halfway)), 64.0);
	vec3 specular = lightColor * spec;

	vec3 lighting = (ambient + (1.0 - shadowCalculation(fs_in.position)) * (diffuse + specular)) * color;

	float gamma = 2.2;
    fragColor = vec4(pow(lighting, vec3(1.0/gamma)), 1.0);
}