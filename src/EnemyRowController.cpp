#include "EnemyRowController.h"

void EnemyRowController::Update(float deltaTime) {
	if (gameObject->getTransform().translation().Z > 2) {
		gameObject->Destroy();
		GameManager::getInstance().RowDestroyed();
	}
	if (xDir < 0 && gameObject->getAreaBox()->Min.X < -GameSettings::WORLD_WIDTH / 2.0f) {
		xDir *= -1;
	}
	if (xDir > 0 && gameObject->getAreaBox()->Max.X > GameSettings::WORLD_WIDTH / 2.0f) {
		xDir *= -1;
	}
	if (gameObject->getTransform().translation().Z > -2) {
		xDir = 0;
	}
	gameObject->translate((-Vector::forward() * zSpeed + Vector::right() * xSpeed * xDir) * deltaTime);
}
