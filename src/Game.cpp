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
#include "Text.h"
#include "TriangleBoxModel.h"
#include "GameSettings.h"
#include "Terrain.h"
#include "TerrainShader.h"

#define _USE_MATH_DEFINES
#include <math.h>

Scene testScene;
Text* text;

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
	go->setTransform(go->getTransform() * Matrix().translation(0,1,0) * Matrix().rotationY(0.5f * M_PI));
	go->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "spaceships/spaceship_4.obj", true), new PhongShader(), true));
	go->setName("Player");
	go->addComponent(new PlayerController());
	// go->addComponent(new TestController());
	go->setCollider(new BoxCollider());
	go->addComponent(new FollowCameraController(mainCamera, Vector(0,.65f,1.5f)));
	testScene.addGameObject(go);

	go = new GameObject();
	BaseModel* pModel = new Terrain(GameSettings::WORLD_WITH, 20, 200, 200);
	pModel->shadowCaster(false);
	pModel->shadowReciver(true);
	go->setRenderer(new MeshRenderer(pModel, new TerrainShader(), true));
	testScene.addGameObject(go);

	go = new GameObject();
	pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
	pModel->shadowCaster(false);
	pModel->shadowReciver(true);
	go->setRenderer(new MeshRenderer(pModel, new PhongShader(), true));
	testScene.addGameObject(go);



	CameraManager::getInstance().activeCamera = &mainCamera;

	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(1.0f, -1.0f, -0.50f));
	dl->color(Color(0.5, 0.5, 0.5));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);

	testScene.Init();

	text = new Text();
	//text->setFont("game_over.ttf");
	//text->setFont("i_mWeird.ttf");
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
		
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	testScene.Draw();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	text->Render("Hallo, Welt!\nHier koennte Ihre Werbung stehen.\nTest.", 0.05, 0.3, 0.5, Color(0, 0, 0));

	ShaderLightMapper::instance().deactivate();
	PostProcessing::getInstance().End(glfwGetTime());

	GLenum Error = glGetError();
	assert(Error == 0);
}

void Game::End()
{
	delete text;
}
