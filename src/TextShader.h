#pragma once
#include "BaseShader.h"

class TextShader : public BaseShader
{
public:
	TextShader();
	TextShader* clone() { return new TextShader(*this); }
	void textColor(const Color& c) { TextColor = c; }
	const Color& textColor() const { return TextColor; }
	virtual void activate(const BaseCamera& Cam) const;

private:
	Color TextColor;

	unsigned int textColorLoc;
};

