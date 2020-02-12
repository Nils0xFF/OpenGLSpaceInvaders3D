#pragma once
#include <string>
#include "Matrix.h"
#include "Component.h"
#include "LifecycleObject.h"
#include "Collider.h"
#include "MeshRenderer.h"
#include <vector>
#include <list>

class GameObject : public GameObjectInterface
{
private:
	bool active = true;
	bool staticObject = false;

	std::string name = "";
	Matrix transform;
	GameObject* parent;

	MeshRenderer* mr;
	Collider* collider;

	std::list<Component*> components;
	std::list<GameObject*> children;

public:
	GameObject():active(true),staticObject(false),name(""),mr(NULL),collider(NULL),parent(NULL) {};
	~GameObject() {
		while (!components.empty()) {
			delete components.front();
			components.pop_front();
		}
	};

	void Init() {
		transform.identity();
		for (Component* c : this->components) {
			c->Init();
		}
	};

	void Start() {
		for (Component* c : this->components) {
			c->Start();
		}
	};
	void Update(float deltaTime);

	void Draw() {
		for (Component* c : this->components) {
			c->Draw();
		}
	};
	void Destroy() {};

	void onCollision(GameObject* other) {};
	void onTrigger() {};

	// getter/setter
	bool isActive() const { return active; }
	void setActive(const bool active) { this->active = active; }
	bool isStatic() const { return staticObject; }
	void setStatic(const bool staticObject) { this->staticObject = staticObject; }
	const std::string& getName() { return name; }
	void setName(const std::string name) { this->name = name; }
	const Matrix& getTransform() const { return this->transform; }
	void setTransform(const Matrix& transform) { 
		if (staticObject) {
			std::cout << "Trying to move static Object" << std::endl;
			return;
		}
		this->transform = transform;
	}

	const MeshRenderer* getRenderer() { return mr; }
	void setRenderer(MeshRenderer* mr) {
		if (this->mr != NULL) {
			std::cout << "Warning Overwriting existing Renderer!" << std::endl;
			delete this->mr;
		}
		mr->setGameObject(this);
		this->mr = mr;
		this->addComponent(mr);
	}

	const Collider* getCollider() { return collider; }
	void setCollider(Collider* collider) {
		if (this->collider != NULL) {
			std::cout << "Warning Overwriting existing Collider!" << std::endl;
			delete this->collider;
		}
		collider->setGameObject(this);
		this->collider = collider;
		this->addComponent(collider);
	}

	void addComponent(Component* comp) {
		comp->setGameObject(this);
		this->components.push_back(comp);
	}
};

