#pragma once
#include <string>
#include "Matrix.h"
#include "Component.h"
#include "MeshRenderer.h"
#include "LifecycleObject.h"
#include <vector>
#include <list>

class GameObject : public LifecycleObject
{
private:
	bool active = true;
	bool staticObject = false;

	std::string name = "";
	Matrix transform;
	GameObject* parent;

	MeshRenderer* mr;

	std::list<Component*> components;
	std::list<GameObject*> children;

public:
	GameObject():active(true),staticObject(false),name(""),mr(NULL),parent(NULL) {};
	~GameObject() {
		delete mr;
		while (!components.empty()) {
			delete components.front();
			components.pop_front();
		}
	};

	void Init() {};
	void Start() {};
	void Update();

	void Draw() {
		if (mr != NULL) {
			mr->Draw();
		}
	};
	void Destroy() {};

	void onCollisionEnter() {};
	void onTriggerEnter() {};

	// getter/setter
	bool isActive() const { return active; }
	void setActive(const bool active) { this->active = active; }
	bool isStatic() const { return staticObject; }
	void setStatic(const bool staticObject) { this->staticObject = staticObject; }
	const std::string& getName() { return name; }
	void setName(const std::string name) { this->name = name; }
	const Matrix& getTransform() const { return this->transform; }
	void setTransform(const Matrix transform) { this->transform = transform; }

	const MeshRenderer* getRenderer() { return mr; }
	void setRenderer(MeshRenderer* mr) {
		if (this->mr != NULL) {
			std::cout << "Warning Overwriting existing Renderer!" << std::endl;
			delete this->mr;
		}
		this->mr = mr;
	}

	void addComponent(Component* comp) {
		this->components.push_back(comp);
	}
};

