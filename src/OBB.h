#pragma once
#include "vector.h"
#include "Aabb.h"
#include <algorithm>
#include <iterator>

class OBB
{
private:
	void calculatePoints() {
		verts[0] = center + right * -halfSize +	up * -halfSize +	forward * -halfSize;
		verts[1] = center + right * halfSize +	up * -halfSize +	forward * -halfSize;
		verts[2] = center + right * -halfSize +	up * halfSize +		forward * -halfSize;
		verts[3] = center + right * halfSize +	up * halfSize +		forward * -halfSize;
		verts[4] = center + right * -halfSize +	up * -halfSize +	forward * halfSize;
		verts[5] = center + right * halfSize +	up * -halfSize +	forward * halfSize;
		verts[6] = center + right * -halfSize +	up * halfSize +		forward * halfSize;
		verts[7] = center + right * halfSize +	up * halfSize +		forward * halfSize;
	}

protected:
	Vector right, up, forward, center, halfSize;
	Vector verts[8];
	const AABB& boundingBox;

public:

	OBB(const AABB& boundingBox):boundingBox(boundingBox) {
		halfSize = boundingBox.size() * 0.5f;
		center = boundingBox.center();
		right = Vector(1, 0, 0);
		up = Vector(0, 1, 0);
		forward = Vector(0, 1, 0);
		calculatePoints();
	}

	void corners(Vector c[8]) const {
		for(int i = 0; i < 8; i++)
		{
			c[i] = verts[i];
		}
	};

	void transform(const Matrix& transform) {
		center = boundingBox.center();
		right = transform.right().normalize();
		up = transform.up().normalize();
		forward = transform.forward().normalize();


		calculatePoints();
	}

	void scale(Vector& scaling) {
		halfSize = halfSize * scaling;

		calculatePoints();
	}


};

