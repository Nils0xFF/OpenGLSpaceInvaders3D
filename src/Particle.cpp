#include "Particle.h"
#include "ParticleShader.h"
#include "ShaderManager.h"

Particle::Particle()
{
	Init();
}

Particle::Particle(Vector velocity, Color beginCol,
	Color endCol, float beginSize, float endSize, 
	float life): Velocity(velocity), Life(life),
	colorBegin(beginCol), colorEnd(endCol),
	sizeBegin(beginSize), sizeEnd(endSize)
{
	Init();
}

Particle::Particle(const Particle& other) {
	Init();

	this->Velocity = other.Velocity;
	this->colorBegin = other.colorBegin;
	this->colorEnd = other.colorEnd;
	this->sizeBegin = other.sizeBegin;
	this->sizeEnd = other.sizeEnd;
	this->Life = other.Life;

	// shader(ShaderManager::getParticleShader(), false);

}

void Particle::draw(const BaseCamera& Cam)
{

	ParticleShader* partShader = dynamic_cast<ParticleShader*>(this->shader());
	if (partShader) {
		partShader->color(col);
		partShader->alpha(alpha);
	}
	BaseModel::draw(Cam);
	VB.activate();
	IB.activate();

	glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

	IB.deactivate();
	VB.deactivate();



}

void Particle::Init()
{
	VB.begin();

	VB.addTexcoord0(0.0f, 0.0f);
	VB.addVertex(-0.5f, 0.5f, 0.0f);
	VB.addTexcoord0(0.0f, 1.0f);
	VB.addVertex(-0.5f, -0.5f, 0.0f);
	VB.addTexcoord0(1.0f, 1.0f);
	VB.addVertex(0.5f, -0.5f, 0.0f);
	VB.addTexcoord0(1.0f, 0.0f);
	VB.addVertex(0.5f, 0.5f, 0.0f);

	VB.end();

	IB.begin();

	IB.addIndex(0);
	IB.addIndex(1);
	IB.addIndex(2);
	IB.addIndex(0);
	IB.addIndex(2);
	IB.addIndex(3);

	IB.addIndex(2);
	IB.addIndex(1);
	IB.addIndex(0);
	IB.addIndex(3);
	IB.addIndex(2);
	IB.addIndex(0);

	IB.end();

	this->shader(ShaderManager::getParticleShader(), false);
}
