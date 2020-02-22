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

Text* text;
Text* text2;
Text* text3;

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

void Game::Init()
{
	CameraManager::getInstance().activeCamera = &mainCamera;
	GameManager::getInstance().Init();
	text = new Text();
	text2 = new Text();
	text3 = new Text();
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
			gameManager.StartGame();
		if (InputManager::getInstance().Keys[GLFW_KEY_ESCAPE])
			glfwWindowShouldClose(pWindow);
		break;
	case GameState::PAUSED:
		if (InputManager::getInstance().Keys[GLFW_KEY_SPACE])
			gameManager.UnPauseGame();
		if (InputManager::getInstance().Keys[GLFW_KEY_ESCAPE])
			gameManager.EndGame();
		break;
	case GameState::LOST:
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
	switch (gameManager.getGameState())
	{
	case GameState::MENU:
	case GameState::BOSSFIGHT:
	case GameState::WAVEMODE:
	case GameState::TRANSITION:
		SceneManager::getInstance().activeScene->Update(dt);
		SceneManager::getInstance().activeScene->detectCollisions();
		CameraManager::getInstance().activeCamera->update();
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

	SceneManager::getInstance().activeScene->Draw();

	switch (gameManager.getGameState())
	{
		case GameState::MENU:
			text->Render("Space Invaders", 0.05f, 0.9f, 1.0f, Color(0, 0, 0));
			text2->Render("Controls:\n<A/D>: Move Left/Right\n<SPACE>: Shoot\n<P> to Pause the Game", 0.05f, 0.8f, 0.5f, Color(0, 0, 0));
			text3->Render("Press <ENTER> to Start\n\n<ESCAPE> to quit", 0.35f, 0.35f, 0.75f, Color(0, 0, 0));
			break;
		case GameState::PAUSED:
			text->Render("Paused!", 0.35f, 0.9f, 1.0f, Color(1, 1, 1));
			text3->Render("Press <SPACE> to Unpause\n\n<ESCAPE> for Menu", 0.35f, 0.35f, 0.75f, Color(0, 0, 0));
			break;
		case GameState::LOST:
			text->Render("Game Over!\nScore:15", 0.35f, 0.9f, 1.0f, Color(1, 1, 1));
			text3->Render("Press <ENTER> to Restart\n\n<ESCAPE> for Menu", 0.35f, 0.35f, 0.75f, Color(0, 0, 0));
			break;
		case GameState::BOSSFIGHT:
		case GameState::TRANSITION:
		case GameState::WAVEMODE:
			text->Render("HP: 5/5", 0.125f, 0.85f, 0.5f, Color(1, 1, 1));
			text2->Render("Score: 100", 0.125f, 0.8f, 0.5f, Color(1, 1, 1));
			break;
		default:
			break;
	}		

	ShaderLightMapper::instance().deactivate();
	PostProcessing::getInstance().End();

	GLenum Error = glGetError();
	if (Error != 0)
		std::cout << Error << std::endl;
	assert(Error == 0);
}

void Game::End()
{
	delete text;
	delete text2;
	delete text3;
}