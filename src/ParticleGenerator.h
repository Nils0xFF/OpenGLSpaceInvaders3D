#pragma once
#include "Component.h"
#include "ParticleSystem.h"

class ParticleGenerator : public Component
{
public:
	ParticleGenerator(const float max, const ParticleProps& props);
	~ParticleGenerator() { delete sys; }
	ParticleGenerator* clone() { return new ParticleGenerator(*this); }

	void Update(float deltaTime);
	void Draw();

	const Vector& getPosition() const { return props.Position; }
	const float getRotation() const { return props.Rotation; }
	const Vector& getVeloctiy() const { return props.Velocity; }
	const Vector& getVelVariation() const { return props.VelocityVariation; }
	const Color& getColorBegin() const { return props.colorBegin; }
	const Color& getColorEnd() const { return props.colorEnd; }
	const float getSizeBegin() const { return props.sizeBegin; }
	const float getSizeEnd() const { return props.sizeEnd; }
	const float getSizeVariation() const { return props.sizeVariation; }
	const float getLifeTime() const { return props.Life; }
	void setPosition(const Vector& pos) { props.Position = pos; }
	void setRotation(const float rot) { props.Rotation = rot; }
	void setVeloctiy(const Vector& vel) { props.Velocity = vel; }
	void setVelVariation(const Vector& var) { props.VelocityVariation = var; }
	void setColorBegin(const Color& col) { props.colorBegin = col; }
	void setColorEnd(const Color& col) { props.colorEnd = col; }
	void setSizeBegin(const float size) { props.sizeBegin = size; }
	void setSizeEnd(const float size) { props.sizeEnd = size; }
	void setSizeVariation(const float var) { props.sizeVariation = var; }
	void setLifeTime(const float life) { props.Life = life; }

	const bool isEmiting() const { return emiting; }
	void setEmiting(const bool value) { emiting = value; }

protected:

private:
	ParticleSystem* sys;
	ParticleProps props;
	bool emiting = false;
};

