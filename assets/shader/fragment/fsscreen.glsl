#version 400
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D brightnessTexture;
uniform sampler2D depthTexture;
uniform sampler2D trueDepthTexture;
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
uniform bool bloom = true;
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

uniform float worldDepth;
uniform float fogStartZ;
uniform float fogEndZ;
uniform vec3 fogColor = vec3(0.5, 0.5, 0.5);

uniform float bloomExposure = 1.0;


vec3 inverse(vec3 color)
{
	return vec3(1.0 - color);
}

vec3 grayscale(vec3 color)
{
	float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	return vec3(average, average, average);
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

float normpdf(in float x, in float sigma)
{
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}

vec3 blur(sampler2D tex, vec2 uv) 
{
	//float weight[5] = float[] (0.4, 0.3, 0.2, 0.06, 0.03);
	float weight[5] = float[] (0.7, 0.55, 0.32, 0.15, 0.09);
	vec2 tex_offset = 1.0 / textureSize(tex, 0);
    vec3 result = texture(tex, uv).rgb * weight[0];
    
	for(int i = 1; i < 5; ++i)
	{
		result += texture(tex, uv + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		result += texture(tex, uv - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
		result += texture(tex, uv + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
		result += texture(tex, uv - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
	}

	return result;
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

	if (bloom) {
		color = mix(color, blur(brightnessTexture, uv), 0.5);
	}
	
	if (fog && texture(depthTexture, uv).x > 0.9) {
		float depth = texture(depthTexture, uv).z * worldDepth;
		float fogFactor = (fogEndZ - depth)/(fogEndZ - fogStartZ);
		fogFactor = clamp( fogFactor, 0.0, 1.0 );
		fogFactor = fogFactor + clamp((exp(texture(depthTexture, uv).y * 3.5) - 1.0),0.0,1.0);
		fogFactor = clamp( fogFactor, 0.0, 1.0 );
		color = mix(color.rgb, fogColor.rgb, 1.0 - fogFactor);

		/*float be = texture(depthTexture, uv).y * 0.004;//0.004 is just a factor; change it if you want
		float bi = texture(depthTexture, uv).y * 0.001;//0.001 is just a factor; change it if you want
 
		float be = 0.025 * smoothstep(0.0, 6.0, 32 - gl_FragCoord.y);
		float bi = 0.075* smoothstep(0.0, 80.0, 10.0 - gl_FragCoord.y); 

		float ext = exp(-depth * be);
		float insc = exp(-depth * bi);

		color = color * ext + fogColor * (1 - insc); */
	}

	if (inverted)
	{
		color = inverse(color);
	}
	if (gray)
	{
		color = grayscale(color);
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