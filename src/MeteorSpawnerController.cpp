#include "MeteorSpawnerController.h"

// Creates meteors with random placement in the gameworld
void MeteorSpawnerController::spawnRandomMeteor() {
	if (prefabs.size() <= 0) return;
	int spawnIndex = (int)std::round(Random::random() * (prefabs.size() - 1.0f));
	Prefab* prefab = prefabs[spawnIndex];
	float xOffset = prefab->getGameObject()->getAreaBox()->size().X / 2.0f;
	float xCord = Random::random() * (GameSettings::WORLD_WIDTH - xOffset) - (GameSettings::WORLD_WIDTH - xOffset) / 2.0f;
	prefab->instantiate(gameObject->getTransform().translation() + Vector(xCord, 0, 0), prefab->getGameObject()->getTransform());
	timeSinceLastMeteor = 0;
}

// Creates meteors on the edge of the map to stop players from hiding at the edge of the map
void MeteorSpawnerController::spawnRandomMeteorBoss() {
	if (prefabs.size() <= 0) return;
	int spawnIndex = (int)std::round(Random::random() * (prefabs.size() - 1));
	Prefab* prefab = prefabs[spawnIndex];
	float xOffset = prefab->getGameObject()->getAreaBox()->size().X / 2.0f;
	float xCord = Random::random() > 0.5f ? (GameSettings::WORLD_WIDTH / 2.0f - xOffset) : -(GameSettings::WORLD_WIDTH / 2.0f - xOffset);
	prefab->instantiate(gameObject->getTransform().translation() + Vector(xCord, 0, 0), prefab->getGameObject()->getTransform());
	timeSinceLastMeteor = 0;
}

// Randomly creates a meteor shower and updates timing every frame
void MeteorSpawnerController::Update(float deltaTime) {
	if (GameManager::getInstance().getGameState() == GameState::WAVEMODE) {
		timeSinceLastMeteor += deltaTime;
		if (!shower && timeSinceLastMeteor >= timeBetweenMeteors && timeSinceLastShower >= minTimeBetweenShowers) {
			shower = Random::random() <= showerChance;
			if (shower) {
				timeSinceLastShower = 0;
				currentShowerDuration = 0;
				std::cout << "SHOWER!!" << std::endl;
			}
		}
		if (!shower) {
			timeSinceLastShower += deltaTime;
			if (timeSinceLastMeteor >= timeBetweenMeteors) {
				spawnRandomMeteor();
			}
		}
		else {
			currentShowerDuration += deltaTime;
			if (timeSinceLastMeteor >= 1.0f / showerSpawnrate) {
				spawnRandomMeteor();
			}

			if (currentShowerDuration >= showerDuration) {
				shower = false;
				std::cout << "SHOWER End!!" << std::endl;
			}
		}
	}
	else if (GameManager::getInstance().getGameState() == GameState::BOSSFIGHT) {
		timeSinceLastMeteor += deltaTime;
		if (timeSinceLastMeteor >= 1.5f / showerSpawnrate) {
			spawnRandomMeteorBoss();
		}
	}
}
