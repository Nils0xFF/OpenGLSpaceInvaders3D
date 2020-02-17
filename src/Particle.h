#pragma once
#include "BaseModel.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Particle : public BaseModel
{
public:
	Particle();
	virtual ~Particle() {}
	virtual void draw(const BaseCamera& Cam);
protected:
	VertexBuffer VB;
	IndexBuffer IB;
private:
	Vector velocity;
	float life;
};

