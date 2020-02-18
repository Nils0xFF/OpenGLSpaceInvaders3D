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

	std::list<BaseModel*> getModelList() {
		std::list<BaseModel*> models;
		for (GameObject* g : gameObjects)
		{
			if(g->getRenderer())
				models.push_back(g->getRenderer()->model);
		}
		return models;
	}
};

