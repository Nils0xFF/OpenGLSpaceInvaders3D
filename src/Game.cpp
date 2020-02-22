#include "Game.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "PhongShader.h"
#include "TrianglePlaneModel.h"
#include "BoxCollider.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "LightComponent.h"
#include "PlayerController.h"
#include "EnemyController.h"
#include "EnemyRowController.h"
#include "FollowCameraController.h"
#include "EnemySpawnerController.h"
#include "MeteorController.h"
#include "BossController.h"
#include "MeteorSpawnerController.h"
#include "Text.h"
#include "TriangleBoxModel.h"
#include "GameSettings.h"
#include "TerrainShader.h"
#include "ParticleGenerator.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

Scene gameScene;
Text* text;
ParticleGenerator* gen;

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

void Game::Init()
{
	GameObject* skybox = new GameObject();
	BaseModel* pModel = new Model(ASSET_DIRECTORY "models/skybox.obj", false);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	skybox->setRenderer(new MeshRenderer(pModel, new PhongShader(), true));
	gameScene.addGameObject(skybox);

	SceneManager::getInstance().activeScene = &gameScene;

	GameObject* playerBullet = new GameObject();
	playerBullet->setName("PlayerBullet");
	playerBullet->setTag(Tag::PlayerBullet);
	pModel = new TriangleBoxModel(0.1f,0.05f,0.3f);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	ConstantShader* shader = new ConstantShader();
	shader->color(Color(0, 0, 0.75f));
	playerBullet->setRenderer(new MeshRenderer(pModel, shader, true));
	playerBullet->setCollider(new BoxCollider());

	PointLight* pl = new PointLight();
	pl->color(Color(0, 0, 0.75f));
	pl->attenuation(Vector(0.5f, 0.1f, 0.5f));
	pl->castShadows(true);
	playerBullet->addComponent(new LightComponent(pl));
	playerBullet->addComponent(new BulletController());

	Prefab* playerBulletPrefab = new Prefab(playerBullet);

	ParticleProps props;
	props.colorBegin = Color(1, 1, 1);
	props.colorEnd = Color(0, 0, 0);
	props.sizeBegin = 0.5f, props.sizeVariation = 0.3f, props.sizeEnd = 0.0f;
	props.Life = 1.0f;
	props.Velocity = Vector(0, 0, 2);
	props.VelocityVariation = Vector(1, 1, 1);
	props.Position = Vector(0, 0, 0);
	gen = new ParticleGenerator(100, props);

	GameObject* player = new GameObject();
	player->setName("Player");
	player->setTag(Tag::Player);
	player->setTransform(Matrix().translation(0,1,0) * Matrix().rotationY(0.5f * (float) M_PI));
	player->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "models/spaceships/spaceship_4_new.obj"), new PhongShader(), true));
	player->setCollider(new BoxCollider());
	player->addComponent(new PlayerController(playerBulletPrefab));	
	player->addComponent(new FollowCameraController(mainCamera, Vector(0,.65f,1.5f)));
	player->addComponent(gen);
	gameScene.addGameObject(player);

	GameObject* enemyBullet = new GameObject();
	enemyBullet->setName("EnemyBullet");
	enemyBullet->setTag(Tag::EnemyBullet);
	enemyBullet->addComponent(new BulletController());

	pModel = new TriangleBoxModel(0.05f, 0.05f, 0.3f);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	shader = new ConstantShader();
	shader->color(Color(0.75f, 0,0));
	enemyBullet->setRenderer(new MeshRenderer(pModel, shader, true));
	enemyBullet->setCollider(new BoxCollider());

	pl = new PointLight();
	pl->color(Color(0.75f, 0, 0));
	pl->attenuation(Vector(0.5f, 0.1f, 0.5f));
	pl->castShadows(false);
	enemyBullet->addComponent(new LightComponent(pl));

	Prefab* enemyBulletPrefab = new Prefab(enemyBullet);

	GameObject* enemy = new GameObject();
	BaseModel* enemyModel = new Model(ASSET_DIRECTORY "models/spaceships/spaceship_1_new.obj");
	enemyModel->shadowCaster(false);
	enemy->setRenderer(new MeshRenderer(enemyModel, new PhongShader(), true));
	enemy->setName("Enemy");
	enemy->setTag(Tag::Enemy);
	enemy->addComponent(new EnemyController(1, 2.5f, enemyBulletPrefab));
	enemy->setCollider(new BoxCollider());

	GameObject* enemyRow = new GameObject();
	enemyRow->addComponent(new EnemyRowController());
	enemyRow->addChild(enemy);

	enemy = new GameObject(*enemy);
	enemy->setTransform(Matrix().translation(1.5f, 0, 0));
	enemyRow->addChild(enemy);

	enemy = new GameObject(*enemy);
	enemy->setTransform(Matrix().translation(-1.5f, 0, 0));
	enemyRow->addChild(enemy);

	Prefab* enemyRowPrefab = new Prefab(enemyRow);

	/* GameObject* ground = new GameObject();
	pModel = new TrianglePlaneModel(GameSettings::WORLD_WITH, GameSettings::WORLD_DEPTH, 200, 200);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	ground->setRenderer(new MeshRenderer(pModel, new TerrainShader(), true));
	testScene.addGameObject(ground); */
	
	std::vector<Prefab*> meteors;

	GameObject* meteor = new GameObject();
	meteor->setTag(Tag::Meteor);
	meteor->setName("Meteor");
	meteor->setTransform(Matrix().rotationY(0.5f * M_PI));
	meteor->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "models/meteors/rock/rock.obj"),new PhongShader(), true));
	meteor->setCollider(new BoxCollider());
	meteor->addComponent(new MeteorController());
	// testScene.addGameObject(meteor);

	Prefab* meteorPrefab = new Prefab(meteor);
	meteors.push_back(meteorPrefab);

	meteor = new GameObject();
	meteor->setTag(Tag::Meteor);
	meteor->setName("Meteor");
	meteor->setTransform(Matrix().rotationY(0.5f * M_PI));
	meteor->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "models/meteors/rock_02/rock_02.obj"), new PhongShader(), true));
	meteor->setCollider(new BoxCollider());
	meteor->addComponent(new MeteorController());
	
	meteorPrefab = new Prefab(meteor);
	meteors.push_back(meteorPrefab);

	GameObject* enemySpawner = new GameObject();
	// enemySpawner->setActive(false);
	enemySpawner->setTransform(Matrix().translation(0.0f, 1.0f, (float) -GameSettings::WORLD_DEPTH));
	shader = new ConstantShader();
	shader->color(Color(0, 0, 0));
	enemySpawner->setRenderer(new MeshRenderer(new TriangleBoxModel(0.1f, 0.1f, 0.1f), shader, true));
	enemySpawner->addComponent(new EnemySpawnerController(enemyRowPrefab));
	enemySpawner->addComponent(new MeteorSpawnerController(meteors));
	gameScene.addGameObject(enemySpawner);

	GameObject* boss = new GameObject();
	boss->setActive(false);
	boss->setTransform(Matrix().translation(0.0f, 1.0f, -GameSettings::WORLD_DEPTH) * Matrix().rotationY(-0.5f * M_PI));
	boss->setTag(Tag::Enemy);
	boss->setName("Boss");
	boss->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "models/spaceships/spaceship_6_new.obj", 10.0f), new PhongShader(), true));
	boss->setCollider(new BoxCollider());
	boss->addComponent(new BossController(new Prefab(*enemyBulletPrefab)));
	gameScene.addGameObject(boss);

	CameraManager::getInstance().activeCamera = &mainCamera;

	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(-0.5f, -1.0f, -1.0f));
	dl->color(Color(1, 1, 1));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);

	gameScene.Init();

	text = new Text();
}

void Game::Start() {
	gameScene.Start();
}

void Game::ProcessInput(GLfloat dt)
{
	if (InputManager::getInstance().Keys[GLFW_KEY_F])
		gen->setEmiting(!gen->isEmiting());
}

void Game::Update(GLfloat dt)
{
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
		gameScene.Update(dt);
		mainCamera.update();
		gameScene.detectCollisions();
		break;
	default:
		break;
	}
}

void Game::Render()
{
	ShadowGenerator.generate(gameScene.getModelList());

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
			gameScene.Draw();
			text->Render("Hallo, Welt!\nHier koennte Ihre Werbung stehen.\nTest.", 0.05f, 0.3f, 0.5f, Color(1, 1, 1));
			break;
		default:
			break;
	}
		
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	ShaderLightMapper::instance().deactivate();
	PostProcessing::getInstance().End((float) glfwGetTime());

	GLenum Error = glGetError();
	assert(Error == 0);
}

void Game::End()
{
	delete text;
}
