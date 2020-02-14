#include "BoxCollider.h"
#include <typeinfo>
#include "CollisionHelper.h"

using namespace std;

bool BoxCollider::checkCollision(const Collider* other) const
{
	if (dynamic_cast<const BoxCollider*>(other)) {
		const BoxCollider* otherBox = dynamic_cast<const BoxCollider*>(other);
		return CollisionHelper::detectOBBCollisions(*(this->collsionBox), *(otherBox->collsionBox));
	}
	return false;
}
