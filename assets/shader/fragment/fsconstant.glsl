#version 400
in vec3 Position;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 DepthColor;

uniform vec3 Color;
uniform vec3 EyePos;
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
    // BrightColor = vec4(FragColor.r * 1.0, FragColor.g * 1.0, FragColor.b * 0.7, 1.0);

    BrightColor = vec4(10 * FragColor.rgb, 1);

    float dist = 2 * (vec3(0,0,WorldDepth)-Position).z - 1.0;
    vec3 projected = Position - dist * vec3(0,0,1);    
    DepthColor = vec4(vec3(1,projected.y / WorldDepth,(gl_FragCoord.z / gl_FragCoord.w) / WorldDepth), 1.0);    
}