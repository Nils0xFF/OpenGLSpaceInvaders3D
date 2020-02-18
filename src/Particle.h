#pragma once
#include "BaseModel.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Particle : public BaseModel
{
public:
	Particle(Vector velocity, float life = 0);
	virtual ~Particle() {}
	virtual void draw(const BaseCamera& Cam);

	const Vector getVelocity() const { return Velocity; }
	const float getLife() const { return Life; }
	void setVelocity(const Vector velocity) { Velocity = velocity; }
	void setLife(const float life) { Life = life; }

protected:
	VertexBuffer VB;
	IndexBuffer IB;

private:
	Vector Velocity = Vector(0, 1, 0);
	float Life = 0.0f;
};

