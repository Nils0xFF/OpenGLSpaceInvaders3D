#include "MeteorController.h"

// Reduces meteor-health by damage
void MeteorController::takeDamage(int damage) {
	this->currentHP -= damage;
	if (this->currentHP <= 0) {
		currentHP = 0;
		gameObject->Destroy();
	}
}

void MeteorController::Update(float deltaTime) {
	if (gameObject->getTransform().translation().Z > 2) {
		gameObject->Destroy();
	}
	gameObject->translate(-Vector::forward() * speed * deltaTime);
}

void MeteorController::onCollision(GameObject* other) {
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
