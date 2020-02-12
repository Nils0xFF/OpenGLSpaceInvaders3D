#include "Game.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "PhongShader.h"
#include "TrianglePlaneModel.h"

Scene testScene;

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

void Game::Init()
{
	GameObject *go = new GameObject();
	MeshRenderer* renderer = new MeshRenderer();
	renderer->model = new Model(ASSET_DIRECTORY "bunny.dae", false);
	renderer->model->shader(new PhongShader(), true);
	go->setRenderer(renderer);
	testScene.addGameObject(go);

	go = new GameObject();
	renderer = new MeshRenderer();
	renderer->model = new TrianglePlaneModel(10.0f, 10.0f, 10, 10);
	renderer->model->shader(new PhongShader(), true);
	go->setRenderer(renderer);
	testScene.addGameObject(go);

	CameraManager::getInstance().activeCamera = &mainCamera;

	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(0, -1, -1));
	dl->color(Color(0.5, 0.5, 0.5));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);
}

void Game::ProcessInput(GLfloat dt)
{
}

void Game::Update(GLfloat dt)
{
	mainCamera.update();
}

void Game::Render()
{
	ShaderLightMapper::instance().activate();
	switch (State)
	{
		case GameState::MENU:
			break;
		case GameState::PAUSED:
			break;
		case GameState::WON:
			break;
		case GameState::LOST:
			break;
		case GameState::PLAYING:
			break;
		default:
			break;
	}

	testScene.Draw();
	ShaderLightMapper::instance().deactivate();

	GLenum Error = glGetError();
	assert(Error == 0);
}

void Game::End()
{
}
