#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Camera.h"
#include "PostProcessing.h"
#include "ShadowMapGenerator.h"
#include "GameManager.h"
#include "TextManager.h"

class Game
{
public:
	Game(GLFWwindow* pWin) : pWindow(pWin), mainCamera(pWindow), ShadowGenerator(2048, 2048), gameManager(GameManager::getInstance()), textManager(TextManager::getInstance()) {};

	GLFWwindow* pWindow;
	Camera mainCamera;

	~Game() {};

	// Initialize game state (load all shaders/textures/levels)
	void Init();

	// Called before the first Frame
	// used to communicate between Components
	void Start();

	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

	// Deloading
	void End();

private:
	ShadowMapGenerator ShadowGenerator;
	GameManager& gameManager;
	TextManager& textManager;
};

