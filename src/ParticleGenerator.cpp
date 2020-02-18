#include "ParticleGenerator.h"
#include "CameraManager.h"
#include "ConstantShader.h"
ParticleGenerator::ParticleGenerator(unsigned int pps, Vector direction, float maxLife): Pps(pps), Direction(direction), MaxLife(maxLife)
{
}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::Update(float deltaTime)
{		
	for (int i = 0; i < particles.size(); i++) {
		Particle* par = particles.at(i);
		if (par->getLife() > MaxLife) {
			particles.erase(particles.begin() + i);
			continue;
		}
		std::cout << par->transform().right() << std::endl;
		par->setLife(par->getLife() + deltaTime);
		//par->setVelocity(par->getVelocity() * deltaTime);
		par->transform(par->transform() * Matrix().translation(par->getVelocity()));
	}
	
	float nr = std::round(Pps * deltaTime);
	for (int i = 0; i < nr; i++) {
		Particle* par = new Particle(Direction);		
		par->transform(gameObject->getTransform() * Matrix().scale(0.01f));
		particles.push_back(par);
	}
}

void ParticleGenerator::Draw()
{
	for (Particle* part : particles)	{
		part->draw(*CameraManager::getInstance().activeCamera);
	}
}
