#pragma once
#include "Scene.h"
class SceneManager
{
public:
	Scene* activeScene = NULL;
	static SceneManager& getInstance() {
		static SceneManager instance;
		return instance;
	}

	void LoadScene(Scene* scene) {
		if(activeScene)
			activeScene->Unload();

		activeScene = scene;
		activeScene->Load();
	}

	void Reset(){
		if (activeScene)
			activeScene->Unload();
		activeScene->Load();
	}

private:
	SceneManager() {};
	SceneManager(const SceneManager& im) {};
	SceneManager& operator= (const SceneManager& mg) {};

};
