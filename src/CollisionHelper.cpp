#include "CollisionHelper.h"

bool CollisionHelper::pointInBox(const Vector* point, const AABB* box)
{
	return (point->X >= box->Min.X && point->X <= box->Max.X) &&
		(point->Y >= box->Min.Y && point->Y <= box->Max.Y) &&
		(point->Z >= box->Min.Z && point->Z <= box->Max.Z);
}

bool CollisionHelper::detectAABBCollision(const AABB* a, const AABB* b)
{
	std::cout << a->Min << " - " << a->Max << std::endl;
	std::cout << b->Min << " - " << b->Max << std::endl << std::endl << std::endl;
	return (a->Min.X <= b->Max.X && a->Max.X >= b->Min.X) &&
		(a->Min.Y <= b->Max.Y && a->Max.Y >= b->Min.Y)&&
		(a->Min.Z <= b->Max.Z && a->Max.Z >= b->Min.Z);
}
