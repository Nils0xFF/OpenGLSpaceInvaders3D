#include "Scene.h"
#include <algorithm>
#include "CollisionHelper.h"

using namespace std;

Scene::~Scene() {
	while (!gameObjects.empty()) {
		delete gameObjects.front();
		gameObjects.pop_front();
	}
	initialGameObjects.clear();
	dynamicObjects.clear();
}

void Scene::Init() {
	for (GameObject* g : initialGameObjects) {
		g->Init();
	}
}

void Scene::Start() {
	for (GameObject* g : initialGameObjects) {
		g->Start();
	}
}

void Scene::Update(float deltaTime) {
	for (list<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end();) {
		if ((*it)->isDeleted()) {
			delete (*it);
			dynamicObjects.remove(*it);
			it = gameObjects.erase(it);
			continue;
		}
		if ((*it)->isActive())
			(*it)->Update(deltaTime);
		it++;
	}
	gameObjects.remove(NULL);
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

		reportedPairs.unique();

		for (pair<GameObject*, GameObject*> possibleCollision : reportedPairs) {
			if (CollisionHelper::detectAABBCollision(possibleCollision.first->getAreaBox(), possibleCollision.second->getAreaBox())) {
				// std::cout << "AABB Collision" << possibleCollision.first->getName() << " and " << possibleCollision.second->getName() << std::endl;
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

void Scene::Unload(){
while (!dynamicObjects.empty()) {
		gameObjects.remove(dynamicObjects.front());
		delete dynamicObjects.front();
		dynamicObjects.pop_front();
	}
}

std::list<GameObject*> Scene::allObjects() {
	std::list<GameObject*> objects = gameObjects;
	for (GameObject* g : objects) {
		for (GameObject* c : g->getChildren()) {
			objects.push_back(c);
		}
	}
	objects.unique();
	return objects;
}

std::list<BaseModel*> Scene::getModelList() {
	std::list<BaseModel*> models;
	for (GameObject* g : allObjects())
	{
		if (g->getRenderer())
			models.push_back(g->getRenderer()->model);
	}
	return models;
}
