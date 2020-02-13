#include "Scene.h"

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

void Scene::detectCollisions() {
	for (GameObject* g : gameObjects) {
		if (g->isActive() && !g->isStatic() && g->getCollider() != NULL)
			for (GameObject* other : gameObjects) {
				if (other != g && other->getCollider() != NULL) {
					if (g->getCollider()->checkCollision(other->getCollider())) {
						g->onCollision(other);
						other->onCollision(g);
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
