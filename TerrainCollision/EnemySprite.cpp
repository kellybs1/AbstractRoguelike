#include "EnemySprite.h"

/*
EnemySprite class: Holds and manages information regarding sprite which are engagable enemies.
Developed by Brendan Kelly 19-30 September, 2016.
*/

EnemySprite::EnemySprite(Graphics^ inGraphics, array<Bitmap^>^ startFileNames, Random^ inRand, int inNFrames, int inXPos, int inYPos, Rectangle inClientRect, int inBoundsAction,
	Bitmap^ inSplatImg, int inReductXPerC, int inReductYPerC, TileMap^ inTileMap, int inVelocity, int inEnemyType, int inStartState) : Sprite(inGraphics,
	startFileNames, inRand, inNFrames, inXPos, inYPos, inClientRect, inBoundsAction, inSplatImg, inReductXPerC, inReductYPerC, inTileMap, inVelocity)
{
	enemyType = inEnemyType;
	enemyState = inStartState;
	bulletTimer = 0;
}


void EnemySprite::updateState(Sprite^ player)
{
	int playerX = player->XPos;
	int playerY = player->YPos;
	switch (enemyState)
	{
	case ENEMY_STATE_WANDER:
		switch (enemyType)
		{
		case ENEMY_TYPE_RANDO:
			enemyState = ENEMY_STATE_WANDER;
			break;

		case ENEMY_TYPE_TURRET:
			//should always hunt
			enemyState = ENEMY_STATE_HUNT;
			break;

		case ENEMY_TYPE_CHASER:
			if (playerInChaserRadius(player) == true)
			{
				enemyState = ENEMY_STATE_HUNT;
			}		
			break;					
		}
		break;

	case ENEMY_STATE_HUNT:
		switch (enemyType)
		{
		case ENEMY_TYPE_CHASER:
			if (NextMoveTileWalkable() == false || playerInChaserRadius(player) == false)
			{
				enemyState = ENEMY_STATE_WANDER;
			}
			break;

		case ENEMY_TYPE_TURRET:
			//never changes state, sits still
			break;
		}
	break;
	}
}

void EnemySprite::doAction(Sprite^ player)
{
	int playerX = player->XPos;
	int playerY = player->YPos;
	int distanceX = XPos - player->XPos;
	int distanceY = YPos - player->YPos;
	int posDistanceX = distanceX;
	int posDistanceY = distanceY;
	int pickedAxis = 0;
	//buffer so sprite can actually collide with player
	int moveBuffer = player->FrameWidth / 2;
	if (distanceX < 0)
	{
		posDistanceX = distanceX * -1;
	}
	if (distanceY < 0)
	{
		posDistanceY = distanceY * -1;
	}

	switch (enemyState)
	{
	case ENEMY_STATE_WANDER:
		switch (enemyType)
		{
		case ENEMY_TYPE_RANDO:
			Wander();
			break;
			

		case ENEMY_TYPE_TURRET:
			break;

		case ENEMY_TYPE_CHASER:
			Wander();
			break;
			
		}
		break;

	case ENEMY_STATE_HUNT:
		switch (enemyType)
		{
			//decide which axis to move on based on distance on each axis
		case ENEMY_TYPE_CHASER:
			if (posDistanceX < posDistanceY || posDistanceX < moveBuffer)
			{
				pickedAxis = MOVE_ON_Y;
			}
			else
			if (posDistanceY > posDistanceX || posDistanceY < moveBuffer)
			{
				pickedAxis = MOVE_ON_X;
			}

			switch (pickedAxis)
			{
				//pick next direction based on player being east or west of sprite
			case MOVE_ON_X:
				if (distanceX < 0)
				{
					SpriteDirection = EAST;
				}
				if (distanceX > 0)
				{
					SpriteDirection = WEST;
				}
				break;

			case MOVE_ON_Y:
				if (distanceY < 0)
				{
					SpriteDirection = SOUTH;
				}
				if (distanceY > 0)
				{
					SpriteDirection = NORTH;
				}	
				break;
			}
			break;
			//if move isn't possible the update state should change state to wandering
			

		case ENEMY_TYPE_RANDO:
			//should never get hunt state
			break;

		case ENEMY_TYPE_TURRET:
			bulletTimer++;
			if (bulletTimer >= BULLET_TIMER)
			{
				bulletTimer = 0;
			}
			break;
			
		}
		break;
	}
}

//returns true if player can be seen by chaser
bool EnemySprite::playerInChaserRadius(Sprite^ player) 
{
	bool playerSeen = true;
	
	int playerLeft = player->XPos - (DETECTION_RADIUS_TILES_X * TILE_SIDE);
	int playerRight = player->XPos + (DETECTION_RADIUS_TILES_X * TILE_SIDE);
	int playerTop = player->YPos - (DETECTION_RADIUS_TILES_Y * TILE_SIDE);
	int playerBottom = player->YPos + (DETECTION_RADIUS_TILES_Y * TILE_SIDE);

	if (XPos < playerLeft)
	{
		playerSeen = false;
	}
	if (XPos > playerRight)
	{
		playerSeen = false;
	}
	if (YPos < playerTop)
	{
		playerSeen = false;
	}
	if (YPos > playerBottom)
	{
		playerSeen = false;
	}

	return playerSeen;	
}

void EnemySprite::randomiseBulletTimer()
{
	bulletTimer = rand->Next(BULLET_TIMER);
}