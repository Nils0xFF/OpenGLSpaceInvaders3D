#pragma once
#include "GameObject.h"
class Prefab
{
private:
	GameObject* prefab;

public:
	Prefab(GameObject* prefab) :prefab(prefab) {};
	~Prefab() { delete prefab; }
	GameObject* instantiate(Matrix& transform = Matrix().identity(), std::string name = "") {
		GameObject* toInstantiate = new GameObject(*prefab);
		toInstantiate->setTransform(transform);
		return toInstantiate;
	};
};

