#pragma once
#include "BaseShader.h"
#include "GameSettings.h"

class ScreenShader : public BaseShader
{
public:
	ScreenShader();

	ScreenShader* setTime(float time) { Time = time; return this; }
	ScreenShader* on(bool value);
	ScreenShader* inverted(bool value);
	ScreenShader* gray(bool value);
	ScreenShader* bloom(bool value);
	ScreenShader* curved(bool value);
	ScreenShader* bars(bool value);
	ScreenShader* lines(bool value);
	ScreenShader* vig(bool value);
	ScreenShader* fog(const bool value);
	void setCurveIntensity(float value);
	void setCurveScale(float value);
	void setBarRange(float value);
	void setBarFrequency(float value);
	void setBarSpeed(float value);
	void setBarOffset(float value);
	void setLineScale(float value);
	void setLineIntensity(float value);
	void setLineSpeed(float value);
	void setVignetteRadius(float value);
	void setVignetteSoftness(float value);	
	void setFogColor(const Color value);
	void setBloomExposure(const float value);

	const bool isOn() const { return On; }
	const bool isInverted() const { return Inverted; }
	const bool isGray() const { return Gray; }
	const bool isBloom() const { return Bloom; }
	const bool isCurved() const { return Curved; }
	const bool isBars() const { return Bars; }
	const bool isLines() const { return Lines; }
	const bool isVig() const { return Vig; }
	const float getCurveIntensity() const { return CurveIntensity; }
	const float getCurveScale() const { return CurveScale; }
	const float getBarRange() const { return BarRange; }
	const float getBarFrequency() const { return BarFrequency; }
	const float getBarSpeed() const { return BarSpeed; }
	const float getBarOffset() const { return BarOffset; }
	const float getLineScale() const { return LineScale; }
	const float getLineIntensity() const { return LineIntensity; }
	const float getLineSpeed() const { return LineSpeed; }
	const float getVignetteRadius() const { return VignetteRadius; }
	const float getVignetteSoftness() const { return VignetteSoftness; }
	const bool isFog() const { return Fog; }
	const Color getFogColor() const { return FogColor; }
	const float getBloomExposure() const { return BloomExposure; }

	virtual void activate(const BaseCamera& Cam) const;

protected:
	void assignLocations();

private:
	float Time = 1.0f;
	bool On = true;
	bool Inverted = false;
	bool Gray = false;
	bool Bloom = true;
	bool Curved = true;
	bool Bars = true;
	bool Lines = true;
	bool Vig = true;
	bool Fog = true;
	float CurveIntensity = 1.0f;
	float CurveScale = 1.05f;
	float BarRange = 0.015f;
	float BarFrequency = 0.5f;
	float BarSpeed = 0.4f;
	float BarOffset = 0.0075f;
	float LineScale = .004f;
	float LineIntensity = 0.1f;
	float LineSpeed = 1.0f;
	float VignetteRadius = 0.45f;
	float VignetteSoftness = 1.0f;
	float BloomExposure = 1.0f;

	int WorldDepth = GameSettings::WORLD_DEPTH;
	float fogStartZ = 13.5f;
	float fogEndZ = GameSettings::WORLD_DEPTH - 1;

	Color FogColor = Color(0.75f, 0.75f, 0.75f);

	GLint OnLoc;
	GLint TimeLoc;
	GLint InvertedLoc;
	GLint GrayLoc;
	GLint BloomLoc;
	GLint CurvedLoc;
	GLint BarsLoc;
	GLint LinesLoc;
	GLint VigLoc;
	GLint BloomExposureLoc;
	GLint CurveIntensityLoc;
	GLint CurveScaleLoc;
	GLint BarRangeLoc;
	GLint BarFrequencyLoc;
	GLint BarSpeedLoc;
	GLint BarOffsetLoc;
	GLint LineScaleLoc;
	GLint LineIntensityLoc;
	GLint LineSpeedLoc;
	GLint VignetteRadiusLoc;
	GLint VignetteSoftnessLoc;
	GLint FogLoc;
	GLint WorldDepthLoc;
	GLint FogStartLoc;
	GLint FogEndLoc;
	GLint FogColorLoc;

	mutable unsigned int UpdateState;

	enum UPDATESTATES
	{
		INVERTED_CHANGED = 1 << 0,
		GRAY_CHANGED = 1 << 1,
		BLOOM_CHANGED = 1 << 2,
		CURVED_CHANGED = 1 << 3,
		BARS_CHANGED = 1 << 4,
		LINES_CHANGED = 1 << 5,
		VIG_CHANGED = 1 << 6,
		BLOOM_EXPOSURE_CHANGED = 1 << 7,
		CURVE_INTENSITY_CHANGED = 1 << 8,
		CURVE_SCALE_CHANGED = 1 << 9,
		BAR_RANGE_CHANGED = 1 << 10,
		BAR_FREQUENCY_CHANGED = 1 << 11,
		BAR_SPEED_CHANGED = 1 << 12,
		BAR_OFFSET_CHANGED = 1 << 13,
		LINE_SCALE_CHANGED = 1 << 14,
		LINE_INTENSITY_CHANGED = 1 << 15,
		LINE_SPEED_CHANGED = 1 << 16,
		VIGNETTE_RADIUS_CHANGED = 1 << 17,
		VIGNETTE_SOFTNESS_CHANGED = 1 << 18,
		ON_CHANGED = 1 << 19,
		FOG_CHANGED = 1 << 20,
		FOG_COLOR_CHANGED = 1 << 21
	};
};

