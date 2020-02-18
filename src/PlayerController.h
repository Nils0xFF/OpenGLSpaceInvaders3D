#pragma once
#include "Component.h"
#include "InputManager.h"
#include <GLFW\glfw3.h>
class PlayerController :
	public Component
{
	void Update(float deltaTime) {

		float xMovement = 0.0f;
		float zMovement = 0.0f;

		if (InputManager::getInstance().Keys[GLFW_KEY_W])
		{
			zMovement -= 1.0f;
		}

		if (InputManager::getInstance().Keys[GLFW_KEY_S])
		{
			zMovement += 1.0f;
		}

		if (InputManager::getInstance().Keys[GLFW_KEY_A])
		{
			xMovement -= 1.0f;
		}

		if (InputManager::getInstance().Keys[GLFW_KEY_D])
		{
			xMovement += 1.0f;
		}

		gameObject->translate(Vector(xMovement, 0.0f, zMovement) * deltaTime);
	}

	PlayerController* clone() { return new PlayerController(*this); }
};

