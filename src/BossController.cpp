#include "BossController.h"

// Reduces boss-health by damage and handles corresponding death/gamestate
void BossController::takeDamage(int damage) {
	currentHP -= damage;
	GameManager::getInstance().updateBossHP(currentHP);
	if (currentHP <= 0) {
		currentHP = 0;
		gameObject->Destroy();
		GameManager::getInstance().BossDied();
	}
}

// handles boss-bullet-movement
void BossController::fire() {
	for (int x = -1; x <= 1; x++) {
		GameObject* bulletInstance = bulletPrefab->instantiate(
			Vector(
				gameObject->getTransform().translation().X + x * (gameObject->getAreaBox()->size().X / 2.0f),
				gameObject->getTransform().translation().Y,
				gameObject->getAreaBox()->Max.Z + bulletPrefab->getGameObject()->getAreaBox()->size().Z), bulletPrefab->getGameObject()->getTransform());
		BulletController* bc = bulletInstance->getComponentByType<BulletController>();

		if (bc != NULL) {
			bc->setDirection(-Vector::forward());
			bc->setSpeed(GameSettings::BOSS_BULLET_SPEED);
			bc->setDamage(GameSettings::BOSS_BULLET_DAMAGE);
		}
	}
}

void BossController::Start() { GameManager::getInstance().updateBossHP(currentHP); }

void BossController::Update(float deltaTime) {
	timeSinceLastShot += deltaTime;
	if (timeSinceLastShot >= timeToNextShot) {
		fire();
		timeSinceLastShot = 0;
		timeToNextShot = timeBetweenShots + timeBetweenShots * shotVariation * (Random::random() - 0.5f);
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

void BossController::onCollision(GameObject* other) {
	if (other->getTag() == Tag::PlayerBullet) {
		BulletController* bc = other->getComponentByType<BulletController>();
		if (bc) {
			takeDamage(bc->getDamage());
		}
		other->Destroy();
	}
}
