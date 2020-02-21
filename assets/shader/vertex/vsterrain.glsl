#version 400
layout(location=0) in vec4 VertexPos;
layout(location=2) in vec2 VertexTexcoord;

uniform mat4 ModelViewProjMat;
uniform float Time;

out vec4 Position;
out vec2 Texcoord;

#define M_PI 3.14159265359
#define e 2.7182818284

void main()
{
	Position = VertexPos;
	Texcoord = VertexTexcoord;

	//if (Texcoord.x < 0.25 || Texcoord.x > 0.75)
	//Position.y = pow((1 / (2 * M_PI)) * pow(e, -0.5 * pow(Position.x, 2)), 0.5) * 4;
	Position.y = Position.x * Texcoord.x;
	
    gl_Position = ModelViewProjMat * Position;
}
