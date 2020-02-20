#pragma once
#include "Component.h"
#include "GameObject.h"
#include "BulletController.h"

class EnemyController : public Component
{

private:
	int maxHP;
	int currentHP;
	/* float timeBetweenShots;
	float timeSinceLastShot;
	float shotVariation;*/

public:
	EnemyController(int maxHP) {
		this->maxHP = maxHP;
		this->currentHP = maxHP;
	}

	void Update(float deltaTime) {
		
	}

	void onCollision(GameObject* other) {
		if (other->getTag() == Tag::PlayerBullet) {
			std::cout << "Enemy HIT by PlayerBullet" << std::endl;
			BulletController* bc = other->getComponentByType<BulletController>();
			if (bc != NULL) {
				this->currentHP -= bc->getDamage();
				std::cout << "HP after HIT by PlayerBullet" << this->currentHP << std::endl;
				if (this->currentHP <= 0) gameObject->Destroy();

				other->Destroy();
			}
		}
	}

	EnemyController* clone() { return new EnemyController(*this); }

};

