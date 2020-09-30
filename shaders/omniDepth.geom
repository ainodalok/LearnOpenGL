#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

layout (std140, binding = 1) uniform OmniShadowUBO
{
	mat4 shadowMatrices[6];
};

out GS_OUT
{
	vec4 position;
} gs_out;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle vertex
        {
            gs_out.position = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * gs_out.position;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  