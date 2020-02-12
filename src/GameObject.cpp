#include "GameObject.h"

void GameObject::Update(float deltaTime)
{
	if (mr != NULL) mr->Update(deltaTime);

	for (Component* c : this->components) {
		c->Update(deltaTime);
	}

	for (GameObject* g : this->children) {
		g->setTransform(this->transform * g->getTransform());
	}
}
