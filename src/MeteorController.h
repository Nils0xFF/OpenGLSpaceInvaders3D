#pragma once
#include "Component.h"
#include "GameSettings.h"
#include "GameObject.h"
#include "BulletController.h"

class MeteorController :
	public Component
{
private:
	int currentHP = GameSettings::METEOR_HP;
	float speed = GameSettings::METEOR_Z_SPEED;

	void takeDamage(int damage) {
		this->currentHP -= damage;
		if (this->currentHP <= 0) {
			currentHP = 0;
			gameObject->Destroy();
		}
	}
public:
	MeteorController* clone() { return new MeteorController(*this); }

	void Update(float deltaTime) {
		if (gameObject->getTransform().translation().Z > 2) {
			gameObject->Destroy();
		}
		gameObject->translate(-Vector::forward() * speed * deltaTime);
	};

	void onCollision(GameObject* other) {
		if (other->getTag() == Tag::PlayerBullet) {
			BulletController* bc = other->getComponentByType<BulletController>();
			if (bc) {
				this->takeDamage(bc->getDamage());
			}
			other->Destroy();
			return;
		}
		if (other->getTag() != Tag::Player && other->getTag() != Tag::Boss && gameObject->getTransform().translation().Z > -GameSettings::WORLD_DEPTH / 2) {
			other->Destroy();
		}
	}
};

