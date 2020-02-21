#pragma once
#include "Component.h"
#include "Prefab.h"

class EnemySpawnerController :
	public Component
{
private:
	Prefab* enemyRow;
	float timeBetweenRows = 7.5f;
	float timeSinceLastRow = timeBetweenRows;

public:
	EnemySpawnerController(Prefab* row) :enemyRow(row) {};
	EnemySpawnerController(const EnemySpawnerController& other) :enemyRow(new Prefab(*other.enemyRow)) {};
	~EnemySpawnerController() { if (enemyRow) delete enemyRow; }

	EnemySpawnerController* clone() { return new EnemySpawnerController(*this); }

	void Update(float dTime) {
		timeSinceLastRow += dTime;
		if (timeSinceLastRow >= timeBetweenRows) {
			std::cout << "Spawing Row @ " << gameObject->getTransform().translation() << std::endl;
			enemyRow->instantiate(gameObject->getTransform().translation());
			timeSinceLastRow = 0;
		}
	}
};
