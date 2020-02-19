#include "Game.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "PhongShader.h"
#include "TrianglePlaneModel.h"
#include "TestController.h"
#include "BoxCollider.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "LightComponent.h"
#include "ParticleGenerator.h"
#include "PlayerController.h"
#include "FollowCameraController.h"
#include "Text.h"
#include "TriangleBoxModel.h"
#include "GameSettings.h"

#define _USE_MATH_DEFINES
#include <math.h>

Scene testScene;
Text* test;

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

void Game::Init()
{
	SceneManager::getInstance().activeScene = &testScene;
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
	
	GameObject* playerBullet = new GameObject();
	playerBullet->setTransform(Matrix().rotationY(0.5f * M_PI));
	playerBullet->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "spaceships/spaceship_4.obj", true), new PhongShader(), true));
	playerBullet->setCollider(new BoxCollider());

	PointLight* pl = new PointLight();
	pl->color(Color(0, 0, 0.5));
	pl->attenuation(Vector(0.5f, 0.1f, 0.05f));
	pl->castShadows(true);
	playerBullet->addComponent(new LightComponent(pl));
	playerBullet->addComponent(new BulletController());
	Prefab* playerBulletPrefab = new Prefab(playerBullet);

	GameObject* player = new GameObject();
	player->setTransform(Matrix().translation(0,1,0) * Matrix().rotationY(0.5f * M_PI));
	player->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "spaceships/spaceship_4.obj", true), new PhongShader(), true));
	player->setName("Player");
	player->addComponent(new PlayerController(playerBulletPrefab));
	// go->addComponent(new TestController());
	player->setCollider(new BoxCollider());
	player->addComponent(new FollowCameraController(mainCamera, Vector(0,.65f,1.5f)));
	testScene.addGameObject(player);

	GameObject* ground = new GameObject();
	BaseModel* pModel = new TrianglePlaneModel(GameSettings::WORLD_WITH, 20, 1000, 100);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	ground->setRenderer(new MeshRenderer(pModel, new PhongShader(), true));
	testScene.addGameObject(ground);

	GameObject* skyBox = new GameObject();
	skyBox->setName("skyBox");
	pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	skyBox->setRenderer(new MeshRenderer(pModel, new PhongShader(), true));
	testScene.addGameObject(skyBox);



	CameraManager::getInstance().activeCamera = &mainCamera;

	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(1.0f, -1.0f, -0.50f));
	dl->color(Color(0.5, 0.5, 0.5));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);

	testScene.Init();

	test = new Text(ASSET_DIRECTORY "fonts/F25_Bank_Printer.ttf");
}

void Game::Start() {
	testScene.Start();
}

void Game::ProcessInput(GLfloat dt)
{	
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

	test->Render("Hallo, Welt!", 0.05, 0.05, 2, Color(0, 0, 0));

	ShaderLightMapper::instance().deactivate();
	PostProcessing::getInstance().End(glfwGetTime());

	GLenum Error = glGetError();
	assert(Error == 0);
}

void Game::End()
{
	delete test;
}
