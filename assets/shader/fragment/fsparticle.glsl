#version 400
in vec4 Position;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform vec3 color;
uniform float alpha;

void main()
{
	FragColor = vec4(color, alpha);
}
