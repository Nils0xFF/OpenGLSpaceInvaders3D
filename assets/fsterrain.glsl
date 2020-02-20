#version 400
in vec4 Position;
in vec2 Texcoord;

out vec4 FragColor;

void main()
{   	
    FragColor = vec4(1, 0, 1, 1);
	if (Position.y > 0.0) {
		FragColor.r = 0;
	}
}
