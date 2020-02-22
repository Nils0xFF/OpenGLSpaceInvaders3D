#version 400
in vec4 Position;
in vec2 Texcoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 DepthColor;

void main()
{   	
    FragColor = vec4(1, 0, 1, 1);
	if (Position.y > 0.0) {
		FragColor.r = 0;
	}
}
