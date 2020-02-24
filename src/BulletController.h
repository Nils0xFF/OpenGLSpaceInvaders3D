#pragma once
#include "vector.h"
#include "Component.h"
#include "GameObject.h"
#include "GameSettings.h"
#include "ParticleGenerator.h"

class BulletController : public Component
{
private:
	Vector direction;
	float speed = 1;
	int damage = 1;

public:
	void Start();

	void Update(float deltaTime);

	void onCollision(GameObject* other);

	BulletController* clone() { return new BulletController(*this); }

	const Vector& getDirection() const { return direction; }
	void setDirection(const Vector& dir) { direction = dir; }

	float getSpeed() const { return speed; }
	void setSpeed(const float speed) { this->speed = speed; }

	int getDamage() const { return damage; }
	void setDamage(const int damage) { this->damage = damage; }
};

