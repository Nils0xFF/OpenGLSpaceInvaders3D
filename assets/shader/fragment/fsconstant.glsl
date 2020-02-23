#version 400
in vec3 Position;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 DepthColor;

uniform vec3 Color;

uniform int WorldDepth;
const float near = 0.1;

float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    float linearDepth = (2.0 * near * WorldDepth) / (z * (WorldDepth - near) - (WorldDepth + near));
    return (near + linearDepth)/(near - WorldDepth);
}

void main()
{
    FragColor = vec4(Color,1);
    
    BrightColor = vec4(FragColor.rgb, 1.0);
        
    DepthColor = vec4(vec3((gl_FragCoord.z / gl_FragCoord.w) / WorldDepth), 1.0);    
}