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

// adapted from: https://gamedev.stackexchange.com/questions/44500/how-many-and-which-axes-to-use-for-3d-obb-collision-with-sat
bool CollisionHelper::detectOBBCollisions(const OBB& a, const OBB& b)
{
	Vector aVerts[8], bVerts[8];
	a.corners(aVerts);
	b.corners(bVerts);
	
	if (Seperated(aVerts, bVerts, a.getRight()))
		return false;

	if (Seperated(aVerts, bVerts, a.getUp()))
		return false;

	if (Seperated(aVerts, bVerts, a.getForward()))
		return false;

	if (Seperated(aVerts, bVerts, b.getRight()))
		return false;
	if (Seperated(aVerts, bVerts, b.getUp()))
		return false;
	if (Seperated(aVerts, bVerts, b.getForward()))
		return false;

	if (Seperated(aVerts, bVerts, a.getRight().cross(b.getRight())))
		return false;
	if (Seperated(aVerts, bVerts, a.getRight().cross(b.getUp())))
		return false;
	if (Seperated(aVerts, bVerts, a.getRight().cross(b.getForward())))
		return false;

	if (Seperated(aVerts, bVerts, a.getUp().cross(b.getRight())))
		return false;
	if (Seperated(aVerts, bVerts, a.getUp().cross(b.getUp())))
		return false;
	if (Seperated(aVerts, bVerts, a.getUp().cross(b.getForward())))
		return false;

	if (Seperated(aVerts, bVerts, a.getForward().cross(b.getRight())))
		return false;
	if (Seperated(aVerts, bVerts, a.getForward().cross(b.getUp())))
		return false;
	if (Seperated(aVerts, bVerts, a.getForward().cross(b.getForward())))
		return false;


	return true;
}

// adapted from: https://gamedev.stackexchange.com/questions/44500/how-many-and-which-axes-to-use-for-3d-obb-collision-with-sat
bool CollisionHelper::Seperated(const Vector vertsA[], const Vector vertsB[], const Vector& axis)
{
	// handels zero vector
	if (axis.lengthSquared() <= 0)
		return false;

	float aMin = FLT_MAX;
	float aMax = FLT_MIN;
	float bMin = FLT_MAX;
	float bMax = FLT_MIN;

	// project verts on axis and calulate the size of the interval
	for (int i = 0; i < 8; i++) {
		float aDist = vertsA[i].dot(axis);
		aMin = std::min(aMin,aDist);
		aMax = std::max(aMax, aDist);
		float bDist = vertsB[i].dot(axis);
		bMin = std::min(bMin, bDist);
		bMax = std::max(bMax, bDist);
	}

	// 1D Intersection test on axis
	float longSpan = std::max(aMax, bMax) - std::min(aMin, bMin);
	float sumSpan = aMax - aMin + bMax - bMin;
	return longSpan >= sumSpan;
}

