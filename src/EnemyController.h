#pragma once
#include "Component.h"
#include "GameObject.h"
#include "BulletController.h"
#include "Prefab.h"

class EnemyController : public Component
{

private:
	Prefab* bullet;
	int maxHP;
	int currentHP;
	float timeBetweenShots;
	float timeToNextShot;
	float timeSinceLastShot = 0.0f;
	float shotVariation = 0.25f;

	void fire() {
		GameObject* bulletInstance = bullet->instantiate(gameObject->getTransform().translation() - Vector::forward());
		BulletController* bc = bulletInstance->getComponentByType<BulletController>();

		if (bc != NULL) {
			bc->setDirection(-Vector::forward());
			bc->setSpeed(2.0f);
			bc->setDamage(1);
		}
	}

public:
	EnemyController(int maxHP, float timeBetweenShots, Prefab* bullet) {
		this->maxHP = maxHP;
		this->currentHP = maxHP;
		this->timeBetweenShots = timeBetweenShots;
		this->timeToNextShot = timeBetweenShots;
		this->bullet = bullet;
	}

	EnemyController(const EnemyController& other) {
		this->maxHP = other.maxHP;
		this->currentHP = other.maxHP;
		this->timeBetweenShots = other.timeBetweenShots;
		this->timeToNextShot = other.timeBetweenShots;
		this->bullet = new Prefab(*other.bullet);
	}

	void Update(float deltaTime) {
		timeSinceLastShot += deltaTime;
		if (timeSinceLastShot >= timeToNextShot) {
			fire();
			timeSinceLastShot = 0;
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			timeToNextShot =  timeBetweenShots + shotVariation * (r - 0.5f);
		}
	}

	void onCollision(GameObject* other) {
		if (other->getTag() == Tag::PlayerBullet) {
			BulletController* bc = other->getComponentByType<BulletController>();
			if (bc != NULL) {
				this->currentHP -= bc->getDamage();
				if (this->currentHP <= 0) gameObject->Destroy();

				other->Destroy();
			}
		}
	}

	EnemyController* clone() { return new EnemyController(*this); }

};

