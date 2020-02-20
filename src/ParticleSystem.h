#pragma once
#include "Component.h"
#include <vector>
#include "Particle.h"

class ParticleSystem : public Component
{
public:
	ParticleSystem();

	void Update(float deltaTime);
	void Draw();

protected:

private:
	std::vector<Particle> particlePool;
	unsigned int index = 999;
};