#pragma once
#include "Component.h"
#include "GameObject.h"
#include "BulletController.h"
#include "Prefab.h"
#include "Random.h"

class EnemyController : public Component
{

private:
	Prefab* bullet;
	int maxHP;
	int currentHP;
	float timeBetweenShots;
	float timeToNextShot;
	float timeSinceLastShot = 0.0f;
	float shotVariation = 0.25f;

	void fire();

public:
	EnemyController(int maxHP, float timeBetweenShots, Prefab* bullet);
	EnemyController(const EnemyController& other);
	~EnemyController() { delete bullet;	}
	EnemyController* clone() { return new EnemyController(*this); }

	void Update(float deltaTime);
	void onCollision(GameObject* other);
};

