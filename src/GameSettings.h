#pragma once

class GameSettings
{
public:
	// Player Parameters
	static const float PLAYER_SPEED;
	static const unsigned int PLAYER_HP;
	static const float PLAYER_FIRERATE;
	static const float PLAYER_BULLET_SPEED;
	static const int PLAYER_BULLET_DAMAGE;

	// Base Enemy Parameters
	static const unsigned int ENEMY_HP;
	static const float ENEMEY_X_SPEED;
	static const float ENEMEY_Z_SPEED;
	static const float ENEMEY_BULLET_SPEED;
	static const float ENEMEY_BULLET_DAMAGE;
	static const float ENEMEY_FIRERATE;
	static const float ENEMEY_FIRERATE_VARIATION;

	// Boss Enemy Parameters
	static const unsigned int BOSS_HP;
	static const float BOSS_X_SPEED;
	static const float BOSS_Z_SPEED;
	static const float BOSS_BULLET_SPEED;
	static const float BOSS_BULLET_DAMAGE;
	static const float BOSS_FIRERATE;
	static const float BOSS_FIRERATE_VARIATION;

	// Enemy Spwaner Parameters
	static const int SPAWNER_WAVES_BEFORE_BOSS;
	static const float SPWANER_TIME_BETWEEN_WAVES;

	// World Parameters
	static const int WORLD_WITH;
	static const int WORLD_DEPTH;
};
