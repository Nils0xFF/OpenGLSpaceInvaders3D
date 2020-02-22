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

private:
	SceneManager() {};
	SceneManager(const SceneManager& im) {};
	SceneManager& operator= (const SceneManager& mg) {};
};
