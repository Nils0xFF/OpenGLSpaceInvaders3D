#pragma once
#include "Component.h"
#include "GameObject.h"
#include "GameSettings.h"
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
		}
		if (gameObject->getAreaBox()->Min.X > -GameSettings::WORLD_WITH / 2) {
			xDir *= -1;
		}
		if (gameObject->getAreaBox()->Max.X < GameSettings::WORLD_WITH / 2) {
			xDir *= -1;
		}
		gameObject->translate((-Vector::forward() * zSpeed  + Vector::right() * xSpeed * xDir) * deltaTime);
	}

	EnemyRowController* clone() { return new EnemyRowController(*this); }
};

