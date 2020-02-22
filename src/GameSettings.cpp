#include "GameSettings.h"

// Player Parameters
const float GameSettings::PLAYER_SPEED = 5.0f;
const unsigned int GameSettings::PLAYER_HP = 5;
const float GameSettings::PLAYER_FIRERATE = 1.0f;
const float GameSettings::PLAYER_BULLET_SPEED = 1.0f;
const int GameSettings::PLAYER_BULLET_DAMAGE = 1;

// Base Enemy Parameters
const unsigned int GameSettings::ENEMY_HP = 1;
const float GameSettings::ENEMEY_X_SPEED = 0.5f;
const float GameSettings::ENEMEY_Z_SPEED = 1.0f;
const float GameSettings::ENEMEY_BULLET_SPEED = 1.0f;
const float GameSettings::ENEMEY_BULLET_DAMAGE = 1.0f;
const float GameSettings::ENEMEY_FIRERATE = 1.0f;
const float GameSettings::ENEMEY_FIRERATE_VARIATION = 1.0f;

// Boss Enemy Parameters
const unsigned int GameSettings::BOSS_HP = 10;
const float GameSettings::BOSS_X_SPEED = 1.25f;
const float GameSettings::BOSS_Z_SPEED = 1.0f;
const float GameSettings::BOSS_BULLET_SPEED = 2.0f;
const float GameSettings::BOSS_BULLET_DAMAGE = 2.0f;
const float GameSettings::BOSS_FIRERATE = 1.5f;
const float GameSettings::BOSS_FIRERATE_VARIATION = 0.1f;

// Enemy Spwaner Parameters
const int GameSettings::SPAWNER_WAVES_BEFORE_BOSS = 10.0f;
const float GameSettings::SPWANER_TIME_BETWEEN_WAVES = 5.0f;

// Meteor Parameters
const unsigned int GameSettings::METEOR_HP = 5;
const float GameSettings::METEOR_Z_SPEED = 2.5f;
const unsigned int GameSettings::METEOR_DAMAGE = 1;

// World Parameters
const int GameSettings::WORLD_WITH = 10;
const int GameSettings::WORLD_DEPTH = 20;
