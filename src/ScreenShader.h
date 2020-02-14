#pragma once
#include "BaseShader.h"
class ScreenShader : public BaseShader
{
public:
	ScreenShader();

	ScreenShader* setTime(float time) { Time = time; return this; }
	ScreenShader* on(bool value);
	ScreenShader* inverted(bool value);
	ScreenShader* gray(bool value);
	ScreenShader* blured(bool value);
	ScreenShader* curved(bool value);
	ScreenShader* bars(bool value);
	ScreenShader* lines(bool value);
	ScreenShader* vig(bool value);
	void setBlurOffset(float value);
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

	bool isOn() const { return On; }
	bool isInverted() const { return Inverted; }
	bool isGray() const { return Gray; }
	bool isBlured() const { return Blured; }
	bool isCurved() const { return Curved; }
	bool isBars() const { return Bars; }
	bool isLines() const { return Lines; }
	bool isVig() const { return Vig; }
	float getBlurOffset() const { return BlurOffset; }
	float getCurveIntensity() const { return CurveIntensity; }
	float getCurveScale() const { return CurveScale; }
	float getBarRange() const { return BarRange; }
	float getBarFrequency() const { return BarFrequency; }
	float getBarSpeed() const { return BarSpeed; }
	float getBarOffset() const { return BarOffset; }
	float getLineScale() const { return LineScale; }
	float getLineIntensity() const { return LineIntensity; }
	float getLineSpeed() const { return LineSpeed; }
	float getVignetteRadius() const { return VignetteRadius; }
	float getVignetteSoftness() const { return VignetteSoftness; }

	virtual void activate(const BaseCamera& Cam) const;

protected:
	void assignLocations();

private:
	float Time = 1.0f;
	bool On = true;
	bool Inverted = false;
	bool Gray = false;
	bool Blured = false;
	bool Curved = true;
	bool Bars = true;
	bool Lines = true;
	bool Vig = true;
	float BlurOffset = 1.0f / 300.0f;
	float CurveIntensity = 1.0f;
	float CurveScale = 1.05f;
	float BarRange = 0.015f;
	float BarFrequency = 0.5f;
	float BarSpeed = 0.4f;
	float BarOffset = 0.0075f;
	float LineScale = .004f;
	float LineIntensity = 0.03f;
	float LineSpeed = 1.0f;
	float VignetteRadius = 0.45f;
	float VignetteSoftness = 1.0f;

	GLuint OnLoc;
	GLuint TimeLoc;
	GLuint InvertedLoc;
	GLuint GrayLoc;
	GLuint BluredLoc;
	GLuint CurvedLoc;
	GLuint BarsLoc;
	GLuint LinesLoc;
	GLuint VigLoc;
	GLuint BlurOffsetLoc;
	GLuint CurveIntensityLoc;
	GLuint CurveScaleLoc;
	GLuint BarRangeLoc;
	GLuint BarFrequencyLoc;
	GLuint BarSpeedLoc;
	GLuint BarOffsetLoc;
	GLuint LineScaleLoc;
	GLuint LineIntensityLoc;
	GLuint LineSpeedLoc;
	GLuint VignetteRadiusLoc;
	GLuint VignetteSoftnessLoc;

	mutable unsigned int UpdateState;

	enum UPDATESTATES
	{
		INVERTED_CHANGED = 1 << 0,
		GRAY_CHANGED = 1 << 1,
		BLURED_CHANGED = 1 << 2,
		CURVED_CHANGED = 1 << 3,
		BARS_CHANGED = 1 << 4,
		LINES_CHANGED = 1 << 5,
		VIG_CHANGED = 1 << 6,
		BLUR_OFFSET_CHANGED = 1 << 7,
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
		ON_CHANGED = 1 << 19
	};
};

