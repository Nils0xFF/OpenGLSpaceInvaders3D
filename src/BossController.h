#pragma once
#include "Component.h"
#include "Prefab.h"
#include "GameSettings.h"
#include "BulletController.h"
#include "Random.h"
#include "GameManager.h"

class BossController :
	public Component
{
private:
	int currentHP = GameSettings::BOSS_HP;
	Prefab* bulletPrefab;
	float xSpeed = GameSettings::BOSS_X_SPEED;
	float zSpeed = GameSettings::BOSS_Z_SPEED;
	short xDir = -1;
	short zDir = 1;
	float timeBetweenShots = 1.0f / GameSettings::BOSS_FIRERATE;
	float timeToNextShot = timeBetweenShots;
	float timeSinceLastShot = 0.0f;
	float shotVariation = GameSettings::BOSS_FIRERATE_VARIATION;

	void takeDamage(int damage);

	void fire();

public:
	BossController(Prefab* bulletPrefab) : bulletPrefab(bulletPrefab) {};

	BossController(const BossController& other) { this->bulletPrefab = other.bulletPrefab; }

	BossController* clone() { return new BossController(*this); }

	~BossController() { delete bulletPrefab; }

	void Start();

	void Update(float deltaTime);

	void onCollision(GameObject* other);

};

