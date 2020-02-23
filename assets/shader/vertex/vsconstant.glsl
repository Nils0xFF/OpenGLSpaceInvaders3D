#version 400
in vec4 VertexPos;

out vec3 Position;

uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;

void main()
{
    Position = (ModelMat * VertexPos).xyz;
    gl_Position = ModelViewProjMat * VertexPos;
}