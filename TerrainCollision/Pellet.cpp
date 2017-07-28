#include "Pellet.h"

//
//Pellet class holds information about individual pellets
//

Pellet::Pellet(int inXPos, int inYPos, Graphics^ inCanvas, Random^ inRand, TileMap^ inTileMap)
{
	workingTileMap = inTileMap;
	xPos = inXPos;
	yPos = inYPos;
	canvas = inCanvas;
	rand = inRand;
	xVelocity = 0;
	//yVelocity = rand->Next(PELLET_VELOCITY_MIN, PELLET_VELOCITY_MAX);
	yVelocity = 0;
	IsAlive = true;
	Next = nullptr;
	//picking colours --- min and max hopefully avoid full white or full black pellets
	//colourR = rand->Next(MIN_COLOR, MAX_COLOR);
	//colourB = rand->Next(MIN_COLOR, MAX_COLOR);
	//colourG = rand->Next(MIN_COLOR, MAX_COLOR);
	colour = Color::FromArgb(PLAYER_BULLET_R, PLAYER_BULLET_G, PLAYER_BULLET_B);
	brush = gcnew SolidBrush(colour);
}

//draws pellet to canvas
void Pellet::draw()
{
	canvas->FillEllipse(brush, xPos, yPos, PELLET_SIZE, PELLET_SIZE);
}

//moves pellet then kills it if it's off screen
void Pellet::move()
{
	yPos += yVelocity;
	xPos += xVelocity;
	if (yPos < 0 - PELLET_SIZE || xPos < 0 - PELLET_SPEED)
	{
		IsAlive = false;
	}
	if (yPos >= TILE_SIDE * ROWCOUNT || xPos >= TILE_SIDE * COLCOUNT)
	{
		IsAlive = false;
	}
}

void Pellet::hitWall()
{
	int xTile = xPos / TILE_SIDE;	
	int yTile = yPos / TILE_SIDE; 

	bool tileClear = workingTileMap->isTileWalkable(yTile, xTile);
	if (tileClear == false)
	{
		IsAlive = false;
	}
}

void Pellet::orient(Point target)
{
	double angleToTarget = computeAngleToTarget(target);
	xVelocity = Math::Cos(angleToTarget) * PELLET_SPEED;
	yVelocity = Math::Sin(angleToTarget) * PELLET_SPEED;
}


double Pellet::computeAngleToTarget(Point target)
{
	double angle;

	int xDelta = target.X - xPos;
	int yDelta = target.Y - yPos;

	if ((xDelta != 0) && (yDelta != 0))
	{
		// atan2 returns the angle of rotation between two points with horizontal distance arg1 and vertical distance arg2
		angle = Math::Atan2(yDelta, xDelta);	// result is in radians
	}
	else
		angle = 0.0;	// instance and target share an x or y axis location. This works as a heuristic

	return angle;
}
