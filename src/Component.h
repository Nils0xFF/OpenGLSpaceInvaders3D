#pragma once
#include "GameObjectInterface.h"

class GameObject;

class Component
{
protected:
	GameObject* gameObject;

public:
	Component() :gameObject(nullptr) {};
	Component(GameObject* gameObject):gameObject(gameObject) {};
	virtual ~Component() { gameObject = NULL; }

	virtual void Init() {};
	virtual void Start() {};
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() {};
	virtual void Destroy() {};
	virtual void updateTransform() {};

	virtual void onCollision(GameObject* other) {};

	virtual Component* clone() = 0;

	virtual void setGameObject(GameObject* go) { this->gameObject = go; }
};

