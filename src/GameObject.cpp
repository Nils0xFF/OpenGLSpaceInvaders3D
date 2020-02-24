#include "GameObject.h"

using namespace std;

GameObject::GameObject() :active(true), staticObject(false), name(""), mr(NULL), collider(NULL), parent(NULL), modelBox(NULL), areaBox(), debugShader(), debugModel(NULL), tag(Tag::Default) {
	transform.identity();
	debugShader.color(Color(0, 1, 0));
}

GameObject::GameObject(const GameObject& other) : mr(NULL), collider(NULL), debugModel(NULL), modelBox(NULL), destroy(false) {
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

GameObject::~GameObject() {
	delete debugModel;

	while (!components.empty()) {
		delete components.back();
		components.pop_back();
	}

	while (!children.empty()) {
		delete children.back();
		children.pop_back();
	}
}

void GameObject::Init() {
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
}

void GameObject::Start() {
	for (Component* c : this->components) {
		c->Start();
	}
	for (GameObject* g : this->children) {
		g->Start();
	}
}

// Handles lifecycle of all its components and scene-children
void GameObject::Update(float deltaTime)
{
	if (mr != NULL) mr->Update(deltaTime);

	for (Component* c : this->components) {
		c->Update(deltaTime);
	}

	moveTo(Matrix().translation(this->transform.translation() + this->deltaTranslation) * Matrix().translation(this->transform.translation()).invert() * this->transform  * deltaRotation);

	if (modelBox) {
		areaBox = *modelBox;
	}
	else if(!children.empty()){
		areaBox = *children.front()->getAreaBox();
	}
	else {
		areaBox = AABB(transform.translation(), transform.translation());
	}

	for (list<GameObject*>::iterator it = children.begin(); it != children.end();) {
		if ((*it)->isDeleted()) {
			delete (*it);
			it = children.erase(it);
			continue;
		}
		if ((*it)->isActive())
			(*it)->translate(deltaTranslation);
			(*it)->rotate(deltaRotation);
			(*it)->Update(deltaTime);
			areaBox.merge(*((*it)->modelBox));
		it++;
	}

	deltaRotation = Matrix().identity();
	deltaTranslation = Vector::zero();

}

void GameObject::Draw() {
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
}

void GameObject::setRenderer(MeshRenderer* mr) {
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

void GameObject::setCollider(Collider* collider) {
	if (this->collider != NULL) {
		std::cout << "Warning Overwriting existing Collider!" << std::endl;
		delete this->collider;
		components.remove(this->collider);
		this->collider = NULL;
	}
	this->collider = collider;
	this->addComponent(collider);
}
