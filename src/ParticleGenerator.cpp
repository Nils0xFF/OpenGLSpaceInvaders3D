#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(const unsigned int max, ParticleProps* props)
{
	sys = new ParticleSystem(max);
	this->props = props;	
}

// Emits particle every other frame based on lastEmit
void ParticleGenerator::Update(float deltaTime)
{
	if (gameObject) {
		props->Position = gameObject->getTransform().translation();
	}

	sys->Update(deltaTime);
	if (emiting) {		
		lastEmit += deltaTime;
		if (lastEmit >= betweenEmit) {
			sys->Emit(*props);
			lastEmit = 0;
		}
	}
}

void ParticleGenerator::Draw() 
{
	sys->Draw();
}