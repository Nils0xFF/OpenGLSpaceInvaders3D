#pragma once
#include "BaseShader.h"

class ParticleShader : public BaseShader
{
public:
	ParticleShader();
	ParticleShader* clone() { return new ParticleShader(*this); }
	void color(const Color& c) { Col = c; }
	const Color& color() const { return Col; }
	virtual void activate(const BaseCamera& Cam) const;

private:
	Color Col;

	GLint ColorLoc;
	GLint ModelViewProjLoc;
};

