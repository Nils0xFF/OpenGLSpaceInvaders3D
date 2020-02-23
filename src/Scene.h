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
	Scene():gameObjects() {};
	~Scene() {
		while (!gameObjects.empty()) {
			delete gameObjects.front();
			gameObjects.pop_front();
		}
		initialGameObjects.clear();
		dynamicObjects.clear();
	}

	void Init();

	void Start();

	void Update(float deltaTime);

	void detectCollisions();

	void Draw();

	void Load() {
		Init();
		Start();
	};

	void Unload() {
		dynamicObjects.remove(NULL);
		while (!dynamicObjects.empty()) {
			gameObjects.remove(dynamicObjects.front());
			delete dynamicObjects.front();
			dynamicObjects.pop_front();
		}
	};

	void addGameObject(GameObject* go) { initialGameObjects.push_back(go); gameObjects.push_back(go); }
	void addDynamicGameObject(GameObject* go) { dynamicObjects.push_back(go); gameObjects.push_back(go); }

	std::list<GameObject*> allObjects() {
		std::list<GameObject*> objects = gameObjects;
		for (GameObject* g : objects) {
			for (GameObject* c : g->getChildren()) {
				objects.push_back(c);
			}
		}
		objects.unique();
		return objects;
	}

	std::list<BaseModel*> getModelList() {
		std::list<BaseModel*> models;
		for (GameObject* g : allObjects())
		{
			if(g->getRenderer())
				models.push_back(g->getRenderer()->model);
		}
		return models;
	}
};

