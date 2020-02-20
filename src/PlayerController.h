#pragma once
#include "Component.h"
#include "InputManager.h"
#include <GLFW\glfw3.h>
#include "GameSettings.h"
#include "SceneManager.h"
#include "BulletController.h"

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
	float timeBetweenShots = 1.0f;
	float timeSinceLastShot = 0.0f;

	void fire() {
		std::cout << "Spawning Bullet @" << gameObject->getTransform().translation() + Vector::forward() << std::endl;
		GameObject* bulletInstance = bullet->instantiate(gameObject->getTransform().translation() + Vector::forward());
		BulletController* bc = bulletInstance->getComponentByType<BulletController>();

		if (bc != NULL) {
			bc->setDirection(Vector::forward());
			bc->setSpeed(2.0f);
			bc->setDamage(1);
		}
	}

public:

	PlayerController(Prefab* bullet) :bullet(bullet) {};

	~PlayerController() {
		delete bullet;
	}

	void Update(float deltaTime) {

		float xMovement = 0.0f;

		if (InputManager::getInstance().Keys[GLFW_KEY_A] && gameObject->getAreaBox()->Min.X > -GameSettings::WORLD_WITH / 2)
		{
			xMovement -= GameSettings::PLAYER_SPEED;
		}

		if (InputManager::getInstance().Keys[GLFW_KEY_D] && gameObject->getAreaBox()->Max.X < GameSettings::WORLD_WITH / 2)
		{
			xMovement += GameSettings::PLAYER_SPEED;
		}

		timeSinceLastShot += deltaTime;

		if (InputManager::getInstance().Keys[GLFW_KEY_SPACE] && timeSinceLastShot >= timeBetweenShots)
		{
			fire();
			timeSinceLastShot = 0;
		}

		deltaTranslate = Vector(xMovement, 0.0f, 0.0f);

		gameObject->translate(deltaTranslate * deltaTime);

		// deltaTranslate = deltaTranslate - deltaTranslate * deltaTime;

	}

	void onCollision(GameObject* other) {
		if (other->getTag() == Tag::EnemyBullet) {
			std::cout << "Player HIT by EnemyBullet" << std::endl;
			BulletController* bc = other->getComponentByType<BulletController>();
			if (bc != NULL) {
				this->currentHP -= bc->getDamage();
				std::cout << "HP after HIT by EnemyBullet" << this->currentHP << std::endl;
				if (this->currentHP <= 0) gameObject->Destroy();

				other->Destroy();
			}
		}
	}

	PlayerController* clone() { return new PlayerController(*this); }

};

