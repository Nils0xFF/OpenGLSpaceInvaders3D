#include "Game.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "PhongShader.h"
#include "TrianglePlaneModel.h"
#include "TestController.h"
#include "BoxCollider.h"
#include "InputManager.h"
#include "LightComponent.h"
#include "ParticleGenerator.h"
#include "PlayerController.h"
#include "FollowCameraController.h"

#define _USE_MATH_DEFINES
#include <math.h>

Scene testScene;

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

void Game::Init()
{
	/*GameObject *go = new GameObject();
	go->setName("Bunny1");
	go->setTransform(Matrix().translation(0, 1, 0));
	go->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "dragon.dae", false), new PhongShader(), true));
	go->setCollider(new BoxCollider());
	go->addComponent(new TestController());
	testScene.addGameObject(go);

	go = new GameObject(*go);
	go->setName("Bunny2");
	go->setTransform(Matrix().translation(0, 3, 0));
	PointLight* light = new PointLight(Vector(0,0,0), Color(0,1,2));
	light->attenuation(Vector(0,0,1));
	go->addComponent(new LightComponent(light));
	go->getComponentByType<TestController>()->speed = -1.0f;
	testScene.addGameObject(go);*/
	

	GameObject* go = new GameObject();
	go->setTransform(go->getTransform() * Matrix().rotationY(-0.5f * M_PI));
	go->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "dragon.dae", false), new PhongShader(), true));
	go->setName("Player");
	go->addComponent(new PlayerController());
	go->addComponent(new TestController());
	go->setCollider(new BoxCollider());
	go->addComponent(new FollowCameraController(mainCamera, Vector(0,1.0f,2.0f)));
	testScene.addGameObject(go);

	go = new GameObject();
	go->setRenderer(new MeshRenderer(new TrianglePlaneModel(10.0f, 10.0f, 10, 10), new PhongShader(), true));
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
	ShadowGenerator.generate(testScene.getModelList());

	PostProcessing::getInstance().shader->on(false);
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
