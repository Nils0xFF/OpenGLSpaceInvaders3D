#include "Game.h"

void Game::Init()
{
}

void Game::ProcessInput(GLfloat dt)
{
}

void Game::Update(GLfloat dt)
{
}

void Game::Render()
{
	switch (State)
	{
		case MENU:
			break;
		case PAUSED:
			break;
		case WON:
			break;
		case LOST:
			break;
		case PLAYING:
			break;
		default:
			break;
	}
}

void Game::End()
{
}
