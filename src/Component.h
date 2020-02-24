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

	// Called before Start() as first method in every component
	virtual void Init() {};
	// Called after Init() as second method in every component
	virtual void Start() {};
	// Called every frame with corresponding deltaTime
	virtual void Update(float deltaTime) = 0;
	// Called every frame
	virtual void Draw() {};
	// Used to mark Component for "garbage collection"
	virtual void Destroy() {};
	// Callback for changed transform
	virtual void updateTransform() {};

	// Callback for collisions
	virtual void onCollision(GameObject* other) {};
	// Used for deep copies of components (prefabs)
	virtual Component* clone() = 0;
	
	virtual void setGameObject(GameObject* go) { this->gameObject = go; }
};

