#version 400
layout(location=0) in vec4 VertexPos;
layout(location=2) in vec2 VertexTexcoord;

uniform sampler2D NoiseMap;
uniform mat4 ModelViewProjMat;
uniform float Time;
uniform int WorldWidth;
uniform int MountainWidth;

out vec4 Position;
out vec2 Texcoord;

void main()
{
	Position = VertexPos;
	Texcoord = VertexTexcoord;

	// float totalWidth = WorldWidth + 2 * MountainWidth;
	float h = clamp((abs(Position.x) - (WorldWidth * 0.5)) / (MountainWidth / 2), 0, 1);
	float noiseValue = texture(NoiseMap, Texcoord).r;
	Position.y = h * noiseValue;
	
    gl_Position = ModelViewProjMat * Position;
}
