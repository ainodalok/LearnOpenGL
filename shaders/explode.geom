#version 460

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT
{
	vec3 normal;
} gs_in[];

layout (std140, binding = 0) uniform ObjectBlock
{
	mat4 PV;
	mat4 M;
	mat4 cofactorM;
	vec3 camera;
    float time;
};

out GS_OUT
{
    vec3 position;
    vec3 normal;
} gs_out;

vec3 getNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 3.0;
    vec3 direction = normal * ((sin(time+1) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
} 

void main()
{
    vec3 normal = getNormal();

    gs_out.position = gl_in[0].gl_Position.xyz;
    gs_out.normal = gs_in[0].normal;
    gl_Position = PV * explode(gl_in[0].gl_Position, normal);
    EmitVertex();

    gs_out.position = gl_in[1].gl_Position.xyz;
    gs_out.normal = gs_in[1].normal;
    gl_Position = PV * explode(gl_in[1].gl_Position, normal);
    EmitVertex();

    gs_out.position = gl_in[2].gl_Position.xyz;
    gs_out.normal = gs_in[2].normal;
    gl_Position = PV * explode(gl_in[2].gl_Position, normal);
    EmitVertex();

    EndPrimitive();
}