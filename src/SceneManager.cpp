#include "SceneManager.h"

void SceneManager::LoadScene(Scene* scene) {
	if (activeScene)
		activeScene->Unload();

	activeScene = scene;
	activeScene->Load();
}

void SceneManager::Reset() {
	if (activeScene)
		activeScene->Unload();
	activeScene->Load();
}
