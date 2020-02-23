#version 400
in vec4 Position;
in vec2 Texcoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 DepthColor;

uniform int WorldDepth;
const float near = 0.1;
float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * WorldDepth) / (WorldDepth + near - z * (WorldDepth - near));
}

void main()
{   	
    FragColor = vec4(1, 0, 1, 1);
	if (Position.y > 0.0) {
		FragColor.r = 0;
	}

	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

	DepthColor = vec4(vec3(linearizeDepth(gl_FragCoord.z)), 1.0);
}
