#pragma once
#include "GameObjectInterface.h"

extern class GameObject;

class Component
{
protected:
	GameObjectInterface* gameObject;

public:
	Component() :gameObject(nullptr) {};
	Component(GameObjectInterface* gameObject):gameObject(gameObject) {};
	~Component() { gameObject = NULL; }

	virtual void Init() {};
	virtual void Start() {};
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() {};
	virtual void Destroy() {};
	virtual Component* clone() = 0;

	virtual void setGameObject(GameObjectInterface* go) { this->gameObject = go; }
};

