#pragma once
#include "Component.h"
#include "Prefab.h"
#include "Random.h"
#include "GameSettings.h"
#include "GameManager.h"

class MeteorSpawnerController :
	public Component
{
private:
	bool shower = false;
	float showerSpawnrate = 3.0f;
	float showerDuration = 5.0f;
	float currentShowerDuration = 0.0f;
	float showerChance = 0.25f;
	float minTimeBetweenShowers = 10.0f;
	float timeSinceLastShower = 0.0f;
	float timeBetweenMeteors = 5.0f;
	float timeSinceLastMeteor = timeBetweenMeteors;
	std::vector<Prefab*> prefabs;

	void spawnRandomMeteor() {
		if (prefabs.size() <= 0) return;
		int spawnIndex = std::round(Random::random() * (prefabs.size() - 1));
		Prefab* prefab = prefabs[spawnIndex];
		float xOffset = prefab->getGameObject()->getAreaBox()->size().X / 2.0f;
		float xCord = Random::random() * (GameSettings::WORLD_WIDTH - xOffset) - (GameSettings::WORLD_WIDTH - xOffset) / 2.0f;
		prefab->instantiate(gameObject->getTransform().translation() + Vector(xCord,0,0), prefab->getGameObject()->getTransform());
		timeSinceLastMeteor = 0;
	}

	void spawnRandomMeteorBoss() {
		if (prefabs.size() <= 0) return;
		int spawnIndex = std::round(Random::random() * (prefabs.size() - 1));
		Prefab* prefab = prefabs[spawnIndex];
		float xOffset = prefab->getGameObject()->getAreaBox()->size().X / 2.0f;
		float xCord = Random::random() > 0.5f ? (GameSettings::WORLD_WIDTH / 2.0f - xOffset) : -(GameSettings::WORLD_WIDTH / 2.0f - xOffset);
		prefab->instantiate(gameObject->getTransform().translation() + Vector(xCord, 0, 0), prefab->getGameObject()->getTransform());
		timeSinceLastMeteor = 0;
	}

public:

	MeteorSpawnerController(Prefab* prefab) {
		prefabs.push_back(prefab);
	}

	MeteorSpawnerController(const std::vector<Prefab*>& prefabs) {
		this->prefabs = prefabs;
	}

	~MeteorSpawnerController() {
		while (!prefabs.empty())
		{
			delete prefabs.back();
			prefabs.pop_back();
		}
	}

	MeteorSpawnerController* clone() { return new MeteorSpawnerController(*this); }

	void Update(float deltaTime) {
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
};

