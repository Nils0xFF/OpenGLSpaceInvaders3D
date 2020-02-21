#pragma once
#include "Component.h"
#include "GameObject.h"
class EnemyRowController :
	public Component
{

private:
	float speed = 1.0f;

public:
	void Update(float deltaTime) {
		std::cout << "translation: " << gameObject->getTransform().translation() << std::endl;
		std::cout << "Childs: " << gameObject->getChildren().size() << std::endl;
		if (gameObject->getTransform().translation().Z > 2) {
			gameObject->Destroy();
		}
		gameObject->translate(-Vector::forward() * speed * deltaTime);
	}

	EnemyRowController* clone() { return new EnemyRowController(*this); }
};

