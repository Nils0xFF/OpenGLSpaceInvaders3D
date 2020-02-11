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
	GameState state;
	GLboolean keys[1024];
	GLuint Width, Heigth;

	Game(GLuint width, GLuint heigth) :Width(width), Heigth(heigth) {};
	~Game();

	// Initialize game state (load all shaders/textures/levels)
	void Init();

	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

	// Deloading
	void End();
};

