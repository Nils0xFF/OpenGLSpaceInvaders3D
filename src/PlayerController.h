#pragma once
#include "Component.h"
#include "InputManager.h"
#include <GLFW\glfw3.h>
#include "GameSettings.h"
#include "SceneManager.h"
#include "BulletController.h"
#include "GameManager.h"
#include "GameObject.h"

#include <math.h>
#include "Prefab.h"

class PlayerController :
	public Component
{

private:
	int maxHP = GameSettings::PLAYER_HP;
	int currentHP = GameSettings::PLAYER_HP;
	Prefab* bullet;
	Vector deltaTranslate = Vector::zero();
	float timeBetweenShots = 1.0f / GameSettings::PLAYER_FIRERATE;
	float timeSinceLastShot = timeBetweenShots;

	void fire();
	void takeDamage(int damage);

public:
	PlayerController(Prefab* bullet) :bullet(bullet) {};
	~PlayerController() { delete bullet; }

	void Init() {
		gameObject->setActive(true);
		currentHP = GameSettings::PLAYER_HP;
	}

	void Start() { GameManager::getInstance().updatePlayerHP(currentHP); }

	void Update(float deltaTime);

	void onCollision(GameObject* other);

	PlayerController* clone() { return new PlayerController(*this); }

};

