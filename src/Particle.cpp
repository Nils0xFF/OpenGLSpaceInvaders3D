#include "Particle.h"
#include "ParticleShader.h"

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

void Particle::draw(const BaseCamera& Cam)
{
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

	this->shader(new ParticleShader(), true);
}
