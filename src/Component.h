#pragma once

class GameObject;

class Component
{
protected:
	GameObject* gameObject;

public:
	Component() :gameObject(nullptr) {};
	Component(GameObject* gameObject):gameObject(gameObject) {};
	~Component() {};

	virtual void Init() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void Destroy() {};
};

