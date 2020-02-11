#pragma once
#include <GL\glew.h>

enum GameState {
	MENU,
	PLAYING,
	PAUSED,
	WON,
	LOST
};

class Game
{
public:
	Game(GLuint width, GLuint height) :State(PLAYING), Keys(), Width(width), Height(height) {};
	GameState State;
	GLboolean Keys[1024];
	GLuint Width, Height;

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

