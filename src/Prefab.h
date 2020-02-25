#pragma once
#include "GameObject.h"
#include "SceneManager.h"

class Prefab
{
private:
	GameObject* prefab;

public:
	Prefab(GameObject* prefab) :prefab(prefab) {};
	Prefab(const Prefab& other) :prefab(new GameObject(*(other.prefab))) {};
	~Prefab() { delete prefab; }

	GameObject* instantiate(const Vector& position = Vector::zero(), const Matrix& rotation = Matrix().identity(), std::string name = "");;

	GameObjectInterface* getGameObject() { return prefab; }
};

