#version 460 core
struct PointLight		//Size 64
{						//Alignment 16
	vec4 position;		//0
	vec3 ambient;		//16
	float constant;		//28
	vec3 diffuse;		//32
	float linear;		//44
	vec3 specular;		//48
	float quadratic;	//60 - 64
};

struct DirSpotLight			//Size 112	
{							//Alignment 16
	vec3 ambientSpot;		//0
	float innerCutoffSpot;	//12
	vec3 diffuseSpot;		//16
	float outerCutoffSpot;	//28
	vec3 specularSpot;		//32
	float linearSpot;		//44
	vec3 ambientDir;		//48
	float quadraticSpot;	//60
	vec3 diffuseDir;		//64
	float constantSpot;		//76
	vec4 directionDir;		//80
    vec3 specularDir;		//96 - 108
};

#define POINT_LIGHTS_NUMBER 4 

layout (location = 0) in vec4 fragNormalView;
layout (location = 1) in vec4 fragPositionView;
layout (location = 2) in vec2 fragTextureUV;

layout (binding = 0) uniform sampler2D diffuse;
layout (binding = 1) uniform sampler2D specular;
layout (location = 3) uniform float shininess;

layout (binding = 0, std140) uniform Light
{
	PointLight pointLight[POINT_LIGHTS_NUMBER];
	DirSpotLight dirSpotLight;
};

layout (location = 0) out vec4 fragColor;

vec3 calcDirLight(DirSpotLight light, vec4 normal, vec4 viewDirection, vec3 diffuseMaterial, vec3 specularMaterial)
{
	vec3 ambient = light.ambientDir * diffuseMaterial;
	vec4 lightDirection = vec4(normalize(-light.directionDir.xyz), 0.0f);
	vec3 diffuse = light.diffuseDir * diffuseMaterial * max(dot(normal, lightDirection), 0.0f);
	vec3 specular = light.specularDir * specularMaterial * pow(max(dot(viewDirection, reflect(-lightDirection, normal)), 0.0f), shininess);

	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec4 normal, vec4 viewDirection, vec3 diffuseMaterial, vec3 specularMaterial, vec4 fragPosition)
{
	vec3 ambient = light.ambient * diffuseMaterial;
	vec4 lightDirectionFromFragment = vec4(normalize((light.position - fragPosition).xyz), 0.0f);
	vec3 diffuse = light.diffuse * diffuseMaterial * max(dot(normal, lightDirectionFromFragment), 0.0f);
	vec3 specular = light.specular * specularMaterial * pow(max(dot(viewDirection, reflect(-lightDirectionFromFragment, normal)), 0.0f), shininess);
	float distanceToLight = distance(light.position, fragPosition);
	float attenuation = 1.0f / (light.constant + light.linear * distanceToLight + light.quadratic * distanceToLight * distanceToLight);

	return (ambient + diffuse + specular) * attenuation;
}

vec3 calcSpotLight(DirSpotLight light, vec4 normal, vec4 viewDirection, vec3 diffuseMaterial, vec3 specularMaterial, vec4 fragPosition)
{
	vec3 lightDirection = vec3(0.0f, 0.0f, -1.0f);
	float theta = dot(-viewDirection.xyz, lightDirection);
	float epsilon = light.innerCutoffSpot - light.outerCutoffSpot;
	float intensity = clamp((theta - light.outerCutoffSpot) / epsilon, 0.0f, 1.0f);
	if (intensity > 0.0f)
	{
		vec3 ambient = light.ambientSpot * diffuseMaterial;
		vec3 diffuse = light.diffuseSpot * diffuseMaterial * max(dot(normal, viewDirection), 0.0f);
		vec3 specular = light.specularSpot * specularMaterial * pow(max(dot(viewDirection, reflect(-viewDirection, normal)), 0.0f), shininess);
		float distanceToLight = length(fragPosition);
		float attenuation = 1.0f / (light.constantSpot + light.linearSpot * distanceToLight + light.quadraticSpot * distanceToLight * distanceToLight);

		return (ambient + diffuse + specular) * attenuation * intensity;
	}
	else
		return vec3(0.0f);
}

void main()
{
	vec4 normal = vec4(normalize(fragNormalView.xyz), 0.0f);
	vec4 viewDirection = vec4(normalize(-fragPositionView.xyz), 0.0f);
	vec3 diffuseMaterial = vec3(texture(diffuse, fragTextureUV));
	vec3 specularMaterial = vec3(texture(specular, fragTextureUV));
	//vec3 specularMaterial = vec3(0.0f);
	vec3 result = vec3(0);
	result += calcDirLight(dirSpotLight, normal, viewDirection, diffuseMaterial, specularMaterial);
	for(int i = 0; i < POINT_LIGHTS_NUMBER; i++)
		result += calcPointLight(pointLight[i], normal, viewDirection, diffuseMaterial, specularMaterial, fragPositionView);
	result += calcSpotLight(dirSpotLight, normal, viewDirection, diffuseMaterial, specularMaterial, fragPositionView);
	fragColor = vec4(result, 1.0f);
}