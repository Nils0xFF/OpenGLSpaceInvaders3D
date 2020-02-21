#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(const float max, const ParticleProps& props)
{
	sys = new ParticleSystem(max);
}

void ParticleGenerator::Update(float deltaTime)
{
	if (gameObject) {
		props.Position += gameObject->getTransform().translation();
	}

	sys->Update(deltaTime);
	if (emiting) {
		sys->Emit(props);
	}
}

void ParticleGenerator::Draw() 
{
	sys->Draw();
}