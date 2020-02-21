#pragma once
#include "vector.h"
#include "Component.h"
#include "GameObject.h"
#include "GameSettings.h"

class BulletController : public Component
{
private:
	Vector direction;
	float speed = 1;
	int damage = 1;

public:

	void Update(float deltaTime) {
		if (gameObject->getTransform().translation().Z < -GameSettings::WORLD_DEPTH || gameObject->getTransform().translation().Z > 2) {
			gameObject->Destroy();
		}
		gameObject->translate(direction * speed * deltaTime);
	};

	void onCollision(GameObject* other) {
		if (other->getTag() == Tag::EnemyBullet || other->getTag() == Tag::PlayerBullet) {
			this->Destroy();
			other->Destroy();
		}
	}

	BulletController* clone() { return new BulletController(*this); }

	const Vector& getDirection() const { return direction; }
	void setDirection(const Vector& dir) { direction = dir; }

	float getSpeed() const { return speed; }
	void setSpeed(const float speed) { this->speed = speed; }

	int getDamage() const { return damage; }
	void setDamage(const int damage) { this->damage = damage; }
};

