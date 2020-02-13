#include "Scene.h"
#include <algorithm>
#include "CollisionHelper.h"

using namespace std;

void Scene::Init() {
	for (GameObject* g : gameObjects) {
		g->Init();
	}
}

void Scene::Start() {
	for (GameObject* g : gameObjects) {
		g->Start();
	}
}

void Scene::Update(float deltaTime) {
	for (GameObject* g : gameObjects) {
		if (g->isActive())
			g->Update(deltaTime);
	}
}

bool compareX(GameObject* a, GameObject* b) { return a->getAreaBox()->Min.X < b->getAreaBox()->Min.X; }

// using one axis sweep and prune for now
void Scene::detectCollisions() {
	if (gameObjects.size() > 1) {

		gameObjects.sort(compareX);
		list<GameObject*> activeList;
		list<pair<GameObject*, GameObject*>> reportedPairs;

		for (GameObject* g : gameObjects) {
			if (g->getCollider() != NULL) {
				activeList.push_back(g);
				for (list<GameObject*>::iterator it = activeList.begin(); it != activeList.end();) {
					if (g != *it) {
						if (g->getAreaBox()->Min.X > (*it)->getAreaBox()->Max.X) {
							it = activeList.erase(it);
							break;
						}
						else {
							if(!g->isStatic() || !(*it)->isStatic())
							reportedPairs.push_back(make_pair(g, *it));
						}
					}
					it++;
				}
			}
		}

		reportedPairs.unique();

		for (pair<GameObject*, GameObject*> possibleCollision : reportedPairs) {
			if (CollisionHelper::detectAABBCollision(possibleCollision.first->getAreaBox(), possibleCollision.second->getAreaBox())) {
				if (possibleCollision.first->getCollider()->checkCollision(possibleCollision.second->getCollider())) {
					possibleCollision.first->onCollision(possibleCollision.second);
					possibleCollision.second->onCollision(possibleCollision.first);
				}
			}
		}
	}
}

void Scene::Draw() {
	for (GameObject* g : gameObjects) {
		if (g->isActive())
			g->Draw();
	}
}
