#pragma once
#include "Aabb.h"
#include "OBB.h"

class CollisionHelper
{
public:
	static bool pointInBox(const Vector* point, const AABB* box);
	static bool detectAABBCollision(const AABB* a, const AABB* b);

	static bool pointInSphere();
	static bool detectSphereCollision();

	static bool detectSphereToAABBCollission();

	static bool detectOBBCollisions(const OBB& a, const OBB& b);
	static bool Seperated(const Vector vertsA[], const Vector vertsB[], const Vector& axis);
};

