#pragma once
#include "Aabb.h"
class CollisionHelper
{
public:
	static bool pointInBox(Vector point, AABB box);
	static bool detectAABBCollision(AABB a, AABB b);

	static bool pointInSphere();
	static bool detectSphereCollision();

	static bool detectSphereToAABBCollission();
};

