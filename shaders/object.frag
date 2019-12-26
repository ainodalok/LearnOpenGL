#version 460 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 lightPositionView;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (location = 0) in vec3 vertexNormalView;
layout (location = 1) in vec3 vertexPositionView;

layout (location = 0) out vec4 fragColor;

layout (location = 3) uniform Material material;
layout (location = 7) uniform Light light;

void main()
{
	vec3 ambient = material.ambient * light.ambient;

	vec3 vecToLight = light.lightPositionView - vertexPositionView;
	vec3 normal = normalize(vertexNormalView);
	vec3 lightDirection = normalize(vecToLight);
	vec3 diffuse = max(dot(normal, lightDirection), 0.0f) * material.diffuse * light.diffuse;

	vec3 viewDirection = normalize(-vertexPositionView);
	vec3 reflectDirection = reflect(-lightDirection, vertexNormalView);
	vec3 specular = pow(max(dot(viewDirection, reflectDirection), 0), material.shininess) * material.specular * light.specular;

	//float distanceToLight = length(vecToLight);
	//float quadraticAttenuation = 1.0f / max(0.2f * distanceToLight * (distanceToLight + 1), 1.0f);

	vec3 result = specular + diffuse + ambient;// * quadraticAttenuation;
	fragColor = vec4(result, 1.0f);
}