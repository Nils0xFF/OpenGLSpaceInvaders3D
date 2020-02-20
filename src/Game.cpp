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
#include "PlayerController.h"
#include "EnemyController.h"
#include "EnemyRowController.h"
#include "FollowCameraController.h"
#include "Text.h"
#include "TriangleBoxModel.h"
#include "GameSettings.h"
#include "TerrainShader.h"
#include "ParticleSystem.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

Scene testScene;
Text* text;
ParticleSystem* sys;
ParticleProps props;

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

void Game::Init()
{
	srand(static_cast <unsigned> (time(0)));

	SceneManager::getInstance().activeScene = &testScene;

	GameObject* playerBullet = new GameObject();
	playerBullet->setName("PlayerBullet");
	playerBullet->setTag(Tag::PlayerBullet);
	BaseModel* pModel = new TriangleBoxModel(0.05f,0.05f,0.3f);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	ConstantShader* shader = new ConstantShader();
	shader->color(Color(0, 0, 0.5));
	playerBullet->setRenderer(new MeshRenderer(pModel, shader, true));
	playerBullet->setCollider(new BoxCollider());

	PointLight* pl = new PointLight();
	pl->color(Color(0, 0, 0.5));
	pl->attenuation(Vector(0.5f, 0.1f, 0.05f));
	pl->castShadows(true);
	playerBullet->addComponent(new LightComponent(pl));
	playerBullet->addComponent(new BulletController());

	Prefab* playerBulletPrefab = new Prefab(playerBullet);

	GameObject* player = new GameObject();
	player->setName("Player");
	player->setTag(Tag::Player);
	player->setTransform(Matrix().translation(0,1,0) * Matrix().rotationY(0.5f * M_PI));
	player->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "spaceships/spaceship_4.obj", true), new PhongShader(), true));
	player->setCollider(new BoxCollider());
	player->addComponent(new PlayerController(playerBulletPrefab));
	player->addComponent(new FollowCameraController(mainCamera, Vector(0,.65f,1.5f)));
	testScene.addGameObject(player);

	GameObject* enemyBullet = new GameObject();
	enemyBullet->setName("EnemyBullet");
	enemyBullet->setTag(Tag::EnemyBullet);
	pModel = new TriangleBoxModel(0.05f, 0.05f, 0.3f);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	shader = new ConstantShader();
	shader->color(Color(0.5f, 0,0));
	enemyBullet->setRenderer(new MeshRenderer(pModel, shader, true));
	enemyBullet->setCollider(new BoxCollider());

	pl = new PointLight();
	pl->color(Color(0.5f, 0, 0));
	pl->attenuation(Vector(0.5f, 0.1f, 0.05f));
	pl->castShadows(true);
	enemyBullet->addComponent(new LightComponent(pl));
	enemyBullet->addComponent(new BulletController());

	Prefab* enemyBulletPrefab = new Prefab(enemyBullet);

	GameObject* enemy = new GameObject();
	enemy->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "spaceships/spaceship_1.obj", true), new PhongShader(), true));
	enemy->setName("Enemy");
	enemy->setTag(Tag::Enemy);
	enemy->addComponent(new EnemyController(1, 2.5f, enemyBulletPrefab));
	enemy->setCollider(new BoxCollider());
	// testScene.addGameObject(enemy);

	GameObject* enemyRow = new GameObject();
	enemyRow->setTransform(Matrix().translation(0, 1, -15));
	enemyRow->addComponent(new EnemyRowController());
	enemyRow->addChild(enemy);

	enemy = new GameObject(*enemy);
	enemy->setTransform(Matrix().translation(2, 0, 0));
	enemyRow->addChild(enemy);

	enemy = new GameObject(*enemy);
	enemy->setTransform(Matrix().translation(-2, 0, 0));
	enemyRow->addChild(enemy);

	testScene.addGameObject(enemyRow);


	/* GameObject* ground = new GameObject();
	pModel = new TrianglePlaneModel(GameSettings::WORLD_WITH, 20, 200, 200);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	ground->setRenderer(new MeshRenderer(pModel, new TerrainShader(), true));
	testScene.addGameObject(ground); */

	GameObject* skybox = new GameObject();
	pModel = new Model(ASSET_DIRECTORY "skybox.obj", false);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	skybox->setRenderer(new MeshRenderer(pModel, new PhongShader(), true));
	testScene.addGameObject(skybox);

	CameraManager::getInstance().activeCamera = &mainCamera;

	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(0.1f, -1.5f, -1.0f));
	dl->color(Color(0.5, 0.5, 0.5));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);

	testScene.Init();

	text = new Text();
	//text->setFont("game_over.ttf");
	//text->setFont("i_mWeird.ttf");

	sys = new ParticleSystem(2);
	props.colorBegin = Color(254 / 255.0f, 212 / 255.0f, 123 / 255.0f);
	props.colorEnd = Color(254 / 255.0f, 109 / 255.0f, 41 / 255.0f);
	props.sizeBegin = 0.5f, props.sizeVariation = 0.3f, props.sizeEnd = 0.0f;
	props.Life = 1.0f;
	props.Velocity = Vector(0, 0, 0);
	props.VelocityVariation = Vector(3, 1, 0);
	props.Position = Vector(0, 0, 0);

}

void Game::Start() {
	testScene.Start();
}

void Game::ProcessInput(GLfloat dt)
{	
}

void Game::Update(GLfloat dt)
{
	testScene.Update(dt);
	mainCamera.update();
	testScene.detectCollisions();
	sys->Update(dt);
	sys->Emit(props);
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
	// sys->Draw();
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
	delete sys;
}
