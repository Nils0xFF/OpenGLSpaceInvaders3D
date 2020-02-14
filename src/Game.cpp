#include "Game.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "PhongShader.h"
#include "TrianglePlaneModel.h"
#include "TestController.h"
#include "BoxCollider.h"

Scene testScene;

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

void Game::Init()
{
	GameObject *go = new GameObject();
	go->setName("Bunny1");
	go->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "buddha.dae", false), new PhongShader(), true));
	go->setCollider(new BoxCollider());
	go->addComponent(new TestController());
	testScene.addGameObject(go);

	go = new GameObject(*go);
	go->setName("Bunny2");
	go->setTransform(Matrix().translation(0, 3, 0));
	testScene.addGameObject(go);

	go = new GameObject();
	go->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "buddha.dae", false), new PhongShader(), true));
	go->setName("BunnyMain");
	go->setCollider(new BoxCollider());
	testScene.addGameObject(go);

	go = new GameObject();
	go->setRenderer(new MeshRenderer(new TrianglePlaneModel(10.0f, 10.0f, 10, 10), new PhongShader(), true));
	testScene.addGameObject(go);

	go = new GameObject(*go);
	go->setTransform(Matrix().translation(0, 3, 0));
	testScene.addGameObject(go);


	CameraManager::getInstance().activeCamera = &mainCamera;

	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(0, -1, -1));
	dl->color(Color(0.5, 0.5, 0.5));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);

	testScene.Init();
}

void Game::Start() {
	testScene.Start();
}

void Game::ProcessInput(GLfloat dt)
{	
}

void Game::WindowResize(int width, int height)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	
	PostProcessing::getInstance().setResolution(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Game::Update(GLfloat dt)
{
	mainCamera.update();
	testScene.Update(dt);
	testScene.detectCollisions();
}

void Game::Render()
{
	PostProcessing::getInstance().Begin();
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
	PostProcessing::getInstance().End(glfwGetTime());

	GLenum Error = glGetError();
	assert(Error == 0);
}

void Game::End()
{
}
