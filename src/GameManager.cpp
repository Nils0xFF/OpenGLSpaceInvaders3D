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
#include "TerrainController.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

void GameManager::StartGame() {
	currentGameState = GameState::WAVEMODE;
	score = 0;
	bossAlive = false;
	currentTransitionDuration = 0;
	currentRows = 0;
	rowsSpawnedSinceBoss = 0;

}

void GameManager::EndGame() {
	currentGameState = GameState::MENU;
	SceneManager::getInstance().Reset();
}

void GameManager::PauseGame() {
	if (currentGameState != GameState::PAUSED && currentGameState != GameState::MENU && currentGameState != GameState::LOST) {
		prePauseGameState = currentGameState;
		currentGameState = GameState::PAUSED;
	}
}

void GameManager::Init() {
	createGameScene();
	createMenuScene();
}

void GameManager::Update(float deltaTime) {
	if (currentGameState == GameState::WAVEMODE || currentGameState == GameState::BOSSFIGHT || currentGameState == GameState::TRANSITION) {
		score += deltaTime;
	}

	if (
		(currentGameState == GameState::WAVEMODE && rowsSpawnedSinceBoss >= GameSettings::SPAWNER_WAVES_BEFORE_BOSS) ||
		(currentGameState == GameState::BOSSFIGHT && !bossAlive)) {
		previousGameState = currentGameState;
		currentGameState = GameState::TRANSITION;
	}

	if (currentGameState == GameState::TRANSITION && currentRows == 0 && !bossAlive && currentTransitionDuration < transitionTime) {
		currentTransitionDuration += deltaTime;
	}

	if (currentGameState == GameState::TRANSITION && currentTransitionDuration >= transitionTime) {
		currentTransitionDuration = 0;
		if (previousGameState == GameState::BOSSFIGHT) {
			rowsSpawnedSinceBoss = 0;
			currentGameState = GameState::WAVEMODE;
		}if (previousGameState == GameState::WAVEMODE) {
			bossAlive = true;
			currentGameState = GameState::BOSSFIGHT;
		}
	}

}

