#pragma once
#include "Component.h"
#include "Matrix.h"
#include "GameObject.h"

class TestController : public Component
{
public:
	void Update(float deltaTime) {
		float speed = 1.0f;
		gameObject->setTransform(gameObject->getTransform() * Matrix().translation(Vector(0,1.0f * deltaTime,0)));
		std::cout << "Test: " << gameObject->getTransform().translation() << std::endl;
	}
};

