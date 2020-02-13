#pragma once
#include "Aabb.h"
class CollisionHelper
{
public:
	static bool pointInBox(const Vector* point, const AABB* box);
	static bool detectAABBCollision(const AABB* a, const AABB* b);

	static bool pointInSphere();
	static bool detectSphereCollision();

	static bool detectSphereToAABBCollission();
};

