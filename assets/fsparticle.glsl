#version 400
in vec4 Position;
out vec4 FragColor;

uniform vec3 color;

void main()
{
	FragColor = vec4(color, 1.0);
}
