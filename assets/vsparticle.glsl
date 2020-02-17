#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec2 VertexTexCoords;
out vec4 Position;
out vec2 TexCoords;

uniform mat4 ModelViewProjMat;

void main()
{
    Position = ModelViewProjMat * VertexPos;
    gl_Position = Position;
}

