#include "BoxCollider.h"
#include <typeinfo>
#include "CollisionHelper.h"
#include "GameObject.h"

using namespace std;

bool BoxCollider::checkCollision(const Collider* other) const
{
	if (dynamic_cast<const BoxCollider*>(other)) {
		const BoxCollider* otherBox = dynamic_cast<const BoxCollider*>(other);
		return CollisionHelper::detectOBBCollisions(this->collsionBox, otherBox->collsionBox);
	}
	return false;
}

void BoxCollider::Start() {
	if (gameObject->getRenderer()->model)
	{
		collsionBox = OBB(gameObject->getRenderer()->model->initialBoundingBox());
	}
	else {
		std::cout << "Warning:: Collider without Renderer!" << std::endl;
	}
	updateTransform();
}

void BoxCollider::updateTransform() {
	collsionBox.transform(gameObject->getTransform());
}
