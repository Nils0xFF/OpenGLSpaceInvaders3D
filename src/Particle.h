#pragma once
#include "BaseModel.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Particle : public BaseModel
{
public:
	Particle();
	Particle(Vector velocity, Color beginCol, Color endCol, 
		float beginSize, float endSize, float life);
	virtual ~Particle() {}
	virtual void draw(const BaseCamera& Cam);

	const Vector getVelocity() const { return Velocity; }
	const Color getColorBegin() const { return colorBegin; }
	const Color getColorEnd() const { return colorEnd; }
	const float getSizeBegin() const { return sizeBegin; }
	const float getSizeEnd() const { return sizeEnd; }
	const float getLife() const { return Life; }
	const float getRemaining() const { return Remaining; }
	void setVelocity(const Vector velocity) { Velocity = velocity; }
	void setColorBegin(const Color col) { colorBegin = col; }
	void setColorEnd(const Color col) { colorEnd = col; }
	void setSizeBegin(const float size) { sizeBegin = size; }
	void setSizeEnd(const float size) { sizeEnd = size; }
	void setLife(const float life) { Life = life; }
	void setRemaining(const float life) { Remaining = life; }

protected:
	VertexBuffer VB;
	IndexBuffer IB;

	void Init();

private:
	Vector Velocity = Vector(0, 1, 0);
	Color colorBegin, colorEnd;
	float sizeBegin, sizeEnd;
	float Life = 1.0f;
	float Remaining = 0.0f;
	bool Active = false;
};

