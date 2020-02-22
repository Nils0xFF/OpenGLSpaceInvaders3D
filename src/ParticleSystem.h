#pragma once
#include <vector>
#include "Particle.h"

struct ParticleProps {
	Vector Position;
	float Rotation = 0;
	Vector Velocity, VelocityVariation;
	Color colorBegin, colorEnd;
	float sizeBegin = 0, sizeEnd = 0, sizeVariation = 0;
	float Life = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem(unsigned int nr);

	void Update(float deltaTime);
	void Draw();

	void Emit(const ParticleProps& props);
protected:

private:
	std::vector<Particle> particlePool;
	unsigned int index;
};