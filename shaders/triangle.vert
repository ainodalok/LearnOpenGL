#version 460 core
layout (location = 0) uniform float uniformVertexColor0;
layout (location = 1) uniform float uniformVertexColor1;
layout (location = 2) uniform float uniformVertexColor2;

layout (location = 0) in vec3 position;

layout (location = 0) out vec4 vertexColor;

void main()
{
	gl_Position = vec4(position, 1.0f);
	if (gl_VertexID == 0)
		vertexColor = vec4(uniformVertexColor0, uniformVertexColor1, uniformVertexColor2, 1.0f);
	else if (gl_VertexID == 1)
		vertexColor = vec4(uniformVertexColor2, uniformVertexColor0, uniformVertexColor1, 1.0f);
	else if (gl_VertexID == 2)
		vertexColor = vec4(uniformVertexColor1, uniformVertexColor2, uniformVertexColor0, 1.0f);
}