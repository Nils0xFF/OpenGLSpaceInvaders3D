#include "BoxCollider.h"
#include <typeinfo>
#include "CollisionHelper.h"

bool BoxCollider::checkCollision(const Collider& other) const
{
	if (typeid(other) == typeid(BoxCollider)) {
		const BoxCollider& otherBox = dynamic_cast<const BoxCollider&>(other);
		// CollisionHelper::detectAABBCollision(this->collsionBox, otherBox.collsionBox);
	}
	return false;
}
