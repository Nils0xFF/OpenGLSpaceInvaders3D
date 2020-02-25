#include "BulletController.h"

void BulletController::Start() {
	// If the bullet-controller is attached to a gameObject, which contains a ParticleGenerator
	// it will emit particles as long as the bullet exists
	if (gameObject) {
		ParticleGenerator* generator = gameObject->getComponentByType<ParticleGenerator>();
		if (generator) {
			generator->setEmiting(true);
		}
	}
}

void BulletController::Update(float deltaTime) {
	// Translates the bullet by its direction & speed every frame
	if (gameObject->getTransform().translation().Z < -GameSettings::WORLD_DEPTH || gameObject->getTransform().translation().Z > 2) {
		gameObject->Destroy();
	}
	gameObject->translate(direction * speed * deltaTime);
}

void BulletController::onCollision(GameObject* other) {
	// If a bullet hits another bullet, both will be destroyed
	if (other->getTag() == Tag::EnemyBullet || other->getTag() == Tag::PlayerBullet) {
		gameObject->Destroy();
		other->Destroy();
	}
}
