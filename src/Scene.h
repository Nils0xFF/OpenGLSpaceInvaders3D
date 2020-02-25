#pragma once
#include "GameObject.h"
#include "LifecycleObject.h"
#include <list>

class Scene : public LifecycleObject
{
private:
	std::list<GameObject*> gameObjects;
	std::list<GameObject*> initialGameObjects;
	std::list<GameObject*> dynamicObjects;

public:
	Scene():gameObjects(),initialGameObjects(),dynamicObjects() {};
	~Scene();

	void Init();
	void Start();
	void Update(float deltaTime);
	void detectCollisions();
	void Draw();

	void Load() {
		Unload();
		Init();
		Start();
	};

	void Unload();

	void addGameObject(GameObject* go) { initialGameObjects.push_back(go); gameObjects.push_back(go); }
	void addDynamicGameObject(GameObject* go) { dynamicObjects.push_back(go); gameObjects.push_back(go); }

	std::list<GameObject*> allObjects();
	std::list<BaseModel*> getModelList();
};

