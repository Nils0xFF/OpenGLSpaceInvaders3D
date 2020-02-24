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

	void fire() {
		std::cout << "Spawning Bullet @" << gameObject->getTransform().translation() + Vector::forward() << std::endl;
		GameObject* bulletInstance = bullet->instantiate(gameObject->getTransform().translation() + Vector::forward());
		BulletController* bc = bulletInstance->getComponentByType<BulletController>();

		if (bc != NULL) {
			bc->setDirection(Vector::forward());
			bc->setSpeed(GameSettings::PLAYER_BULLET_SPEED);
			bc->setDamage(GameSettings::PLAYER_BULLET_DAMAGE);
		}
	}

	void takeDamage(int damage) {
		this->currentHP -= damage;
		GameManager::getInstance().updatePlayerHP(currentHP);
		if (this->currentHP <= 0) {
			currentHP = 0;
			gameObject->setActive(false);
			GameManager::getInstance().PlayerDied();
		}
	}

public:

	PlayerController(Prefab* bullet) :bullet(bullet) {};

	~PlayerController() {
		delete bullet;
	}

	void Init() {
		gameObject->setActive(true);
		currentHP = GameSettings::PLAYER_HP;
	}

	void Start() {
		GameManager::getInstance().updatePlayerHP(currentHP);
	}

	void Update(float deltaTime) {

		// float xMovement = 0.0f;

		if (InputManager::getInstance().Keys[GLFW_KEY_A])
		{
			if (gameObject->getAreaBox()->Min.X > -GameSettings::WORLD_WIDTH / 2.0f) {
				if (deltaTranslate.X > -GameSettings::PLAYER_MAX_SPEED)
					deltaTranslate.X -= GameSettings::PLAYER_ACCEL * deltaTime;
			}
			else {
				deltaTranslate.X = 0;
			}
		}

		if (InputManager::getInstance().Keys[GLFW_KEY_D])
		{
			if (gameObject->getAreaBox()->Max.X < GameSettings::WORLD_WIDTH / 2.0f) {
				if (deltaTranslate.X < GameSettings::PLAYER_MAX_SPEED)
					deltaTranslate.X += GameSettings::PLAYER_ACCEL * deltaTime;
			}
			else {
				deltaTranslate.X = 0;
			}
		}

		timeSinceLastShot += deltaTime;

		if (InputManager::getInstance().Keys[GLFW_KEY_SPACE] && timeSinceLastShot >= timeBetweenShots)
		{
			fire();
			timeSinceLastShot = 0;
		}

		// deltaTranslate += Vector(xMovement, 0.0f, 0.0f);
		gameObject->translate(deltaTranslate * deltaTime);

		deltaTranslate = deltaTranslate - deltaTranslate * 5 * deltaTime;

	}

	void onCollision(GameObject* other) {
		if (other->getTag() == Tag::EnemyBullet) {
			BulletController* bc = other->getComponentByType<BulletController>();
			if (bc != NULL) {
				takeDamage(bc->getDamage());
				other->Destroy();
			}
		}
		if (other->getTag() == Tag::Enemy) {
			takeDamage(1);
			other->Destroy();
		}
		if (other->getTag() == Tag::Meteor) {
			takeDamage(GameSettings::METEOR_DAMAGE);
			other->Destroy();
		}
	}

	PlayerController* clone() { return new PlayerController(*this); }

};

