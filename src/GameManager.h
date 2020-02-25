#pragma once
#include "GameObject.h"
#include "SceneManager.h"
#include "GameSettings.h"
#include "PhongShader.h"
#include "ConstantShader.h"

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

	void StartGame();
	void ReStartGame() { SceneManager::getInstance().Reset(); StartGame(); }
	void EndGame();
	void PauseGame();
	void UnPauseGame() { currentGameState = prePauseGameState; };

	void Init();
	void Start() { SceneManager::getInstance().LoadScene(&gameScene); }
	void Update(float deltaTime);;

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

	PhongShader phongShader;
	ConstantShader enemyBulletShader;
	ConstantShader playerBullterShader;

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

