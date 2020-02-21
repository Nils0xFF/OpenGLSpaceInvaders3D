#pragma once
#include "vector.h"
#include "Aabb.h"
#include <algorithm>
#include <iterator>

class OBB
{
private:
	void calculatePoints() {
		verts[0] = center + right * -halfSize.X +	up * -halfSize.Y +	forward * -halfSize.Z;
		verts[1] = center + right * halfSize.X +	up * -halfSize.Y +	forward * -halfSize.Z;
		verts[2] = center + right * -halfSize.X +	up * halfSize.Y +		forward * -halfSize.Z;
		verts[3] = center + right * halfSize.X +	up * halfSize.Y +		forward * -halfSize.Z;
		verts[4] = center + right * -halfSize.X +	up * -halfSize.Y +	forward * halfSize.Z;
		verts[5] = center + right * halfSize.X +	up * -halfSize.Y +	forward * halfSize.Z;
		verts[6] = center + right * -halfSize.X +	up * halfSize.Y +		forward * halfSize.Z;
		verts[7] = center + right * halfSize.X +	up * halfSize.Y +		forward * halfSize.Z;
	}

protected:
	Vector right, up, forward, center, offset, halfSize;
	Vector verts[8];

public:

	OBB(const AABB& boundingBox) {
		halfSize = boundingBox.size() * 0.5f;
		//std::cout << "BB: " << boundingBox.center() << " TT: " << transform.translation() << std::endl;
		center = boundingBox.center();
		offset = Vector::zero();

		right = Vector(1,0,0);
		up = Vector(0,1,0);
		forward = Vector(0,0,-1);
		boundingBox.corners(verts);
	}

	OBB(const AABB& boundingBox, const Matrix& transform){
		halfSize = boundingBox.size() * 0.5f;
		center = boundingBox.center();
		offset = boundingBox.center() - transform.translation();
		std::cout << "BB: " << boundingBox.center() << " TT: " << transform.translation() << " Offset: " << offset  << std::endl;

		std::cout << "RI:" << transform.right().normalize() << std::endl << " UP: " << transform.up().normalize() << std::endl << "FW:" << transform.forward().normalize() << std::endl << std::endl;
		right = transform.right();
		up = transform.up();
		forward = transform.forward();

		calculatePoints();
	}

	void corners(Vector c[8]) const {
		for(int i = 0; i < 8; i++)
		{
			c[i] = verts[i];
		}
	};

	void transform(const Matrix& transform) {
		right = transform.right().normalize();
		up = transform.up().normalize();
		forward = transform.forward().normalize();
		center = transform.translation() + up * offset.Y + right * offset.X + forward * offset.Z;

		calculatePoints();
	}

	void transformNoCenter(const Matrix& transform) {
		right = transform.right().normalize();
		up = transform.up().normalize();
		forward = transform.forward().normalize();
		center = transform.translation() + up * halfSize.Y;

		calculatePoints();
	}

	void scale(Vector& scaling) {
		halfSize = halfSize * scaling;

		calculatePoints();
	}

	const Vector& getRight() const { return right; }
	const Vector& getForward() const { return forward; }
	const Vector& getUp() const { return up; }

};

