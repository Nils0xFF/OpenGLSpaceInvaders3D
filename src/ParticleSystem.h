#pragma once
#include "Component.h"
#include <vector>
#include "Particle.h"

struct ParticleProps {
	Vector Position;
	float Rotation;
	Vector Velocity, VelocityVariation;
	Color colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float Life = 1.0f;
};

class ParticleSystem : public Component
{
public:
	ParticleSystem(unsigned int nr);
	ParticleSystem* clone() { return new ParticleSystem(*this); }

	void Update(float deltaTime);
	void Draw();

	void Emit(const ParticleProps& props);
	void Reset();
protected:

private:
	std::vector<Particle> particlePool;
	unsigned int index;
};