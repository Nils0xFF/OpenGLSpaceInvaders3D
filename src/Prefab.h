#pragma once
#include "GameObject.h"
#include "SceneManager.h"

class Prefab
{
private:
	GameObject* prefab;

public:
	Prefab(GameObject* prefab) :prefab(prefab) {};
	~Prefab() { delete prefab; }
	GameObject* instantiate(const Vector& position = Vector::zero(), const Matrix& rotation = Matrix().identity(), std::string name = "") {
		GameObject* toInstantiate = new GameObject(*prefab);
		toInstantiate->setTransform(Matrix().translation(position) * rotation * prefab->getTransform());
		toInstantiate->Init();
		toInstantiate->Start();
		SceneManager::getInstance().activeScene->addGameObject(toInstantiate);
		return toInstantiate;
	};
};

