#include "Particle.h"

Particle::Particle()
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

	IB.end();
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
