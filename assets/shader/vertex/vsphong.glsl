#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec3 VertexTexcoord[3];

out vec3 Position;
out vec3 Normal;
out vec2 Texcoord;
out vec3 Tangent;
out vec3 BiTangent;
out vec4 ViewSpace;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ModelViewProjMat;

void main()
{
    ViewSpace = ViewMat * ModelMat * VertexPos;
    Position = (ModelMat * VertexPos).xyz;
    Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz;
    Texcoord = VertexTexcoord[0].xy;
    Tangent = (ModelMat * vec4(VertexTexcoord[1] ,0)).xyz;
    BiTangent = (ModelMat * vec4(VertexTexcoord[2] ,0)).xyz;
    gl_Position = ModelViewProjMat * VertexPos;
}

