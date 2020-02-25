#pragma once
#include "Particle.h"
#include <list>

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
	ParticleSystem(const ParticleSystem& other) {
		for (Particle* p : other.particlePool) {
			particlePool.push_back(new Particle());
		}
		index = particlePool.size() - 1;
	}

	~ParticleSystem() {
		while (!particlePool.empty()) {
			delete particlePool.back();
			particlePool.pop_back();
		}
	}

	void Update(float deltaTime);
	void Draw();

	void Emit(const ParticleProps& props);
protected:

private:
	std::list<Particle*> particlePool;
	unsigned int index;
};
