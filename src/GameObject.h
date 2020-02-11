#pragma once
#include <string>
#include "Matrix.h"
#include "Component.h"
#include "MeshRenderer.h"
#include <vector>
#include <list>

class GameObject
{
private:
	bool active;
	bool staticObject;

	std::string name;
	Matrix transform;
	GameObject* parent;

	std::list<Component*> components;
	std::list<GameObject*> children;

public:

	virtual void Init() {};
	virtual void Start() {};
	virtual void Update() {
		for (Component* c : this->components) {
			c->Update();
		}

		for (GameObject* g : this->children) {
			g->setTransform(this->transform * g->getTransform());
		}

	};
	virtual void Draw() {
		for (Component* c : this->components) {
			if (typeid(*c) == typeid(MeshRenderer)) {
				c->Draw();
			}
		}
	};
	virtual void Destroy() {};

	virtual void onCollisionEnter() {};
	virtual void onTriggerEnter() {};

	// getter/setter
	bool isActive() { return active; }
	void setActive(bool active) { this->active = active; }
	bool isStatic() { return staticObject; }
	void setStatic(bool staticObject) { this->staticObject = staticObject; }
	std::string getName() { return name; }
	void setName(std::string name) { this->name = name; }
	Matrix getTransform() { return this->transform; }
	void setTransform(Matrix transform) { this->transform = transform; }

};

