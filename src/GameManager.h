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
		currentGameState = GameState::WAVEMODE;
		score = 0;
		bossAlive = false;
		currentTransitionDuration = 0;
		currentRows = 0;
		rowsSpawnedSinceBoss = 0;

	}
	void ReStartGame() { SceneManager::getInstance().Reset(); StartGame(); }
	void EndGame() { 
		currentGameState = GameState::MENU;
		SceneManager::getInstance().Reset();
	};

	void PauseGame() { 
		if (currentGameState != GameState::PAUSED && currentGameState != GameState::MENU && currentGameState != GameState::LOST) {
			prePauseGameState = currentGameState;
			currentGameState = GameState::PAUSED;
		}
	};

	void UnPauseGame() { currentGameState = prePauseGameState; };

	void Init() {
		createGameScene();
		createMenuScene();
	};

	void Start() { SceneManager::getInstance().LoadScene(&gameScene); }

	void Update(float deltaTime) {
		if (currentGameState == GameState::WAVEMODE || currentGameState == GameState::BOSSFIGHT || currentGameState == GameState::TRANSITION) {
			score += deltaTime;
		}

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
	void BossDied() { bossAlive = false; score += 50; }
	void RowSpawned() { rowsSpawnedSinceBoss++; currentRows++; }
	void RowDestroyed() { currentRows--; }

	void updatePlayerHP(int hp) { playerHP = hp; }
	int getPlayerHP() { return playerHP; }

	void updateBossHP(int hp) { bossHP = hp; }
	int getBossHP() { return bossHP; }

	int getScore() { return (int) std::round(score); }

	float deltaTime = 0;

private:
	Scene menuScene;
	Scene gameScene;
	GameManager() {};
	GameManager(const GameManager& im) {};
	GameManager& operator= (const GameManager& mg) {};

	GameState currentGameState = GameState::MENU;
	GameState prePauseGameState = GameState::MENU;
	GameState previousGameState = GameState::MENU;

	float transitionTime = 3.0f;
	float currentTransitionDuration = 0.0f;

	int playerHP = 0;
	int bossHP = 0;
	int currentRows = 0;
	int rowsSpawnedSinceBoss = 0;

	float score = 0;

	bool bossAlive = false;

	void createGameScene();
	void createMenuScene() {};
};

