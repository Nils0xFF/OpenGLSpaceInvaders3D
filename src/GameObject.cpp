#include "GameObject.h"

void GameObject::Update()
{
	for (Component* c : this->components) {
		c->Update();
	}

	for (GameObject* g : this->children) {
		g->setTransform(this->transform * g->getTransform());
	}
}
