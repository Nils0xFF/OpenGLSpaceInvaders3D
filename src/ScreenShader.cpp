#include "ScreenShader.h"
#include "CameraManager.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

ScreenShader::ScreenShader(): UpdateState(0xFFFFFFFF) {
	bool loaded = load(ASSET_DIRECTORY "shader/vertex/vsscreen.glsl", ASSET_DIRECTORY "shader/fragment/fsscreen.glsl");
	if (!loaded)
		throw std::exception();
	assignLocations();
}

void ScreenShader::assignLocations()
{
	OnLoc = glGetUniformLocation(ShaderProgram, "on");
	TimeLoc = glGetUniformLocation(ShaderProgram, "time");

	InvertedLoc = glGetUniformLocation(ShaderProgram, "inverted");
	GrayLoc = glGetUniformLocation(ShaderProgram, "gray");
	BluredLoc = glGetUniformLocation(ShaderProgram, "blured");
	CurvedLoc = glGetUniformLocation(ShaderProgram, "curved");
	BarsLoc = glGetUniformLocation(ShaderProgram, "bars");
	LinesLoc = glGetUniformLocation(ShaderProgram, "lines");
	VigLoc = glGetUniformLocation(ShaderProgram, "vig");
	FogLoc = glGetUniformLocation(ShaderProgram, "fog");

	BlurOffsetLoc = glGetUniformLocation(ShaderProgram, "blurOffset");
	CurveIntensityLoc = glGetUniformLocation(ShaderProgram, "curveIntensity");
	CurveScaleLoc = glGetUniformLocation(ShaderProgram, "curveScale");
	BarRangeLoc = glGetUniformLocation(ShaderProgram, "barRange");
	BarFrequencyLoc = glGetUniformLocation(ShaderProgram, "barFrequency");
	BarSpeedLoc = glGetUniformLocation(ShaderProgram, "barSpeed");
	BarOffsetLoc = glGetUniformLocation(ShaderProgram, "barOffset");
	LineScaleLoc = glGetUniformLocation(ShaderProgram, "LineScale");
	LineIntensityLoc = glGetUniformLocation(ShaderProgram, "LineIntensity");
	LineSpeedLoc = glGetUniformLocation(ShaderProgram, "LineSpeed");
	VignetteRadiusLoc = glGetUniformLocation(ShaderProgram, "vignetteRadius");
	VignetteSoftnessLoc = glGetUniformLocation(ShaderProgram, "vignetteSoftness");

	WorldDepthLoc = glGetUniformLocation(ShaderProgram, "worldDepth");
	FogStartLoc = glGetUniformLocation(ShaderProgram, "fogStartZ");
	FogEndLoc = glGetUniformLocation(ShaderProgram, "fogEndZ");

	FogColorLoc = glGetUniformLocation(ShaderProgram, "fogColor");
	
	GLuint screenTexLoc = glGetUniformLocation(ShaderProgram, "screenTexture");
	assert(screenTexLoc >= 0);
	GLuint brightnessTexLoc = glGetUniformLocation(ShaderProgram, "brightnessTexture");
	assert(brightnessTexLoc >= 0);
	GLuint depthTexLoc = glGetUniformLocation(ShaderProgram, "depthTexture");
	assert(depthTexLoc >= 0);	
	GLuint trueDepthTexLoc = glGetUniformLocation(ShaderProgram, "trueDepthTexture");
	assert(trueDepthTexLoc >= 0);

	activate(*CameraManager::getInstance().activeCamera);
	glUniform1i(screenTexLoc, 0);
	glUniform1i(brightnessTexLoc, 1);
	glUniform1i(depthTexLoc, 2);
	glUniform1i(trueDepthTexLoc, 3);
	deactivate();
}

void ScreenShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	if (UpdateState & ON_CHANGED)
		glUniform1i(OnLoc, On);
	if (!On) return;

	glUniform1f(TimeLoc, Time);

	// update uniforms if necessary
	if (UpdateState & INVERTED_CHANGED)
		glUniform1f(InvertedLoc, Inverted);
	if (UpdateState & GRAY_CHANGED)
		glUniform1f(GrayLoc, Gray);
	if (UpdateState & BLURED_CHANGED)
		glUniform1f(BluredLoc, Blured);
	if (UpdateState & CURVED_CHANGED)
		glUniform1f(CurvedLoc, Curved);
	if (UpdateState & BARS_CHANGED)
		glUniform1f(BarsLoc, Bars);
	if (UpdateState & LINES_CHANGED)
		glUniform1f(LinesLoc, Lines);
	if (UpdateState & VIG_CHANGED)
		glUniform1f(VigLoc, Vig);
	if (UpdateState & FOG_CHANGED)
		glUniform1f(FogLoc, Fog);

	if (UpdateState & BLUR_OFFSET_CHANGED && Blured)
		glUniform1f(BlurOffsetLoc, BlurOffset);
	if (UpdateState & CURVE_INTENSITY_CHANGED && Curved)
		glUniform1f(CurveIntensityLoc, CurveIntensity);
	if (UpdateState & CURVE_SCALE_CHANGED && Curved)
		glUniform1f(CurveScaleLoc, CurveScale);
	if (UpdateState & BAR_RANGE_CHANGED && Bars)
		glUniform1f(BarRangeLoc, BarRange);
	if (UpdateState & BAR_FREQUENCY_CHANGED && Bars)
		glUniform1f(BarFrequencyLoc, BarFrequency);
	if (UpdateState & BAR_SPEED_CHANGED && Bars)
		glUniform1f(BarSpeedLoc, BarSpeed);
	if (UpdateState & BAR_OFFSET_CHANGED && Bars)
		glUniform1f(BarOffsetLoc, BarOffset);
	if (UpdateState & LINE_SCALE_CHANGED && Lines)
		glUniform1f(LineScaleLoc, LineScale);
	if (UpdateState & LINE_INTENSITY_CHANGED && Lines)
		glUniform1f(LineIntensityLoc, LineIntensity);
	if (UpdateState & LINE_SPEED_CHANGED && Lines)
		glUniform1f(LineSpeedLoc, LineSpeed);
	if (UpdateState & VIGNETTE_RADIUS_CHANGED && Vig)
		glUniform1f(VignetteRadiusLoc, VignetteRadius);
	if (UpdateState & VIGNETTE_SOFTNESS_CHANGED && Vig)
		glUniform1f(VignetteSoftnessLoc, VignetteSoftness);
	if (UpdateState & FOG_COLOR_CHANGED && Fog)
		// glUniform3f(FogColorLoc, FogColor.R, FogColor.G, FogColor.B);

	glUniform1f(WorldDepthLoc, WorldDepth);
	glUniform1f(FogStartLoc, fogStartZ);
	glUniform1f(FogEndLoc, fogEndZ);
}

ScreenShader* ScreenShader::on(bool value)
{
	On = value;
	UpdateState |= ON_CHANGED;
	return this;
}

ScreenShader* ScreenShader::inverted(bool value)
{
	Inverted = value;
	UpdateState |= INVERTED_CHANGED;
	return this;
}

ScreenShader* ScreenShader::gray(bool value)
{
	Gray = value;
	UpdateState |= GRAY_CHANGED;
	return this;
}

ScreenShader* ScreenShader::blured(bool value)
{
	Blured = value;
	UpdateState |= BLURED_CHANGED;
	return this;
}

ScreenShader* ScreenShader::curved(bool value)
{
	Curved = value;
	UpdateState |= CURVED_CHANGED;
	return this;
}

ScreenShader* ScreenShader::bars(bool value)
{
	Bars = value;
	UpdateState |= BARS_CHANGED;
	return this;
}

ScreenShader* ScreenShader::lines(bool value)
{
	Lines = value;
	UpdateState |= LINES_CHANGED;
	return this;
}

ScreenShader* ScreenShader::vig(bool value)
{
	Vig = value;
	UpdateState |= VIG_CHANGED;
	return this;
}

void ScreenShader::setBlurOffset(float value)
{
	BlurOffset = value;
	UpdateState |= BLUR_OFFSET_CHANGED;
}

void ScreenShader::setCurveIntensity(float value)
{
	CurveIntensity = value;
	UpdateState |= CURVE_INTENSITY_CHANGED;
}

void ScreenShader::setCurveScale(float value)
{
	CurveScale = value;
	UpdateState |= CURVE_SCALE_CHANGED;
}

void ScreenShader::setBarRange(float value)
{
	BarRange = value;
	UpdateState |= BAR_RANGE_CHANGED;
}

void ScreenShader::setBarFrequency(float value)
{
	BarFrequency = value;
	UpdateState |= BAR_FREQUENCY_CHANGED;
}

void ScreenShader::setBarSpeed(float value)
{
	BarSpeed = value;
	UpdateState |= BAR_SPEED_CHANGED;
}

void ScreenShader::setBarOffset(float value)
{
	BarOffset = value;
	UpdateState |= BAR_OFFSET_CHANGED;
}

void ScreenShader::setLineScale(float value)
{
	LineScale = value;
	UpdateState |= LINE_SCALE_CHANGED;
}

void ScreenShader::setLineIntensity(float value)
{
	LineIntensity = value;
	UpdateState |= LINE_INTENSITY_CHANGED;
}

void ScreenShader::setLineSpeed(float value)
{
	LineSpeed = value;
	UpdateState |= LINE_SPEED_CHANGED;
}

void ScreenShader::setVignetteRadius(float value)
{
	VignetteRadius = value;
	UpdateState |= VIGNETTE_RADIUS_CHANGED;
}

void ScreenShader::setVignetteSoftness(float value)
{
	VignetteSoftness = value;
	UpdateState |= VIGNETTE_SOFTNESS_CHANGED;
}

void ScreenShader::setFog(const bool value)
{
	Fog = value;
	UpdateState |= FOG_CHANGED;
}

void ScreenShader::setFogColor(const Color value)
{
	FogColor = value;
	UpdateState |= FOG_COLOR_CHANGED;
}