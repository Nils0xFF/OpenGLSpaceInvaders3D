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

	void Update(float dTime);
};

