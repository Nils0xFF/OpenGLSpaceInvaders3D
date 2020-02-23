#pragma once
#include "BaseShader.h"

class BlurShader : public BaseShader
{
public:
	BlurShader();

	void activate(const BaseCamera& Cam) const;

	const bool getHorizontal() const { return Horizontal; }
	void setHorizontal(const bool value) { Horizontal = value; }

protected:
private:
	GLint HorizontalLoc;

	bool Horizontal = false;
};

