#pragma once

#include "Component.h"
#include "Matrix.h"
#include "GameObject.h"
#define _USE_MATH_DEFINES
#include <math.h>

class TestController : public Component
{
public:
	void Update(float deltaTime) {
		float speed = 1.0f;
		gameObject->setTransform(gameObject->getTransform() * Matrix().rotationY(0.1 * M_PI * deltaTime) * Matrix().translation(Vector(-0.5f * deltaTime, 0, 0)) * Matrix().rotationY(0.1 * M_PI * deltaTime));
	}
};

