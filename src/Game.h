#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Camera.h"
#include "PostProcessing.h"
#include "ShadowMapGenerator.h"

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
	Game(GLFWwindow* pWin) : State(GameState::PLAYING), pWindow(pWin), mainCamera(pWindow), ShadowGenerator(2048, 2048) { glfwGetWindowSize(pWin, &SCREEN_WIDTH, &SCREEN_HEIGHT); };

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
	ShadowMapGenerator ShadowGenerator;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
};