void GameManager::createGameScene()
{
	GameObject* skyBox = new GameObject();
	skyBox->setTransform(Matrix().translation(Vector(0.0f, 0.0f, 0.0f)));
	BaseModel *pModel = new Model(ASSET_DIRECTORY "models/skybox.obj", 2 * GameSettings::WORLD_DEPTH);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	skyBox->setRenderer(new MeshRenderer(pModel, new PhongShader(),true));
	gameScene.addGameObject(skyBox);

	GameObject* ground = new GameObject();
	pModel = new TrianglePlaneModel((float) GameSettings::WORLD_WIDTH + 2 * GameSettings::MOUNTAIN_WIDTH, (float) GameSettings::WORLD_DEPTH + 5, 300, 300);
	pModel->shadowCaster(true);
	pModel->shadowReciver(true);
	ground->setRenderer(new MeshRenderer(pModel, new TerrainShader(), true));
	gameScene.addGameObject(ground);
	
	GameObject* playerBullet = new GameObject();
	playerBullet->setName("PlayerBullet");
	playerBullet->setTag(Tag::PlayerBullet);
	pModel = new Model(ASSET_DIRECTORY "models/lazor/lazor.obj", 0.425f);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	ConstantShader* shader = new ConstantShader();
	Color playerBulCol = Color(0.02f, 0.02f, 0.12f);
	shader->color(playerBulCol);
	playerBullet->setRenderer(new MeshRenderer(pModel, shader, true));
	playerBullet->setCollider(new BoxCollider());	
	playerBullet->addComponent(new BulletController());

	ParticleProps* props = new ParticleProps();
	props->colorBegin = playerBulCol;
	props->colorEnd = Color(1, 1, 1);
	props->sizeBegin = 0.07f, props->sizeVariation = 0.04f, props->sizeEnd = 0.0f;
	props->Life = .8f;
	props->Velocity = Vector(0, 0, 2);
	props->VelocityVariation = Vector(10, 4, 0);
	playerBullet->addComponent(new ParticleGenerator(200, props));

	PointLight* pl = new PointLight();
	pl->color(Color(0.125f, 0.125f, 0.75f));
	pl->attenuation(Vector(0.5f, 0.1f, 0.5f));
	pl->castShadows(true);
	playerBullet->addComponent(new LightComponent(pl));

	Prefab* playerBulletPrefab = new Prefab(playerBullet);	

	GameObject* player = new GameObject();
	player->setName("Player");
	player->setTag(Tag::Player);
	player->setTransform(Matrix().translation(0, 0.25f, 0) * Matrix().rotationY((float)M_PI));
	player->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "models/spaceships/fighterjet/fighterjet.obj"), new PhongShader(), true));
	player->setCollider(new BoxCollider());
	player->addComponent(new PlayerController(playerBulletPrefab));
	player->addComponent(new FollowCameraController(*CameraManager::getInstance().activeCamera, Vector(0, 0.75f, 1.5f)));
	gameScene.addGameObject(player);

	GameObject* enemyBullet = new GameObject();
	enemyBullet->setName("EnemyBullet");
	enemyBullet->setTag(Tag::EnemyBullet);
	enemyBullet->addComponent(new BulletController());
	enemyBullet->setTransform(Matrix().rotationY(M_PI));
	pModel = new Model(ASSET_DIRECTORY "models/lazor/lazor.obj", 0.315f);
	pModel->shadowCaster(false);
	pModel->shadowReciver(false);
	shader = new ConstantShader();
	Color enemyBulCol = Color(0.95f, 0.02f, 0.02f);
	shader->color(enemyBulCol);
	enemyBullet->setRenderer(new MeshRenderer(pModel, shader, true));
	enemyBullet->setCollider(new BoxCollider());
	ParticleProps* props2 = new ParticleProps(*props);
	props2->colorBegin = enemyBulCol;
	props2->Life = 0.4f;
	enemyBullet->addComponent(new ParticleGenerator(200, props2));

	pl = new PointLight();
	pl->color(Color(0.95f, 0.02f, 0.02f));
	pl->attenuation(Vector(0.5f, 0.1f, 0.5f));
	pl->castShadows(false);
	enemyBullet->addComponent(new LightComponent(pl));

	Prefab* enemyBulletPrefab = new Prefab(enemyBullet);

	GameObject* enemy = new GameObject();
	BaseModel* enemyModel = new Model(ASSET_DIRECTORY "models/spaceships/frigate01/frigate01.obj");
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
	enemy->setTransform(Matrix().translation(2.5f, 0, 0));
	enemyRow->addChild(enemy);

	enemy = new GameObject(*enemy);
	enemy->setTransform(Matrix().translation(1.5f, 0, 0));
	enemyRow->addChild(enemy);

	enemy = new GameObject(*enemy);
	enemy->setTransform(Matrix().translation(-1.5f, 0, 0));
	enemyRow->addChild(enemy);

	enemy = new GameObject(*enemy);
	enemy->setTransform(Matrix().translation(-2.5f, 0, 0));
	enemyRow->addChild(enemy);

	Prefab* enemyRowPrefab = new Prefab(enemyRow);



	std::vector<Prefab*> meteors;

	GameObject* meteor = new GameObject();
	meteor->setTag(Tag::Meteor);
	meteor->setName("Meteor");
	meteor->setTransform(Matrix().rotationY(0.5f * (float) M_PI));
	meteor->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "models/meteors/rock/rock.obj"), new PhongShader(), true));
	meteor->setCollider(new BoxCollider());
	meteor->addComponent(new MeteorController());

	Prefab* meteorPrefab = new Prefab(meteor);
	meteors.push_back(meteorPrefab);

	meteor = new GameObject();
	meteor->setTag(Tag::Meteor);
	meteor->setName("Meteor");
	meteor->setTransform(Matrix().rotationY(0.5f * (float) M_PI));
	meteor->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "models/meteors/rock_02/rock_02.obj"), new PhongShader(), true));
	meteor->setCollider(new BoxCollider());
	meteor->addComponent(new MeteorController());

	meteorPrefab = new Prefab(meteor);
	meteors.push_back(meteorPrefab);

	GameObject* boss = new GameObject();
	boss->setTag(Tag::Boss);
	boss->setName("Boss");
	boss->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "models/spaceships/cruiser01/cruiser01.obj", 7.5f), new PhongShader(), true));
	boss->setCollider(new BoxCollider());
	boss->addComponent(new BossController(new Prefab(*enemyBulletPrefab)));

	Prefab* bossPrefab = new Prefab(boss);

	GameObject* enemySpawner = new GameObject();
	enemySpawner->setTransform(Matrix().translation(0.0f, 0.25f, (float)-GameSettings::WORLD_DEPTH - 1));
	shader = new ConstantShader();
	shader->color(Color(0, 0, 0));
	enemySpawner->setRenderer(new MeshRenderer(new TriangleBoxModel(0.1f, 0.1f, 0.1f), shader, true));
	enemySpawner->addComponent(new EnemySpawnerController(enemyRowPrefab, bossPrefab));
	enemySpawner->addComponent(new MeteorSpawnerController(meteors));
	gameScene.addGameObject(enemySpawner);

	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(-0.5f, -1.0f, -1.0f));
	dl->color(Color(1, 1, 1));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);

}
