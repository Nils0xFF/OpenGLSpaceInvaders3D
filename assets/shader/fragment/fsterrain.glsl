#version 400
in vec4 Position;
in vec2 Texcoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 DepthColor;

uniform float Time;
uniform float WorldWidth;
uniform float WorldDepth;

const float near = 0.1;

float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    float linearDepth = (2.0 * near * WorldDepth) / (z * (WorldDepth - near) - (WorldDepth + near));
    return (near + linearDepth)/(near - WorldDepth);
}

vec3 scanline(vec3 color, vec2 uv)
{
    vec3 lineColorStart = vec3(0.8, 0.2, 0.92);
    vec3 lineColorEnd = vec3(0.8, 0.355, 0.295);
    vec3 lineColor = mix(lineColorStart, lineColorEnd, Position.y);
    lineColor = mix (lineColor, color.rgb, clamp((WorldWidth - abs(Position.x)) / (WorldWidth),0.0,1.0));
    float line = sin((uv.y / .003 + (Time * 0.5 * 6.0))) * .05;
    color.rgb += lineColor * line;
	return color;
}

void main()
{   	
    FragColor = vec4(0, 0, 0, 1);

    FragColor = 15 * vec4(scanline(vec3(FragColor.rgb), Texcoord), 1.0);

    BrightColor = vec4(FragColor.rgb, 1.0);

    DepthColor = vec4(vec3(1.0,0.0,(gl_FragCoord.z / gl_FragCoord.w) / WorldDepth), 1.0);    
}
