#include "Game.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "PhongShader.h"
#include "TrianglePlaneModel.h"
#include "BoxCollider.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "LightComponent.h"
#include "PlayerController.h"
#include "EnemyController.h"
#include "EnemyRowController.h"
#include "FollowCameraController.h"
#include "EnemySpawnerController.h"
#include "MeteorController.h"
#include "BossController.h"
#include "MeteorSpawnerController.h"
#include "Text.h"
#include "TriangleBoxModel.h"
#include "GameSettings.h"
#include "TerrainShader.h"
#include "ParticleGenerator.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

void Game::Init()
{
	CameraManager::getInstance().activeCamera = &mainCamera;
	GameManager::getInstance().Init();
}

void Game::Start() {
	GameManager::getInstance().Start();
}

void Game::ProcessInput(GLfloat dt)
{
	switch (gameManager.getGameState())
	{
	case GameState::MENU:
		if (InputManager::getInstance().Keys[GLFW_KEY_ENTER])
			gameManager.ReStartGame();
		if (InputManager::getInstance().Keys[GLFW_KEY_ESCAPE]) {
				SceneManager::getInstance().activeScene->Unload();
				glfwSetWindowShouldClose(pWindow, true);
		}
		break;
	case GameState::PAUSED:
		if (InputManager::getInstance().Keys[GLFW_KEY_ENTER])
			gameManager.UnPauseGame();
		if (InputManager::getInstance().Keys[GLFW_KEY_Q])
			gameManager.EndGame();
		break;
	case GameState::LOST:
		if (InputManager::getInstance().Keys[GLFW_KEY_ENTER])
			gameManager.ReStartGame();
		if (InputManager::getInstance().Keys[GLFW_KEY_Q])
			gameManager.EndGame();
		break;
	case GameState::BOSSFIGHT:
	case GameState::WAVEMODE:
	case GameState::TRANSITION:
		if (InputManager::getInstance().Keys[GLFW_KEY_P])
			gameManager.PauseGame();
		break;
	default:
		break;
	}
}

void Game::Update(GLfloat dt)
{
	GameManager::getInstance().Update(dt);
	CameraManager::getInstance().activeCamera->update();
	switch (gameManager.getGameState())
	{
	case GameState::MENU:
		SceneManager::getInstance().activeScene->Update(dt);
		break;
	case GameState::BOSSFIGHT:
	case GameState::WAVEMODE:
	case GameState::TRANSITION:
		SceneManager::getInstance().activeScene->Update(dt);
		SceneManager::getInstance().activeScene->detectCollisions();
		break;
	case GameState::PAUSED:
		break;
	case GameState::LOST:
		break;
	default:
		break;
	}
}

void Game::Render()
{
	ShadowGenerator.generate(SceneManager::getInstance().activeScene->getModelList());

	PostProcessing::getInstance().Begin();
	ShaderLightMapper::instance().activate();
	TextManager::getInstance().Begin();
	SceneManager::getInstance().activeScene->Draw();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	switch (gameManager.getGameState())
	{
		case GameState::MENU:
			textManager.renderText("game_over.ttf", "Space Phoenix", 0.2695f, 0.85f, 2.0f, Color(1, 1, 1));
			textManager.renderText("game_over.ttf", "Controls", 0.05f, 0.75f, 0.95f, Color(1, 1, 1));
			textManager.renderText("game_over.ttf", "[A/D]: Move Left/Right", 0.05f, 0.65f, 0.75f, Color(1, 1, 1));
			textManager.renderText("game_over.ttf", "[SPACE]: Shoot", 0.05f, 0.60f, 0.75f, Color(1, 1, 1));
			textManager.renderText("game_over.ttf", "[P]: Pause", 0.05f, 0.55f, 0.75f, Color(1, 1, 1));
			textManager.renderText("game_over.ttf", "Insert Coin [ENTER] to Start", 0.3f, 0.25f, 1.0f, Color(1, 1, 1));
			textManager.renderText("game_over.ttf", "Press [ESCAPE] to quit", 0.335f, 0.15f, 1.0f, Color(1, 1, 1));
			break;
		case GameState::PAUSED:
			textManager.renderText("game_over.ttf", "PAUSE", 0.385f, 0.85f, 2.0f, Color(.75f, .75f, .75f));
			textManager.renderText("game_over.ttf", "Press [ENTER] to Unpause", 0.285f, 0.25f, 1.0f, Color(1.0f, 1.0f, 1.0f));
			textManager.renderText("game_over.ttf", "Press [Q] for Menu", 0.345f, 0.15f, 1.0f, Color(1.0f, 1.0f, 1.0f));
			break;
		case GameState::LOST:
			textManager.renderText("game_over.ttf", "GAME OVER", 0.29f, 0.85f, 2.0f, Color(1, 1, 1));
			textManager.renderText("game_over.ttf", ("Score: " + std::to_string(GameManager::getInstance().getScore())).c_str(), 0.385f, 0.45f, 1.5f, Color(1, 1, 1));
			textManager.renderText("game_over.ttf", "Press [ENTER] to Restart", 0.285f, 0.25f, 1.0f, Color(1, 1, 1));
			textManager.renderText("game_over.ttf", "Press [Q] for Menu", 0.345f, 0.15f, 1.0f, Color(1.0f, 1.0f, 1.0f));
			break;
		case GameState::BOSSFIGHT:
			textManager.renderText("game_over.ttf", 
				("Boss: " + std::to_string(GameManager::getInstance().getBossHP()) + "/" + std::to_string(GameSettings::BOSS_HP)).c_str(), 0.35f, 0.85f, 1.75f, Color(1, 1, 1));
		case GameState::TRANSITION:
		case GameState::WAVEMODE:
			textManager.renderText("game_over.ttf", ("HP: " + std::to_string(
				GameManager::getInstance().getPlayerHP()) + "/" +
				std::to_string(GameSettings::PLAYER_HP)).c_str(), 0.125f, 0.85f, 0.75f, Color(0.8f, 0.355f, 0.295f));
			textManager.renderText("game_over.ttf", ("Score: " + std::to_string(GameManager::getInstance().getScore())).c_str(), 0.125f, 0.8f, 0.75f, Color(0.8f, 0.3f, 0.5f));
			break;
		default:
			break;
	}
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	TextManager::getInstance().End();
	ShaderLightMapper::instance().deactivate();
	PostProcessing::getInstance().End();

	GLenum Error = glGetError();
	if (Error != 0)
		std::cout << Error << std::endl;
	// assert(Error == 0);
}

void Game::End()
{
}