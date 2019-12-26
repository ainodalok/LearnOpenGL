#version 460 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec4 lightVectorView;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float constant;
    float linear;
    float quadratic;
	vec4 directionView;
	float innerCutoff;
	float outerCutoff;
};

layout (location = 0) in vec4 fragNormalView;
layout (location = 1) in vec4 fragPositionView;
layout (location = 2) in vec2 fragTextureUV;

layout (location = 3) uniform Material material;
layout (location = 6) uniform Light light;

layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 textureSample = vec3(texture(material.diffuse, fragTextureUV));
	vec3 ambient = textureSample * light.ambient;

	
	vec4 lightDirection = normalize(-light.lightVectorView);
	float intensity = 1.0f;
	float quadraticAttenuation = 1.0f;
	if (light.lightVectorView.w != 0.0f)	//if not Directional light => Positional light
	{
		vec4 lightDirectionSpot = lightDirection;
		vec4 vecToLight = light.lightVectorView - fragPositionView;
		lightDirection = normalize(vecToLight);
	
		//If spotlight
		if (light.directionView.w != 1.0f)
		{
			float theta = dot(lightDirection, -light.directionView);
			float epsilon = light.innerCutoff - light.outerCutoff;
			intensity = clamp((theta - light.outerCutoff) / epsilon , 0.0f, 1.0f);
		}
		float distanceToLight = length(vecToLight);
		quadraticAttenuation = 1.0f / (light.constant + light.linear * distanceToLight + light.quadratic * distanceToLight * distanceToLight);
	}
	vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
	vec3 specular = vec3(0.0f, 0.0f, 0.0f);
	
	if (intensity > 0.0f)
	{
		vec4 normal = normalize(fragNormalView);
		diffuse = max(dot(normal, lightDirection), 0.0f) * textureSample * light.diffuse;

		vec4 viewDirection = normalize(-fragPositionView);
		vec4 reflectDirection = reflect(-lightDirection, normal);
		specular = pow(max(dot(viewDirection, reflectDirection), 0), material.shininess) * vec3(texture(material.specular, fragTextureUV)) * light.specular;
	}

	vec3 result = ((specular + diffuse) * intensity + ambient) * quadraticAttenuation;
	fragColor = vec4(result, 1.0f);
}