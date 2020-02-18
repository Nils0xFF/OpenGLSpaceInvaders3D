#pragma once
#include "Component.h"
#include "Particle.h"

class ParticleGenerator : public Component
{
public:
	void Update(float deltaTime);
	void Draw();

	ParticleGenerator(unsigned int nr, Vector direction, float life);
	~ParticleGenerator();
	ParticleGenerator* clone() { return new ParticleGenerator(*this); }

	const unsigned int getPps() const { return Pps; }
	const Vector getDirection() const { return Direction; }
	const float getMaxLife() const { return MaxLife; }
	void setPps(const unsigned int pps) { Pps = Pps; }
	void setDirection(const Vector direction) { Direction = direction; }
	void setMaxLife(const float life) { MaxLife = life; }

protected:

private:
	std::vector<Particle*> particles;
	unsigned int Pps;
	Vector Direction;
	float MaxLife;
};

