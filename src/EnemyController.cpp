#include "EnemyController.h"

// handles enemy-bullet-movement
void EnemyController::fire() {
	GameObject* bulletInstance = bullet->instantiate(
		Vector(
			gameObject->getTransform().translation().X,
			gameObject->getTransform().translation().Y,
			gameObject->getAreaBox()->Max.Z + bullet->getGameObject()->getAreaBox()->size().Z), bullet->getGameObject()->getTransform());
	BulletController* bc = bulletInstance->getComponentByType<BulletController>();

	if (bc != NULL) {
		bc->setDirection(-Vector::forward());
		bc->setSpeed(GameSettings::ENEMEY_BULLET_SPEED);
		bc->setDamage(GameSettings::ENEMEY_BULLET_DAMAGE);
	}
}

EnemyController::EnemyController(int maxHP, float timeBetweenShots, Prefab* bullet) {
	this->maxHP = maxHP;
	this->currentHP = maxHP;
	this->timeBetweenShots = timeBetweenShots;
	this->timeToNextShot = timeBetweenShots;
	this->bullet = bullet;
}

EnemyController::EnemyController(const EnemyController& other) {
	this->maxHP = other.maxHP;
	this->currentHP = other.maxHP;
	this->timeBetweenShots = other.timeBetweenShots;
	this->timeToNextShot = other.timeBetweenShots;
	this->bullet = new Prefab(*other.bullet);
}

void EnemyController::Update(float deltaTime) {
	timeSinceLastShot += deltaTime;
	if (timeSinceLastShot >= timeToNextShot) {
		fire();
		timeSinceLastShot = 0;
		timeToNextShot = timeBetweenShots + timeBetweenShots * shotVariation * (Random::random() - 0.5f);
	}
}

void EnemyController::onCollision(GameObject* other) {
	if (other->getTag() == Tag::PlayerBullet) {
		BulletController* bc = other->getComponentByType<BulletController>();
		if (bc != NULL) {
			this->currentHP -= bc->getDamage();
			if (this->currentHP <= 0) gameObject->Destroy();

			other->Destroy();
		}
	}
	if (other->getTag() == Tag::EnemyBullet) {
		other->Destroy();
	}
}
