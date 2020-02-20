#pragma once
#include "BaseShader.h"

class ParticleShader : public BaseShader
{
public:
	ParticleShader();
	ParticleShader* clone() { return new ParticleShader(*this); }

	void color(const Color& c) { Col = c; }
	void alpha(const float a) { Alpha = a; }
	const Color& color() const { return Col; }
	const float alpha() const { return Alpha; }

	virtual void activate(const BaseCamera& Cam) const;

private:
	Color Col;
	float Alpha;

	int ColorLoc;
	int AlphaLoc;
	int ModelViewProjLoc;
};

