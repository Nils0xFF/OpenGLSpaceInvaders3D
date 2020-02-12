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

	void Start() {
		for (GameObject* g : gameObjects) {
			g->Start();
		}
	}

	void Update(float deltaTime) {
		for (GameObject* g : gameObjects) {
			if(g->isActive())
				g->Update(deltaTime);
		}
	}

	void detectCollisions() {
		for (GameObject* g : gameObjects) {
			if (g->isActive() && !g->isStatic() && g->getCollider() != NULL)
				for (GameObject* other : gameObjects) {
					if (other != g && other->getCollider() != NULL) {
						if (g->getCollider()->checkCollision(*(other->getCollider()))) {
							g->onCollision(other);
							other->onCollision(g);
						}
					}
				}
		}
	}

	void Draw() {
		for (GameObject* g : gameObjects) {
			if(g->isActive())
				g->Draw();
		}
	};

	void addGameObject(GameObject* go) { gameObjects.push_back(go); }

};

