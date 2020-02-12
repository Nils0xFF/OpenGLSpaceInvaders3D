#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Camera.h"

enum class GameState {
	MENU,
	PLAYING,
	PAUSED,
	WON,
	LOST
};

class Game
{
public:
	Game(GLFWwindow* pWin) : State(GameState::PLAYING), pWindow(pWin), mainCamera(pWindow) {};

	GLFWwindow* pWindow;
	GameState State;
	Camera mainCamera;

	~Game() {};

	// Initialize game state (load all shaders/textures/levels)
	void Init();

	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

	// Deloading
	void End();
};

