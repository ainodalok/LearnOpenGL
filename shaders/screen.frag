#version 460 core
layout (binding = 0) uniform sampler2D screenTexture;

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 fragColor;

void main()
{     
    //Inverse
    //fragColor = vec4(vec3(1.0 - texture(screenTexture, uv)), 1.0);
    
    //B&w filter
    //fragColor = vec4(vec3(texture(screenTexture, uv)), 1.0);
    //float average = (fragColor.r + fragColor.g + fragColor.b) / 3.0;
    //fragColor = vec4(vec3(average), 1.0);

    //Blur kernel
    //const float offset = 1.0 / 300.0;
    //vec2 offsets[9] = vec2[](
    //    vec2(-offset,  offset), // top-left
    //    vec2( 0.0f,    offset), // top-center
    //    vec2( offset,  offset), // top-right
    //    vec2(-offset,  0.0f),   // center-left
    //    vec2( 0.0f,    0.0f),   // center-center
    //    vec2( offset,  0.0f),   // center-right
    //    vec2(-offset, -offset), // bottom-left
    //    vec2( 0.0f,   -offset), // bottom-center
    //    vec2( offset, -offset)  // bottom-right    
    //);
    //
    //float kernel[9] = float[](
    //    1.0/16.0, 2.0/16.0, 1.0/16.0,
    //    2.0/16.0, 4.0/16.0, 2.0/16.0,
    //    1.0/16.0, 2.0/16.0, 1.0/16.0
    //);
    //
    //vec3 sampleTex[9];
    //for(int i = 0; i < 9; i++)
    //{
    //    sampleTex[i] = vec3(texture(screenTexture, uv.st + offsets[i]));
    //}
    //
    //vec3 col = vec3(0.0);
    //for(int i = 0; i < 9; i++)
    //    col += sampleTex[i] * kernel[i];
    //
    //fragColor = vec4(col, 1.0);

    fragColor = vec4(vec3(texture(screenTexture, uv)), 1.0);
}