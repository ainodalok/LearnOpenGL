#version 460 core

in GS_OUT
{
    vec4 position;
} fs_in;

layout (std140, binding = 2) uniform LightBlock
{
	vec4 viewPos;
    vec3 lightPos;
    float far_plane;
};

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(fs_in.position.xyz - lightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}  