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

	void spawnRandomMeteor();
	void spawnRandomMeteorBoss();

public:

	MeteorSpawnerController(Prefab* prefab) { prefabs.push_back(prefab); }
	MeteorSpawnerController(const std::vector<Prefab*>& prefabs) { this->prefabs = prefabs;	}
	~MeteorSpawnerController() {
		while (!prefabs.empty())
		{
			delete prefabs.back();
			prefabs.pop_back();
		}
	}

	MeteorSpawnerController* clone() { return new MeteorSpawnerController(*this); }

	void Update(float deltaTime);
};

