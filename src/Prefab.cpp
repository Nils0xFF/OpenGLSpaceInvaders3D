#include "Prefab.h"

// Creates a deep copy of the prefabed gameObject and returns it
GameObject* Prefab::instantiate(const Vector& position, const Matrix& rotation, std::string name) {
	assert(prefab);
	GameObject* toInstantiate = new GameObject(*prefab);
	toInstantiate->setTransform(Matrix().translation(position) * rotation);
	toInstantiate->Init();
	toInstantiate->Start();
	SceneManager::getInstance().activeScene->addDynamicGameObject(toInstantiate);
	return toInstantiate;
}
