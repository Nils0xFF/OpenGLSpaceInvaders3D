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
#include "Tags.h"


class GameObject : public GameObjectInterface
{
private:
	bool active = true;
	bool staticObject = false;

	std::string name = "";
	Tag tag = Tag::Default;

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
		transform.identity();
		debugShader.color(Color(0,1,0));
	};

	GameObject(const GameObject& other):mr(NULL), collider(NULL),debugModel(NULL) {
		this->transform = transform;
		this->active = other.active;
		this->staticObject = other.staticObject;
		this->name = other.name;
		this->parent = NULL;
		debugShader.color(Color(0, 1, 0));

		for (GameObject* g : other.children) {
			this->addChild(new GameObject(*g));
		}

		for (Component* c : other.components) {
			if (dynamic_cast<MeshRenderer*>(c)) {
				setRenderer(dynamic_cast<MeshRenderer*>(c)->clone());
				continue;
			}
			if (dynamic_cast<Collider*>(c)) {
				setCollider(dynamic_cast<Collider*>(c)->clone());
				continue;
			} 
			this->addComponent(c->clone());
		}
	}

	~GameObject() {
		while (!components.empty()) {
			delete components.front();
			components.pop_front();
		}
	};

	void Init() {
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
		std::cout << "Kollision! " << name << " " << other->name << std::endl;
	};
	void onTrigger() {};

	// getter/setter
	bool isActive() const { return active; }
	void setActive(const bool active) { this->active = active; }
	bool isStatic() const { return staticObject; }
	void setStatic(const bool staticObject) { this->staticObject = staticObject; }
	const std::string& getName() { return name; }
	void setTag(Tag tag) { this->tag = tag; }
	const Tag getTag() const { return tag; }

	void setName(const std::string name) { this->name = name; }
	const Matrix& getTransform() const { return this->transform; }
	void setTransform(const Matrix& transform) {
		this->transform = transform;
	}

	void moveTo(const Matrix& transform) { 
		if (staticObject) {
			std::cout << "Trying to move static Object" << std::endl;
			return;
		}
		this->transform = transform;
		for (Component* c : components) {
			c->updateTransform();
		}
	}

	const MeshRenderer* getRenderer() { return mr; }
	void setRenderer(MeshRenderer* mr) {
		if (this->mr != NULL) {
			std::cout << "Warning Overwriting existing Renderer!" << std::endl;
			delete this->mr;
		}
		this->mr = mr;
		this->addComponent(mr);
		this->areaBox = &(mr->model->boundingBox());
	}

	const Collider* getCollider() { return collider; }
	void setCollider(Collider* collider) {
		if (!this->mr) {
			std::cout << "Adding a Collider requires a Renderer!" << std::endl;
			return;
		}
		if (this->collider != NULL) {
			std::cout << "Warning Overwriting existing Collider!" << std::endl;
			delete this->collider;
		}
		this->collider = collider;
		this->addComponent(collider);
	}

	const AABB* getAreaBox() { return areaBox; }

	const GameObject* getParent() const { return parent; }
	void setParent(GameObject* p) { parent = p; }

	const std::list<Component*>& getComponents() const { return components; }
	void addComponent(Component* comp) {
		comp->setGameObject(this);
		this->components.push_back(comp);
	}

	const std::list<GameObject*>& getChildren() const { return children; }
	void addChild(GameObject* g) { 
		g->setParent(this); 
		this->children.push_back(g);
	}

	template <typename T>
	T* getComponentByType() {
		for (Component* c : components) {
			if (dynamic_cast<T*>(c)) {
				return dynamic_cast<T*>(c);
			}
		}
	}

	template <typename T>
	std::list<T*> getComponentsByType() {
		std::list<T*> componentList;
		for (Component* c : components) {
			if (dynamic_cast<T*>(c)) {
				myList.push_back(dynamic_cast<T*>(c));
			}
		}
		return componentList;
	}

};

