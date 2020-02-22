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
	~Prefab() { if(prefab) delete prefab; }

	GameObject* instantiate(const Vector& position = Vector::zero(), const Matrix& rotation = Matrix().identity(), std::string name = "") {
		assert(prefab);
		GameObject* toInstantiate = new GameObject(*prefab);
		toInstantiate->setTransform(Matrix().translation(position) * rotation);
		toInstantiate->Init();
		toInstantiate->Start();
		SceneManager::getInstance().activeScene->addGameObject(toInstantiate);
		return toInstantiate;
	};

	GameObjectInterface* getGameObject() {
		return prefab;
	}
};

