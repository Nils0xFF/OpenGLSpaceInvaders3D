#pragma once
#include "Component.h"

class Component;

class Collider : public Component
{
	public:
		virtual bool checkCollision(const Collider* other) const = 0;
		virtual void Start() {};
		virtual void Update(float deltaTime) {};
		virtual void updateBoundingVolume() {};
};

