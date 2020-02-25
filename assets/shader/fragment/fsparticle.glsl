#version 400
in vec4 Position;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 DepthColor;

uniform vec3 color;
uniform float alpha;
uniform float WorldDepth;

void main()
{
	FragColor = vec4(color, alpha);

	BrightColor = vec4(FragColor.rgb, alpha);

	DepthColor = vec4(vec3(1, 0, (gl_FragCoord.z / gl_FragCoord.w) / WorldDepth), 1.0);    
}
