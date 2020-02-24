#include "EnemySpawnerController.h"

// Handles enemy-rows. Instantiates a row based in time.
void EnemySpawnerController::Update(float dTime) {
	if (GameManager::getInstance().getGameState() == GameState::WAVEMODE) {
		spawnedBoss = false;
		timeSinceLastRow += dTime;
		if (timeSinceLastRow >= timeBetweenRows) {
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
