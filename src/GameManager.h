#pragma once
#include "GameObject.h"
#include "SceneManager.h"
#include "GameSettings.h"

enum class GameState {
	MENU,
	WAVEMODE,
	BOSSFIGHT,
	TRANSITION,
	PAUSED,
	LOST
};

class GameManager
{
public:
	static GameManager& getInstance() {
		static GameManager instance;
		return instance;
	}

	GameState getGameState() { return currentGameState; }

	void StartGame() { 
		SceneManager::getInstance().LoadScene(&gameScene); 
		currentGameState = GameState::WAVEMODE;
	}
	void ReStartGame() { StartGame(); };
	void EndGame() { 
		SceneManager::getInstance().LoadScene(&menuScene); 
		currentGameState = GameState::MENU;
	};

	void PauseGame() { 
		prePauseGameState = currentGameState;
		currentGameState = GameState::PAUSED; 
	};

	void UnPauseGame() { currentGameState = prePauseGameState; };

	void Init() {
		createGameScene();
		createMenuScene();
	};

	void Start() { SceneManager::getInstance().LoadScene(&menuScene); }

	void Update(float deltaTime) {
		if (
			(currentGameState == GameState::WAVEMODE && rowsSpawnedSinceBoss >= GameSettings::SPAWNER_WAVES_BEFORE_BOSS)||
			(currentGameState == GameState::BOSSFIGHT && !bossAlive)) {
			previousGameState = currentGameState;
			currentGameState = GameState::TRANSITION;
		}

		if (currentGameState == GameState::TRANSITION && currentRows == 0 && !bossAlive && currentTransitionDuration < transitionTime) {
			currentTransitionDuration += deltaTime;
		}

		if (currentGameState == GameState::TRANSITION && currentTransitionDuration >= transitionTime) {
			currentTransitionDuration = 0;
			if (previousGameState == GameState::BOSSFIGHT) {
				rowsSpawnedSinceBoss = 0;
				currentGameState = GameState::WAVEMODE;
			}if (previousGameState == GameState::WAVEMODE) {
				bossAlive = true;
				currentGameState = GameState::BOSSFIGHT;
			}
		}
	
	};

	void PlayerDied() { currentGameState = GameState::LOST; }
	void BossDied() { bossAlive = false; }
	void RowSpawned() { rowsSpawnedSinceBoss++; currentRows++; }
	void RowDestroyed() { currentRows--; }

	void updatePlayerHP(int hp) { playerHP = hp; }
	int getPlayerHP() { return playerHP; }

	void updateBossHP(int hp) { bossHP = hp; }
	int getBossHP() { return bossHP; }

private:
	Scene menuScene;
	Scene gameScene;
	GameManager() {};
	GameManager(const GameManager& im) {};
	GameManager& operator= (const GameManager& mg) {};

	GameState currentGameState = GameState::MENU;
	GameState prePauseGameState = GameState::MENU;
	GameState previousGameState = GameState::MENU;

	float transitionTime = 5.0f;
	float currentTransitionDuration = 0.0f;

	int playerHP = 0;
	int bossHP = 0;
	int currentRows = 0;
	int rowsSpawnedSinceBoss = 0;

	bool bossAlive = false;

	void createGameScene();
	void createMenuScene() {};
};

