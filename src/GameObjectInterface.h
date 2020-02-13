#pragma once
#include "Matrix.h"
#include "LifecycleObject.h"
#include "Aabb.h"

extern class MeshRenderer;

class GameObjectInterface: public LifecycleObject
{
	public:
		virtual const Matrix& getTransform() const = 0;
		virtual void setTransform(const Matrix& m) = 0;
		virtual const MeshRenderer* getRenderer() = 0;
		virtual const AABB* getAreaBox() = 0;

};

