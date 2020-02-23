#version 400
in vec2 TexCoords;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 DepthColor;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    FragColor = vec4(1, 1, 1, texture(text, TexCoords).r) * vec4(textColor, 1.0);
    BrightColor = FragColor;
    DepthColor = vec4(0,0,1.0 - FragColor.a,1);
}