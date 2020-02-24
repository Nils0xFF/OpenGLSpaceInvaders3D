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
	void Update(float deltaTime);

	EnemyRowController* clone() { return new EnemyRowController(*this); }
};

