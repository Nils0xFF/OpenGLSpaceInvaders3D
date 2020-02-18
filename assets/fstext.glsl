#version 400
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    FragColor = vec4(1, 1, 1, texture(text, TexCoords).r) * vec4(textColor, 1.0);
} 