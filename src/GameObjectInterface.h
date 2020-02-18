#pragma once
#include "Matrix.h"
#include "LifecycleObject.h"
#include "Aabb.h"

extern class MeshRenderer;

class GameObjectInterface: public LifecycleObject
{
	public:
		virtual const Matrix& getTransform() const = 0;
		virtual void moveTo(const Matrix& m) = 0;
		virtual void translate(const Vector& delta) = 0;
		virtual void rotate(const Matrix& rot) = 0;
		virtual const MeshRenderer* getRenderer() = 0;
		virtual const AABB* getAreaBox() = 0;

};

