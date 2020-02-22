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
	ConstantShader debugShader;

	std::list<Component*> components;
	std::list<GameObject*> children;

public:
	GameObject():active(true),staticObject(false),name(""),mr(NULL),collider(NULL),parent(NULL),modelBox(NULL),areaBox(),debugShader(),debugModel(NULL),tag(Tag::Default) {
		transform.identity();
		debugShader.color(Color(0,1,0));
	};

	GameObject(const GameObject& other):mr(NULL), collider(NULL),debugModel(NULL), modelBox(NULL), destroy(false) {
		this->transform = other.transform;
		this->active = other.active;
		this->staticObject = other.staticObject;
		this->name = other.name;
		this->tag = other.tag;
		this->parent = NULL;
		this->areaBox = AABB(this->transform.translation(), this->transform.translation());
		debugShader.color(Color(0, 1, 0));

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

		for (GameObject* g : other.children) {
			this->addChild(new GameObject(*g));
		}
	}

	~GameObject() {
		
		// std::cout << "~GO()" << std::endl;

		delete debugModel;

		while (!components.empty()) {
			delete components.front();
			components.pop_front();
		}

		while (!children.empty()) {
			delete children.front();
			children.pop_front();
		}
	};

	void Init() {
		deltaRotation = Matrix().identity();
		deltaTranslation = Vector(0, 0, 0);
		for (Component* c : this->components) {
			c->Init();
			c->updateTransform();
		}
		for (GameObject* g : this->children) {
			g->setTransform(g->getTransform() * transform);
			g->Init();
		}
	};

	void Start() {
		for (Component* c : this->components) {
			c->Start();
		}
		for (GameObject* g : this->children) {
			g->Start();
		}
	};

	void Update(float deltaTime);

	void Draw() {
		#if _DEBUG
			if (debugModel != NULL) delete debugModel;
			debugModel = new LineBoxModel(areaBox.Min, areaBox.Max);
			debugModel->shader(&debugShader, false);
			debugModel->shadowCaster(false);
			debugModel->draw(*CameraManager::getInstance().activeCamera);
		#endif

		for (Component* c : this->components) {
			c->Draw();
		}

		for (GameObject* g : this->children) {
			g->Draw();
		}
	};

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

	void translate(const Vector& delta) {
		this->deltaTranslation += delta;
	}

	void rotate(const Matrix& rot) {
		this->deltaRotation *= rot;
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
			components.remove(this->mr);
			this->mr = NULL;
		}
		this->mr = mr;
		this->addComponent(mr);
		this->modelBox = &(mr->model->boundingBox());
		areaBox = *modelBox;
	}

	const Collider* getCollider() { return collider; }
	void setCollider(Collider* collider) {
		if (this->collider != NULL) {
			std::cout << "Warning Overwriting existing Collider!" << std::endl;
			delete this->collider;
			components.remove(this->collider);
			this->collider = NULL;
		}
		this->collider = collider;
		this->addComponent(collider);
	}

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

