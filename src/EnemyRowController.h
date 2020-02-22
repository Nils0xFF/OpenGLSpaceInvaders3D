#pragma once
#include "Component.h"
#include "GameObject.h"
#include "GameSettings.h"
#include "GameManager.h"

class EnemyRowController :
	public Component
{

private:
	float zSpeed = 0.5f;
	float xSpeed = 1.0f;
	short xDir = -1;

public:
	void Update(float deltaTime) {
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
		gameObject->translate((-Vector::forward() * zSpeed  + Vector::right() * xSpeed * xDir) * deltaTime);
	}

	EnemyRowController* clone() { return new EnemyRowController(*this); }
};

