#include "GameObject.h"

using namespace std;

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
