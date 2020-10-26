#version 460 core
layout (binding = 0) uniform sampler2D brightTexture;

layout (location = 0) in vec2 texCoord;

layout (location = 0) uniform int verticalBlurBool;

layout (location = 0) out vec4 fragColor;

void main()
{     
	//const float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

	vec2 offset = 1.0 / textureSize(brightTexture, 0);
	vec2 offsets[2] = vec2[](1.3846153846 * offset, 3.2307692308 * offset);
	const float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);

	
	vec3 result = texture(brightTexture, texCoord).rgb * weight[0];

	for (int i = 0/*1*/; i < 2/*5*/; i++)
	{
		//vec2 offset = vec2(offset.x * i * (1 - verticalBlurBool), offset.y * i * verticalBlurBool);
		vec2 offset = vec2(offsets[i].x * (1 - verticalBlurBool), offsets[i].y * verticalBlurBool);
		result += texture(brightTexture, texCoord + offset).rgb * weight[i + 1];
		result += texture(brightTexture, texCoord - offset).rgb * weight[i + 1];
	}

	fragColor = vec4(result, 1.0);
}