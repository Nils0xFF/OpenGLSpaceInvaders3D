#version 400
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform float time = 1.0;
uniform bool on = true;
uniform bool inverted = false;
uniform bool gray = false;
uniform bool blured = false;
uniform bool curved = true;
uniform bool bars = true;
uniform bool lines = true;
uniform bool vig = true;
uniform bool fog = true;
uniform float blurOffset = 1.0 / 300.0;
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
uniform vec3 fogColor = vec3(0.4, 0.4, 0.4);

const float density = 0.0001;
const float gradient = 1.5;


vec3 inverse(vec3 color)
{
	return vec3(1.0 - color);
}

vec3 grayscale(vec3 color)
{
	float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	return vec3(average, average, average);
}

vec3 blur(vec3 color)
{
    vec2 offsets[9] = vec2[](
        vec2(-blurOffset,  blurOffset), // top-left
        vec2( 0.0f,    blurOffset), // top-center
        vec2( blurOffset,  blurOffset), // top-right
        vec2(-blurOffset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( blurOffset,  0.0f),   // center-right
        vec2(-blurOffset, -blurOffset), // bottom-left
        vec2( 0.0f,   -blurOffset), // bottom-center
        vec2( blurOffset, -blurOffset)  // bottom-right    
    );

    float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16  
	);
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    for(int i = 0; i < 9; i++)
        color += sampleTex[i] * kernel[i];

	return color;
}

vec3 vignette(vec3 color, vec2 uv)
{
	float vignette = uv.x * uv.y * ( 1.0 - uv.x ) * ( 1.0 - uv.y );
    vignette = clamp( pow( 16.0 * vignetteSoftness * vignette, vignetteRadius ), 0.0, 1.0 );
    return color * vignette;
}

vec3 scanline(vec3 color, vec2 uv)
{
	color.rgb += sin((uv.y / lineScale - (time * lineSpeed * 6.0))) * lineIntensity;
	// color.rgb += sin(uv.x / lineScale * 2.0) * lineIntensity * 0.5;
	return color;
}

vec2 curve(vec2 uv)
{
   	uv = uv * 2.0 - 1.0;
    vec2 offset = abs( uv.yx ) / vec2( 5.0, 4.0 ) * curveIntensity;
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}

vec3 cutoff(vec3 color, vec2 uv) {
	if ( uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0 )
    {
        color = vec3( 0.0, 0.0, 0.0 );
    }
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
	if (!on) 
	{
		FragColor = vec4(texture(screenTexture, TexCoords).rgb, 1.0);
		return;
	}


	vec2 uv = TexCoords;
	if (curved) 
	{
		uv = curve(uv);
	}
	if (bars) 
	{
		uv = horizontalBars(uv);
	}

	vec3 color = texture(screenTexture, uv).rgb;
	vec4 depthCol = texture(depthTexture, uv);
	float depth = (depthCol.r + depthCol.g + depthCol.b) / 3;
	if (fog && depth > 0.9975)	
		color = mix(texture(screenTexture, uv).rgb, fogColor, depth).rgb;

	if (inverted)
	{
		color = inverse(color);
	}
	if (gray)
	{
		color = grayscale(color);
	}
	if (blured)
	{
		color = blur(color);
	}
	if (lines) 
	{
		color = scanline(color, uv);
	}
	if (vig) 
	{
		color = vignette(color, uv);
	}

	FragColor = vec4(color, 1.0);
}