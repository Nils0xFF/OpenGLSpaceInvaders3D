#pragma once
#include "vector.h"
#include "Component.h"

class BulletController : public Component
{
private:
	Vector direction;
	float speed;
	int damage;

public:

	void Update(float deltaTime) {
		if (gameObject->getTransform().translation().Z < -15 || gameObject->getTransform().translation().Z > 2) {
			gameObject->Destroy();
		}
		gameObject->translate(direction * speed * deltaTime);
	};

	BulletController* clone() { return new BulletController(*this); }

	const Vector& getDirection() const { return direction; }
	void setDirection(const Vector& dir) { direction = dir; }

	float getSpeed() const { return speed; }
	void setSpeed(const float speed) { this->speed = speed; }

	int getDamage() const { return damage; }
	void setDamage(const int damage) { this->damage = damage; }
};

