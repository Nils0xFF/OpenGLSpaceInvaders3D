#pragma once
#include "Component.h"

class Component;

class Collider : public Component
{
	public:
		virtual void test() = 0;
};

