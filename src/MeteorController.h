#pragma once
#include "Component.h"
#include "GameSettings.h"
#include "GameObject.h"
#include "BulletController.h"

class MeteorController :
	public Component
{
private:
	int currentHP = GameSettings::METEOR_HP;
	float speed = GameSettings::METEOR_Z_SPEED;

	void takeDamage(int damage);
public:
	MeteorController* clone() { return new MeteorController(*this); }

	void Update(float deltaTime);;

	void onCollision(GameObject* other);
};

