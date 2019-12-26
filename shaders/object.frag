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

	
	vec4 lightDirection;
	vec4 normal = normalize(fragNormalView);
	if (light.lightVectorView.w == 0.0f)	//Directional light
		lightDirection = normalize(-light.lightVectorView);
	else	//Positional light
	{
		vec4 vecToLight = light.lightVectorView - fragPositionView;
		lightDirection = normalize(light.lightVectorView);

		//float distanceToLight = length(vecToLight);
		//float quadraticAttenuation = 1.0f / max(0.2f * distanceToLight * (distanceToLight + 1), 1.0f);
	}
	vec3 diffuse = max(dot(normal, lightDirection), 0.0f) * textureSample * light.diffuse;

	vec4 viewDirection = normalize(-fragPositionView);
	vec4 reflectDirection = reflect(-lightDirection, fragNormalView);
	vec3 specular = pow(max(dot(viewDirection, reflectDirection), 0), material.shininess) * vec3(texture(material.specular, fragTextureUV)) * light.specular;

	vec3 result = specular + diffuse + ambient;// * quadraticAttenuation;
	fragColor = vec4(result, 1.0f);
}