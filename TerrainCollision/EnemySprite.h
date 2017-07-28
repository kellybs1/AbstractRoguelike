#pragma once
#include "Sprite.h"

/*
EnemySprite class: Holds and manages information regarding sprite which are engagable enemies.
Developed by Brendan Kelly 19-30 September, 2016.
*/

#define BULLET_TIMER 50

#define ENEMY_TYPE_TURRET 0
#define ENEMY_TYPE_CHASER 1
#define ENEMY_TYPE_RANDO 2

#define ENEMY_STATE_WANDER 0
#define ENEMY_STATE_HUNT 1

#define DETECTION_RADIUS_TILES_X	 6
#define DETECTION_RADIUS_TILES_Y 3


#define MOVE_ON_X 0
#define MOVE_ON_Y 1

ref class EnemySprite :
public Sprite
{

private:
	int enemyType;
	int enemyState;
	int bulletTimer;

	bool playerInChaserRadius(Sprite^ player);

	

public:
	EnemySprite(Graphics^ inGraphics, array<Bitmap^>^ startFileNames, Random^ inRand, int inNFrames, int inXPos, int inYPos, Rectangle inClientRect, int inBoundsAction,
		Bitmap^ inSplatImg, int inReductXPerC, int inReductYPerC, TileMap^ inTileMap, int inVelocity, int inEnemyType, int inStartState);

	void updateState(Sprite^ player);
	void doAction(Sprite^ player);
	void randomiseBulletTimer();


	int getEnemyType() { return enemyType; }
	void setEnemyType(int inEnemyType) { enemyType = inEnemyType; }

	int getEnemyState() { return enemyState; }
	void setEnemyState(int inEnemyState) { enemyState = inEnemyState; }

	int getBulletTimer() { return bulletTimer; }
	void setBulletTimer(int inBulletTimer) { bulletTimer = inBulletTimer; }
};

