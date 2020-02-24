#include "PlayerController.h"

// Handles player-bullet-movement
void PlayerController::fire() {
	GameObject* bulletInstance = bullet->instantiate(gameObject->getTransform().translation() + Vector::forward());
	BulletController* bc = bulletInstance->getComponentByType<BulletController>();

	if (bc != NULL) {
		bc->setDirection(Vector::forward());
		bc->setSpeed(GameSettings::PLAYER_BULLET_SPEED);
		bc->setDamage(GameSettings::PLAYER_BULLET_DAMAGE);
	}
}

// Reduces player-health based on damage
void PlayerController::takeDamage(int damage) {
	this->currentHP -= damage;
	GameManager::getInstance().updatePlayerHP(currentHP);
	if (this->currentHP <= 0) {
		currentHP = 0;
		gameObject->setActive(false);
		GameManager::getInstance().PlayerDied();
	}
}

// Handles player-movement based on key-input
void PlayerController::Update(float deltaTime) {

	if (InputManager::getInstance().Keys[GLFW_KEY_A])
	{
		if (gameObject->getAreaBox()->Min.X > -GameSettings::WORLD_WIDTH / 2.0f) {
			if (deltaTranslate.X > -GameSettings::PLAYER_MAX_SPEED)
				deltaTranslate.X -= GameSettings::PLAYER_ACCEL * deltaTime;
		}
		else {
			deltaTranslate.X = 0;
		}
	}

	if (InputManager::getInstance().Keys[GLFW_KEY_D])
	{
		if (gameObject->getAreaBox()->Max.X < GameSettings::WORLD_WIDTH / 2.0f) {
			if (deltaTranslate.X < GameSettings::PLAYER_MAX_SPEED)
				deltaTranslate.X += GameSettings::PLAYER_ACCEL * deltaTime;
		}
		else {
			deltaTranslate.X = 0;
		}
	}

	timeSinceLastShot += deltaTime;

	if (InputManager::getInstance().Keys[GLFW_KEY_SPACE] && timeSinceLastShot >= timeBetweenShots)
	{
		fire();
		timeSinceLastShot = 0;
	}

	gameObject->translate(deltaTranslate * deltaTime);
	deltaTranslate = deltaTranslate - deltaTranslate * 5 * deltaTime;

}

void PlayerController::onCollision(GameObject* other) {
	if (other->getTag() == Tag::EnemyBullet) {
		BulletController* bc = other->getComponentByType<BulletController>();
		if (bc != NULL) {
			takeDamage(bc->getDamage());
			other->Destroy();
		}
	}
	if (other->getTag() == Tag::Enemy) {
		takeDamage(1);
		other->Destroy();
	}
	if (other->getTag() == Tag::Meteor) {
		takeDamage(GameSettings::METEOR_DAMAGE);
		other->Destroy();
	}
}
