#include "CollisionHelper.h"

bool CollisionHelper::pointInBox(Vector point, AABB box)
{
	return (point.X >= box.Min.X && point.X <= box.Max.X) &&
		(point.Y >= box.Min.Y && point.Y <= box.Max.Y) &&
		(point.Z >= box.Min.Z && point.Z <= box.Max.Z);
}

bool CollisionHelper::detectAABBCollision(AABB a, AABB b)
{
	return (a.Min.X <= b.Max.X && a.Max.X >= b.Min.X) &&
		(a.Min.Y <= b.Max.Y && a.Max.Y >= b.Min.Y)&&
		(a.Min.Z <= b.Max.Z && a.Max.Z >= b.Min.Z);
}
