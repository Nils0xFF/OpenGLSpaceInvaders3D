#pragma once
#include "Component.h"
#include "Prefab.h"
#include "GameSettings.h"
#include "BulletController.h"

class BossController :
	public Component
{
private:
	int currentHP = GameSettings::BOSS_HP;
	Prefab* bulletPrefab;
	float xSpeed = GameSettings::BOSS_X_SPEED;
	float zSpeed = GameSettings::BOSS_Z_SPEED;
	short xDir = -1;
	short zDir = 1;

	void takeDamage(int damage) {
		currentHP -= damage;
		if (currentHP <= 0) {
			currentHP = 0;
			gameObject->Destroy();
		}
	}

public:
	BossController(Prefab* bulletPrefab) : bulletPrefab(bulletPrefab) {};

	BossController(const BossController& other) { this->bulletPrefab = other.bulletPrefab; }

	BossController* clone() { return new BossController(*this); }

	void Update(float deltaTime) {
		if (zDir < 0 && gameObject->getAreaBox()->Min.Z < -GameSettings::WORLD_DEPTH) {
			zDir *= -1;
		}
		if (zDir > 0 && gameObject->getAreaBox()->Max.Z > -2) {
			zDir *= -1;
		}
		if (xDir < 0 && gameObject->getAreaBox()->Min.X < -GameSettings::WORLD_WITH / 2.0f) {
			xDir *= -1;
		}
		if (xDir > 0 && gameObject->getAreaBox()->Max.X > GameSettings::WORLD_WITH / 2.0f) {
			xDir *= -1;
		}
		gameObject->translate((Vector::forward() * zDir * zSpeed + Vector::right() * xSpeed * xDir) * deltaTime);
	}

	void onCollision(GameObject* other) {
		if (other->getTag() == Tag::PlayerBullet) {
			BulletController* bc = other->getComponentByType<BulletController>();
			if (bc) {
				takeDamage(bc->getDamage());
			}
		}
	}

};

