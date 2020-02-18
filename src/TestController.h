#pragma once

#include "Component.h"
#include "Matrix.h"
#include "GameObject.h"
#define _USE_MATH_DEFINES
#include <math.h>

class TestController : public Component
{
public:
	float speed = 1.0f;
	TestController() {};
	TestController(const TestController& other) = default;

	TestController* clone() { return new TestController(*this); }

	void Update(float deltaTime) {
		gameObject->rotate(Matrix().rotationZ(0.1f * M_PI * deltaTime));
	}
};

