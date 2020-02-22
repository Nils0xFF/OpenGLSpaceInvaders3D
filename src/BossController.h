#pragma once
#include "Component.h"
#include "Prefab.h"
#include "GameSettings.h"
#include "BulletController.h"
#include "Random.h"
#include "GameManager.h"

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
	float timeBetweenShots = 1.0f / GameSettings::BOSS_FIRERATE;
	float timeToNextShot = timeBetweenShots;
	float timeSinceLastShot = 0.0f;
	float shotVariation = GameSettings::BOSS_FIRERATE_VARIATION;

	void takeDamage(int damage) {
		currentHP -= damage;
		GameManager::getInstance().updateBossHP(currentHP);
		if (currentHP <= 0) {
			currentHP = 0;
			gameObject->Destroy();
			GameManager::getInstance().BossDied();
		}
	}

	void fire() {
		for (int x = -1; x <= 1; x++) {
			GameObject* bulletInstance = bulletPrefab->instantiate(
				Vector(
					gameObject->getTransform().translation().X + x * (gameObject->getAreaBox()->size().X /  2.0f), 
					gameObject->getTransform().translation().Y, 
					gameObject->getAreaBox()->Max.Z + bulletPrefab->getGameObject()->getAreaBox()->size().Z));
			BulletController* bc = bulletInstance->getComponentByType<BulletController>();

			if (bc != NULL) {
				bc->setDirection(-Vector::forward());
				bc->setSpeed(GameSettings::BOSS_BULLET_SPEED);
				bc->setDamage(GameSettings::BOSS_BULLET_DAMAGE);
			}
		}
	}

public:
	BossController(Prefab* bulletPrefab) : bulletPrefab(bulletPrefab) {};

	BossController(const BossController& other) { this->bulletPrefab = other.bulletPrefab; }

	BossController* clone() { return new BossController(*this); }

	void Start() {
		GameManager::getInstance().updateBossHP(currentHP);
	}

	void Update(float deltaTime) {
		timeSinceLastShot += deltaTime;
		if (timeSinceLastShot >= timeToNextShot) {
			fire();
			timeSinceLastShot = 0;
			timeToNextShot = timeBetweenShots + timeBetweenShots * shotVariation * 2 * (Random::random() - 0.5f);
		}

		if (zDir < 0 && gameObject->getAreaBox()->Min.Z < -GameSettings::WORLD_DEPTH) {
			zDir *= -1;
		}
		if (zDir > 0 && gameObject->getAreaBox()->Max.Z > -(GameSettings::WORLD_DEPTH / 2.0f)) {
			zDir *= -1;
		}
		if (xDir < 0 && gameObject->getAreaBox()->Min.X < -GameSettings::WORLD_WIDTH / 2.0f) {
			xDir *= -1;
		}
		if (xDir > 0 && gameObject->getAreaBox()->Max.X > GameSettings::WORLD_WIDTH / 2.0f) {
			xDir *= -1;
		}
		gameObject->translate((-Vector::forward() * zDir * zSpeed + Vector::right() * xSpeed * xDir) * deltaTime);
	}

	void onCollision(GameObject* other) {
		if (other->getTag() == Tag::PlayerBullet) {
			BulletController* bc = other->getComponentByType<BulletController>();
			if (bc) {
				takeDamage(bc->getDamage());
			}
			other->Destroy();
		}
	}

};

