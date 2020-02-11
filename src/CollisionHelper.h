#pragma once
#include "Aabb.h"
class CollisionHelper
{
	static bool pointInBox(Vector box1, AABB box2);
	static bool detectAABBCollision(AABB box1, AABB box2);

	static bool pointInSphere();
	static bool detectSphereToAABBColission(){}
};

