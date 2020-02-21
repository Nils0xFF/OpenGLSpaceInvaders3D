#pragma once
#include "Component.h"

class Component;

class Collider : public Component
{
	public:
		virtual ~Collider() {};
		virtual bool checkCollision(const Collider* other) const = 0;
		virtual Collider* clone() = 0;
		virtual void Start() {};
		virtual void Update(float deltaTime) {};
		virtual void updateTransform() {};
};

