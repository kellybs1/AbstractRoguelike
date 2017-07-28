#pragma once

#include "TileMap.h"

//
//Pellet class holds information about individual pellets
//

#define PELLET_SIZE 10
#define PELLET_VELOCITY_MIN 5
#define PELLET_VELOCITY_MAX 20
#define PELLET_SPEED 10
#define PLAYER_BULLET_R 251
#define PLAYER_BULLET_G 51
#define PLAYER_BULLET_B 0
#define MIN_COLOR 5
#define MAX_COLOR 250

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class Pellet
{
private:
	int xPos;
	int yPos;
	int xVelocity;
	int yVelocity;
	Color colour;
	Graphics^ canvas;
	Brush^ brush;
	Random^ rand;
	TileMap^ workingTileMap;
	int colourR;
	int colourG;
	int colourB;

public:
	Pellet^ Next;
	property bool IsAlive;

public:
	Pellet(int inXPos, int inYPos, Graphics^ inCanvas, Random^ inRand, TileMap^ inTileMap);
	void draw();
	void move();
	void hitWall();
	void orient(Point target);
	double computeAngleToTarget(Point target);

	int getYPos() { return yPos; }
};

