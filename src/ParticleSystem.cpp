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
		par.setLife(par.getLife() - deltaTime);
		par.transform(par.transform() * Matrix().translation(par.getVelocity() * deltaTime));
		//par.transform(par.transform() * Matrix().rotationZ(5 * deltaTime));
	}
}

void ParticleSystem::Draw()
{
	for (Particle& par : particlePool) {
		if (!par.isActive())
			continue;

		float life = par.getRemaining() / par.getLife();
		Color col = Color(
			par.getColorBegin().R + life * (par.getColorEnd().R - par.getColorBegin().R),
			par.getColorBegin().G + life * (par.getColorEnd().G - par.getColorBegin().G),
			par.getColorBegin().B + life * (par.getColorEnd().B - par.getColorBegin().B)
		);
		float size = par.getSizeBegin() + life * (par.getSizeEnd() - par.getSizeBegin());

		//par.transform(par.transform() * Matrix().scale(size));
		ParticleShader* shader = (ParticleShader*) par.shader();
		shader->color(col);
		shader->alpha(life);

		par.draw(*CameraManager::getInstance().activeCamera);
	}
}

void ParticleSystem::Emit(const ParticleProps& props)
{
	Particle& particle = particlePool[index];
	particle.setActive(true);
	particle.transform(particle.transform() *
		Matrix().translation(props.Position));
	particle.transform(particle.transform() *
		Matrix().rotationZ(props.Rotation * Random::random()));
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
