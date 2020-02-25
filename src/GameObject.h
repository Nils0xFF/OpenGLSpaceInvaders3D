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


class MeshRenderer;

class GameObject : public GameObjectInterface
{
private:
	bool active = true;
	bool staticObject = false;
	bool destroy = false;

	std::string name = "";
	Tag tag = Tag::Default;

	Vector deltaTranslation;
	Matrix deltaRotation;

	Matrix transform;
	Vector scale = Vector(1,1,1);

	GameObject* parent;

	MeshRenderer* mr;
	Collider* collider;

	const AABB* modelBox;
	AABB areaBox;
	LineBoxModel* debugModel;

	std::list<Component*> components;
	std::list<GameObject*> children;

public:
	GameObject();
	GameObject(const GameObject& other);
	~GameObject();
	
	void Init();
	void Start();
	void Update(float deltaTime);
	void Draw();
	void Destroy() { 
		this->active = false;
		this->destroy = true;
	}
	void onCollision(GameObject* other) {
		for (Component* c : this->components) {
			c->onCollision(other);
		}
	};
	void onTrigger() {};

	// getter/setter
	bool isActive() const { return active; }
	void setActive(const bool active) { this->active = active; }
	bool isStatic() const { return staticObject; }
	void setStatic(const bool staticObject) { this->staticObject = staticObject; }
	bool isDeleted() { return destroy; }

	const std::string& getName() { return name; }
	void setTag(Tag tag) { this->tag = tag; }
	const Tag getTag() const { return tag; }

	void setName(const std::string name) { this->name = name; }
	const Matrix& getTransform() const { return this->transform; }
	void setTransform(const Matrix& transform) {
		this->transform = transform;
	}

	void translate(const Vector& delta) { this->deltaTranslation += delta; }
	void rotate(const Matrix& rot) { this->deltaRotation *= rot; }
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
	void setRenderer(MeshRenderer* mr);
	const Collider* getCollider() { return collider; }
	void setCollider(Collider* collider);
	const AABB* getAreaBox() { return &areaBox; }
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

		return NULL;
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

