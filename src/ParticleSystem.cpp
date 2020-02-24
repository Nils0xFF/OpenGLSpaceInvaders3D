#include "ParticleSystem.h"
#include "Random.h"
#include "ParticleShader.h"
#include "CameraManager.h"

ParticleSystem::ParticleSystem(unsigned int nr)
{
	particlePool.resize(nr + 1);
	index = nr;
}

void ParticleSystem::Update(float deltaTime)
{
		for (Particle& par : particlePool) {
		if (!par.isActive())
			continue;
		if (par.getRemaining() <= 0.0f) {
			par.setActive(false);
			continue;
		}
		par.setRemaining(par.getRemaining() - deltaTime);
		par.transform(par.transform() * Matrix().translation(par.getVelocity() * deltaTime));

		float life = par.getRemaining() / par.getLife();
		Color col = Color(
			par.getColorEnd().R + life * (par.getColorBegin().R - par.getColorEnd().R),
			par.getColorEnd().G + life * (par.getColorBegin().G - par.getColorEnd().G),
			par.getColorEnd().B + life * (par.getColorBegin().B - par.getColorEnd().B)
		);

		float size = par.getSizeEnd() + life * (par.getSizeBegin() - par.getSizeEnd());				
		par.transform(par.transform() * Matrix().scale(par.getSize()).invert() * Matrix().scale(size));
		par.setSize(size);
		ParticleShader* shader = (ParticleShader*)par.shader();
		shader->color(col);
		shader->alpha(life);
	}
}

void ParticleSystem::Draw()
{
	for (Particle& par : particlePool) {
		if (!par.isActive())
			continue;	

		par.draw(*CameraManager::getInstance().activeCamera);
	}
}

void ParticleSystem::Emit(const ParticleProps& props)
{
	Particle& particle = particlePool[index];
	particle.setActive(true);	
	particle.transform(Matrix().translation(props.Position));
	particle.transform(particle.transform() *
		Matrix().rotationZ(props.Rotation * (Random::random() - 0.5f)));
	particle.transform(particle.transform() * Matrix().scale(props.sizeBegin));
	particle.setVelocity(
		Vector(
			props.Velocity.X + props.VelocityVariation.X * (Random::random() - 0.5f),
			props.Velocity.Y + props.VelocityVariation.Y * (Random::random() - 0.5f),
			props.Velocity.Z + props.VelocityVariation.Z * (Random::random() - 0.5f))
		);
	particle.setColorBegin(props.colorBegin);
	particle.setColorEnd(props.colorEnd);
	particle.setLife(props.Life);
	particle.setRemaining(props.Life);
	particle.setSizeBegin(props.sizeBegin + props.sizeVariation * (Random::random() - 0.5f));
	particle.setSizeEnd(props.sizeEnd);

	index = --index % particlePool.size();
}
