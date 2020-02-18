#pragma once
#include "Component.h"
#include "InputManager.h"
#include <GLFW\glfw3.h>
#include "GameSettings.h"

#include <math.h>

class PlayerController :
	public Component
{

private:
	
	Vector deltaTranslate = Vector::zero();

public:
	void Update(float deltaTime) {

		float xMovement = 0.0f;

		if (InputManager::getInstance().Keys[GLFW_KEY_A] && gameObject->getAreaBox()->Min.X > -GameSettings::WORLD_WITH / 2)
		{
			xMovement -= GameSettings::PLAYER_SPEED;
		}

		if (InputManager::getInstance().Keys[GLFW_KEY_D] && gameObject->getAreaBox()->Max.X < GameSettings::WORLD_WITH / 2)
		{
			xMovement += GameSettings::PLAYER_SPEED;
		}

		deltaTranslate = Vector(xMovement, 0.0f, 0.0f);

		gameObject->translate(deltaTranslate * deltaTime);

		// deltaTranslate = deltaTranslate - deltaTranslate * deltaTime;

	}

	PlayerController* clone() { return new PlayerController(*this); }
};

