#pragma once
#include "Matrix.h"
#include "LifecycleObject.h"

class GameObjectInterface: public LifecycleObject
{
	public:
		virtual const Matrix& getTransform() const = 0;
		virtual void setTransform(const Matrix& m) = 0;
};

