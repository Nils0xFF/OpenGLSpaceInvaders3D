#pragma once
#include "GameObject.h"
#include "LifecycleObject.h"
#include <list>

class Scene : public LifecycleObject
{
private:
	std::list<GameObject*> gameObjects;

public:
	Scene():gameObjects() {};
	~Scene() {
		while (!gameObjects.empty()) {
			delete gameObjects.front();
			gameObjects.pop_front();
		}
	}

	void Init();

	void Start();

	void Update(float deltaTime);

	void detectCollisions();

	void Draw();

	void addGameObject(GameObject* go) { gameObjects.push_back(go); }

	std::list<GameObject*> allObjects() {
		std::list<GameObject*> objects = gameObjects;
		for (GameObject* g : objects) {
			for (GameObject* c : g->getChildren()) {
				objects.push_back(c);
			}
		}
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

