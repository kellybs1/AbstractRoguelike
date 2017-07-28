#pragma once

/*
GameManager class: Manages creation of all game objects and interaction between objects.
Developed by Brendan Kelly 19-30 September, 2016.
*/

#include "SpriteList.h"
#include "TileMap.h"
#include "Dungeon.h"
#include "ItemSprite.h"
#include "EnemySprite.h"

#define VISION_HORIZONTAL_RADIUS 6
#define VISION_VERTICAL_RADIUS 4

#define PLAYER_BULLET_MANA_COST 5

#define PLAYER_BULLET_DAMAGE 40
#define ENEMY_BULLET_DAMAGE 10

#define PLAYER_HURT_MIN 1
#define PLAYER_HURT_MAX 5
#define ENEMY_HURT_MIN 10
#define ENEMY_HURT_MAX 30
#define DAMAGE_BATTLE_TIMER 10
#define DAMAGE_INCREASE_PER_LEVEL 4

#define KILL_SCORE 47

#define COLLISION_RADIUS_TILES_X	 2
#define COLLISION_RADIUS_TILES_Y 2

//starting config values
#define SPRITE_FRAMES 8
#define START_NHEALTH_ITEMS 5
#define START_NMANA_ITEMS 5
#define START_NLOOT_ITEMS 6
#define START_NENEMY_TURRETS 5
#define START_NENEMY_RANDOMS 5
#define START_NENEMY_CHASERS 5
#define START_PLAYERVEL 7
#define START_ENEMYVEL 5
#define START_BULLETVEL 10
#define START_ITEMVEL 0
#define START_LEVEL 0

ref class GameManager
{
private:
	TileList^ mainTileList;
	TileMap^ mainTileMap;
	String^ mapFileName;
	Graphics^ canvas;
	Random^ rand;
	Bitmap^ blankImg;
	Bitmap^ splatImg;
	Bitmap^ playerSplatImg;
	Bitmap^ fogImg;
	SpriteList^ enemies;
	SpriteList^ bullets;
	SpriteList^ items;
	SpriteList^ enemyBullets;
	SpriteList^ levelWarps;
	array<Bitmap^>^ playerFiles;
	array<Bitmap^>^ enemyFiles;
	array<Bitmap^>^ chaserFiles;
	array<Bitmap^>^ randoFiles;
	array<Bitmap^>^ bulletFiles;	
	array<Bitmap^>^ enemyBulletFiles;
	array<Bitmap^>^ healthItemFiles;
	array<Bitmap^>^ manaItemFiles;
	array<Bitmap^>^ lootItemFiles;
	array<Bitmap^>^ levelWarpFiles;
	Sprite^ player;
	Sprite^ levelWarp;
	Viewport^ mainVPort;
	Rectangle clientRect;
	Dungeon^ mainDungeon;
	array<int, 2>^ generatedDungeon;
	array<bool, 2>^ fogMap;
	int nFrames;
	int itemVel;
	int nHealthItems;
	int nManaItems;
	int nLootItems;
	int nTotalItems;
	int nEnemyTurrets;
	int nEnemyRandoms;
	int nEnemyChasers;
	int nTotalEnemies;
	int playerNewX;
	int playerNewY;
	int playerVel;
	int enemyVel;
	int bulletVel;
	int centeringBullet;
	int battleReadyTimer;
	int damageIncrease;
	
	void fillTileList();
	void loadMiscImages();
	void loadPlayerImages();
	void makePlayer();          
	void loadEnemyImages();
	void makeEnemies();
	void loadBulletImages();
	void playerActions();
	void calculatePlayerNewXYPos();
	void enemyActions();
	void damageEnemies();
	void playerEnemyCollision();
	void playerBulletCollision();
	void playerDirectionManager();
	void bulletActions(SpriteList^ currentBulletList);
	void loadHealthItemImages();
	void loadManaItemImages();
	void loadLootItemImages();
	void makeItems();
	void itemActions();
	void collectItems();
	void initFogMap();
	void unFog();
	void spriteHealthCheck(Sprite^ spriteToCheck);
	void makeEnemyBullet(Sprite^ currentTurret);
	bool playerSpawnBreakingViewport();
	void updateDisplayValues();
	void loadLevelWarpImages();
	void makeLevelWarp();
	void levelWarpActions();
	void playerWarpCollision();
	bool playerNear(Sprite^ testSprite);


public:
	GameManager(Graphics^ inCanvas, Random^ inRand, Rectangle inClientRect);
	void MakeBullet(int startDirection);
	void BulletManaManage(int startDirection);
	void GameCycle();
	void NextDungeon();
	property bool LeftDown;
	property bool RightDown;
	property bool UpDown;
	property bool DownDown;
	property int PlayerScore;
	property int PlayerHealth;
	property int PlayerMana;
	property int Level;

};

