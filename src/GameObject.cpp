#include "GameObject.h"

void GameObject::Update(float deltaTime)
{
	if (mr != NULL) mr->Update(deltaTime);

	for (Component* c : this->components) {
		c->Update(deltaTime);
	}

	moveTo(Matrix().translation(this->transform.translation() + this->deltaTranslation) * Matrix().translation(this->transform.translation()).invert() * this->transform  * deltaRotation);

	areaBox = *modelBox;
	for (GameObject* g : this->children) {
		g->translate(deltaTranslation);
		g->rotate(deltaRotation);
		g->Update(deltaTime);

		areaBox.merge(*(g->modelBox));
	}

	deltaRotation = Matrix().identity();
	deltaTranslation = Vector::zero();




}
