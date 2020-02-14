#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Camera.h"
#include "PostProcessing.h"

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

	// Called before the first Frame
	// used to communicate between Components
	void Start();

	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

	// Post-Processing
	void WindowResize(int width, int height);

	// Deloading
	void End();

private:
	int SCREEN_WIDTH = 960;
	int SCREEN_HEIGHT = 540;
};

