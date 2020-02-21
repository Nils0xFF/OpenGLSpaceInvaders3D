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
	for (list<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end();) {
		if ((*it)->isDeleted()) {
			delete (*it);
			it = gameObjects.erase(it);
			continue;
		}
		if ((*it)->isActive())
			(*it)->Update(deltaTime);
		it++;
	}
}

bool compareZ(GameObject* a, GameObject* b) { return a->getAreaBox()->Min.Z > b->getAreaBox()->Min.Z; }

// using one axis sweep and prune for now
// most of our movement happens around the z axis, therefor we use that
void Scene::detectCollisions() {
	std::list<GameObject*> collisionObjects = allObjects();
	if (collisionObjects.size() > 1) {

		collisionObjects.sort(compareZ);
		list<GameObject*> activeList;
		list<pair<GameObject*, GameObject*>> reportedPairs;

		for (GameObject* g : collisionObjects) {
			if (g->getCollider() != NULL) {
				activeList.push_back(g);
				for (list<GameObject*>::iterator it = activeList.begin(); it != activeList.end();) {
					if (g != *it) {
						if (g->getAreaBox()->Min.Z > (*it)->getAreaBox()->Max.Z) {
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

		// reportedPairs.unique();

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
