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
	};

	void Init() {
		for (GameObject* g : gameObjects) {
			g->Init();
		}
	};

	void Update(float deltaTime) {
		for (GameObject* g : gameObjects) {
			g->Update(deltaTime);
		}
	}

	void Draw() {
		for (GameObject* g : gameObjects) {
			g->Draw();
		}
	};

	void addGameObject(GameObject* go) { gameObjects.push_back(go); }

};

