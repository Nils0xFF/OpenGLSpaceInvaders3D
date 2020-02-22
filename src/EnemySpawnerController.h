#pragma once
#include "Component.h"
#include "Prefab.h"
#include "GameManager.h"

class EnemySpawnerController :
	public Component
{
private:
	Prefab* enemyRow;
	Prefab* bossPrefab;
	bool spawnedBoss = false;
	float timeBetweenRows = 6.5f;
	float timeSinceLastRow = timeBetweenRows;

public:
	EnemySpawnerController(Prefab* row, Prefab* bossPrefab) :enemyRow(row), bossPrefab(bossPrefab) {};
	EnemySpawnerController(const EnemySpawnerController& other):enemyRow(new Prefab(*other.enemyRow)), bossPrefab(new Prefab(*other.bossPrefab)) {};
	~EnemySpawnerController() { if (enemyRow) delete enemyRow; if (bossPrefab) delete bossPrefab; }

	EnemySpawnerController* clone() { return new EnemySpawnerController(*this); }

	void Update(float dTime) {
		if (GameManager::getInstance().getGameState() == GameState::WAVEMODE) {
			spawnedBoss = false;
			timeSinceLastRow += dTime;
			if (timeSinceLastRow >= timeBetweenRows) {
				std::cout << "Spawing Row @ " << gameObject->getTransform().translation() << std::endl;
				enemyRow->instantiate(gameObject->getTransform().translation());
				timeSinceLastRow = 0;
				GameManager::getInstance().RowSpawned();
			}
		}
		else if (GameManager::getInstance().getGameState() == GameState::BOSSFIGHT && !spawnedBoss) {
			bossPrefab->instantiate(gameObject->getTransform().translation(), bossPrefab->getGameObject()->getTransform());
			spawnedBoss = true;
		}
	}
};

