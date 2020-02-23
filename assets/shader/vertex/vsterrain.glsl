#version 400
layout(location=0) in vec4 VertexPos;
layout(location=2) in vec2 VertexTexcoord;

uniform sampler2D NoiseMap;
uniform mat4 ModelViewProjMat;
uniform float Time;
uniform float WorldWidth;
uniform float WorldDepth;
uniform float MountainWidth;

out vec4 Position;
out vec2 Texcoord;

void main()
{
	Position = VertexPos;
	Texcoord = VertexTexcoord;

	// float totalWidth = WorldWidth + 2 * MountainWidth;
	float h = clamp((abs(Position.x) - (WorldWidth * 0.5)) / (MountainWidth / 4.0f), 0.0, 1.0);
	float noiseValue = (texture(NoiseMap, vec2(Texcoord.x, Texcoord.y)).r + 0.25) * 1.75;
	Position.y = h * noiseValue;
	
    gl_Position = ModelViewProjMat * Position;
}
