#version 400
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time = 1.0;
uniform float curveIntensity = 1.0;
uniform float curveScale = 1.05;
uniform float barRange = 0.015;
uniform float barFrequency = 0.5;
uniform float barSpeed = 0.4;
uniform float barOffset = 0.0075;
uniform float lineScale = .004;
uniform float lineIntensity = 0.03;
uniform float lineSpeed = 1.0;
uniform float vignetteRadius = 0.45;
uniform	float vignetteSoftness = 1.0;


vec4 inverse(vec4 color)
{
	return vec4(vec3(1.0 - color), 1.0);
}

vec2 curve(vec2 uv)
{
   uv = uv * 2.0 - 1.0;
    vec2 offset = abs( uv.yx ) / vec2( 5.0, 4.0 ) * curveIntensity;
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}

vec3 vignette(vec3 color, vec2 uv)
{
	float vignette = uv.x * uv.y * ( 1.0 - uv.x ) * ( 1.0 - uv.y );
    vignette = clamp( pow( 16.0 * vignetteSoftness * vignette, vignetteRadius ), 0.0, 1.0 );
    return color * vignette;
}

vec3 scanline(vec3 color, vec2 uv){    
	color.rgb += sin((uv.y / lineScale - (time * lineSpeed * 6.0))) * lineIntensity;
	return color;
}

vec2 horizontalBars(vec2 uv) {
	for (float i = 0.0; i < 1.0; i += barSpeed) {
        float pos = mod(time * barSpeed * i, 1.7);
        float offset = sin(time * barFrequency * i) * barOffset;

		float edge0 = (pos - barRange);
		float edge1 = (pos + barRange);

		float x = smoothstep(edge0, pos, uv.y) * offset;
		x -= smoothstep(pos, edge1, uv.y) * offset;

        uv.x += x;
    }
	return uv;
}

void main()
{
	vec2 uv = horizontalBars(curve(TexCoords));	
	vec3 color = vignette(scanline(texture(screenTexture, uv).rgb, uv), uv);
	
	FragColor = vec4(color, 1.0);
}

