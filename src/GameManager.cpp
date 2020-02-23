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

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

void GameManager::createGameScene()
{

	GameObject* ground = new GameObject();
	BaseModel* pModel = new TrianglePlaneModel(GameSettings::WORLD_WIDTH + 2 * GameSettings::MOUNTAIN_WIDTH, GameSettings::WORLD_DEPTH, 200, 200);
	pModel->shadowCaster(true);
	pModel->shadowReciver(true);
	ground->setRenderer(new MeshRenderer(pModel, new TerrainShader(), true));
	gameScene.addGameObject(ground);

	SceneManager::getInstance().activeScene = &gameScene;

	GameObject* playerBullet = new GameObject();
	playerBullet->setName("PlayerBullet");
	playerBullet->setTag(Tag::PlayerBullet);
	pModel = new TriangleBoxModel(0.1f, 0.05f, 0.3f);
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
	ParticleGenerator* gen = new ParticleGenerator(100, props);

	GameObject* player = new GameObject();
	player->setName("Player");
	player->setTag(Tag::Player);
	player->setTransform(Matrix().translation(0, 0.25f, 0) * Matrix().rotationY((float)M_PI));
	player->setRenderer(new MeshRenderer(new Model(ASSET_DIRECTORY "models/spaceships/fighterjet/fighterjet.obj"), new PhongShader(), true));
	player->setCollider(new BoxCollider());
	player->addComponent(new PlayerController(playerBulletPrefab));
	player->addComponent(new FollowCameraController(*CameraManager::getInstance().activeCamera, Vector(0, .65f, 1.5f)));
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
	shader->color(Color(0.75f, 0, 0));
	enemyBullet->setRenderer(new MeshRenderer(pModel, shader, true));
	enemyBullet->setCollider(new BoxCollider());

	pl = new PointLight();
	pl->color(Color(0.75f, 0, 0));
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
	enemy->setTransform(Matrix().translation(1.5f, 0, 0));
	enemyRow->addChild(enemy);

	enemy = new GameObject(*enemy);
	enemy->setTransform(Matrix().translation(-1.5f, 0, 0));
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
	// testScene.addGameObject(meteor);

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
	enemySpawner->setTransform(Matrix().translation(0.0f, 0.25f, (float)-GameSettings::WORLD_DEPTH));
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
