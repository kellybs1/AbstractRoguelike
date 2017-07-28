#pragma once

#include "TileMap.h"
/*
Sprite class: Holds and manages information regarding a single sprite.
Developed by Brendan Kelly throughout August and September, 2016.
*/


//effective constants
#define VELOCITY 5
#define PROBABILITY_MAX 25
#define MAX_DIRECTIONS 9 
#define STILLFRAME 1
#define NCORNERS 4

//directions
#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7
#define STILL 8

//bounds states
#define BOUNCE 0
#define WRAP 1
#define DIE 2
#define STOP 3

//collision type
#define COLLISION_NONE 0
#define COLLISION_X 1
#define COLLISION_Y 2

#define SPRITE_HEALTH_MAX 100
#define SPRITE_MANA_MAX 100

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class Sprite
{
protected:
	Graphics^ canvas;					//for sprite to draw itself on
	array<Bitmap^>^ spriteSheets;		//source of sprites
	array<Point>^ velocityDirections;	//array for managing direction points
	Random^ rand;						//random number generator
	Bitmap^ splatImg;					//image to use when sprite "dies' but you want to leave remains instead of deleting the Sprite entirely
	int nFrames;						//number of frames in spritesheet animation		
	int xVel;							//x size of step
	int yVel;							//y size of step
	int currentFrame;					//number of frame currently being displayed 
	int previousSpriteDirection;		//for tracking the direction sprite was in before direction change   
	Rectangle clientRect;				//the dimensions of the visible part of the form   
	Rectangle boundsRect;				//rectangle of the boundaries for a single sprite   
	int hitType;						//int for type of collision detected   
	int actualDirections;				//number of directions not including Still - to be calculated in constructor
	int reductXPerC;					//the percentage on x axis to reduce the collision rectangle 
	int reductYPerC;					//the percentage on y axis to reduce the collision rectangle 
	int testXPos;						//for testing the position of the next move
	int testYPos;						//for testing the position of the next move
	int currentSpriteLocation;			//the x location in the spritesheet of the curent sprite
	Bitmap^ currentSpriteSheet;			//the directional spritesheet to be currently using
	Rectangle drawRect;					//the rectangle in the spritesheet to draw as the actual sprite image
	TileMap^ workingTileMap;			//the tilemap currently being worked with
	void boundsCollisionCheck();
	void boundsCollisionActions();
	void setMoveSprite();
	void loadDeathImages();

public:
	Sprite(Graphics^ inGraphics, array<Bitmap^>^ startFileNames, Random^ inRand, int inNFrames, int inXPos, int inYPos, Rectangle inClientRect, int inBoundsAction, 
		Bitmap^ inSplatImg, int inReductXPerC, int inReductYPerC, TileMap^ inTileMap, int inVelocity);
	Sprite^ Next;
	void Draw();
	void ForceDraw(int inX, int inY);
	void DrawSetup();
	void Move();
	void LimitedMove();
	void UpdateFrame();
	void SetSpriteSheet(Bitmap^ newSpriteSheet, int newNFrames);
	void Wander();
	void RandomisePosition();
	void RandomiseDirection();
	void RandomiseCurrentFrame();
	void SpawnInsideBoundsRectangle();
	void SpawnOnRandomWalkableTile();
	void SpawnOnRandomTileTypeOf(int wantedTileType);
	void AllMoveActions();
	void SpriteDeath();
	bool CollideWith(Sprite^ otherSprite);
	void CalcCollisionRectangle();
	void SetStillFrameIfStill();
	void SetBoundsRectToTileMapEdges();
	void KillOnWall();
	Point NextMoveLocation();
	bool NextMoveTileWalkable();
	bool NextMoveInsideDrawableWorld();

	property int SpriteDirection;
	property int BoundsAction;
	property int XPos;				
	property int YPos;				
	property bool IsAlive;	
	property Rectangle CollisionRect;
	property int FrameWidth;					//pixel height of a single frame in the spritesheet
	property int FrameHeight;					//pixel width of a single frame in the spritesheet
	property int Health;
	property int Mana;
	
};

