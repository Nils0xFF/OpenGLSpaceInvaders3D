#pragma once
#include <string>
#include <vector>
#include <list>

#include "Matrix.h"
#include "Component.h"
#include "LifecycleObject.h"
#include "Collider.h"
#include "MeshRenderer.h"
#include "ConstantShader.h"
#include "LineBoxModel.h"


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

	const AABB* areaBox;
	LineBoxModel* debugModel;
	ConstantShader debugShader;

	std::list<Component*> components;
	std::list<GameObject*> children;

public:
	GameObject():active(true),staticObject(false),name(""),mr(NULL),collider(NULL),parent(NULL),areaBox(),debugShader(),debugModel(NULL) {
		debugShader.color(Color(0,1,0));
	};

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
		#if _DEBUG
			if (debugModel != NULL) delete debugModel;
			debugModel = new LineBoxModel(areaBox->Min, areaBox->Max);
			debugModel->shader(&debugShader, false);
			if (debugModel != NULL) debugModel->draw(*CameraManager::getInstance().activeCamera);
		#endif
		for (Component* c : this->components) {
			c->Draw();
		}
	};
	void Destroy() {};

	void onCollision(GameObject* other) {
		std::cout << "Kollision!" << std::endl;
	};
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
		this->mr->updateTransform();
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
		this->areaBox = &(mr->model->boundingBox());
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

	const AABB* getAreaBox() { return areaBox; }

	void addComponent(Component* comp) {
		comp->setGameObject(this);
		this->components.push_back(comp);
	}
};

