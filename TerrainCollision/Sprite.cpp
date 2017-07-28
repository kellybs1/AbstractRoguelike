#include "Sprite.h"

/*
Sprite class: Holds and manages information regarding a single sprite.
Developed by Brendan Kelly throughout August and September, 2016.
*/

Sprite::Sprite(Graphics^ inGraphics, array<Bitmap^>^ startFileNames, Random^ inRand, int inNFrames, int inXPos, int inYPos, Rectangle inClientRect, int inBoundsAction, Bitmap^ inSplatImg, int inReductXPerC, int inReductYPerC, TileMap^ inTileMap, int inVelocity)
{
	canvas = inGraphics;
	//give sprites their spritesheets from filenames
	spriteSheets = gcnew array<Bitmap^>(MAX_DIRECTIONS);
	for (int i = 0; i < MAX_DIRECTIONS; i++)
	{
		spriteSheets[i] = startFileNames[i];
		spriteSheets[i]->MakeTransparent(spriteSheets[i]->GetPixel(0, 0));
	}

	//set up direction points with respective vectors
	velocityDirections = gcnew array<Point>(MAX_DIRECTIONS);

	velocityDirections[0] = Point(0, -1); //North
	velocityDirections[1] = Point(1, -1); //North East
	velocityDirections[2] = Point(1, 0); //East
	velocityDirections[3] = Point(1, 1); //South East
	velocityDirections[4] = Point(0, 1); //South
	velocityDirections[5] = Point(-1, 1); //South West
	velocityDirections[6] = Point(-1, 0); //West
	velocityDirections[7] = Point(-1, -1); //North West
	velocityDirections[8] = Point(0, 0); //Still

	splatImg = inSplatImg;
	splatImg->MakeTransparent(splatImg->GetPixel(0, 0));
	rand = inRand;
	nFrames = inNFrames;
	clientRect = inClientRect;
	boundsRect = clientRect;
	BoundsAction = inBoundsAction;
	currentFrame = 0;
	IsAlive = true;
	Health = SPRITE_HEALTH_MAX;
	Mana = SPRITE_MANA_MAX;
	FrameWidth = (int)spriteSheets[0]->Width / nFrames;
	FrameHeight = spriteSheets[0]->Height;
	XPos = inXPos;
	YPos = inYPos;
	testXPos = XPos;
	testYPos = YPos;
	xVel = inVelocity;
	yVel = inVelocity;
	SpriteDirection = STILL;
	previousSpriteDirection = STILL;
	hitType = COLLISION_NONE;
	actualDirections = MAX_DIRECTIONS - 1; //minus the still direction to allow for modular mathematics
	reductXPerC = inReductXPerC;
	reductYPerC = inReductYPerC;
	//init collision rectangle
	CalcCollisionRectangle();
	workingTileMap = inTileMap;
	currentSpriteLocation = currentFrame * FrameWidth;
	currentSpriteSheet = spriteSheets[SpriteDirection];
	drawRect = Rectangle(currentSpriteLocation, 0, FrameWidth, FrameHeight);
}


//draws the part of the spritesheet that holds the current frame
void Sprite::Draw()
{
	DrawSetup();
	canvas->DrawImage(currentSpriteSheet, XPos, YPos, drawRect, GraphicsUnit::Pixel);
}

//draws at the given x y location
void Sprite::ForceDraw(int inXPos, int inYPos)
{
	DrawSetup();
	canvas->DrawImage(currentSpriteSheet, inXPos, inYPos, drawRect, GraphicsUnit::Pixel);
}

//wrapper for setting up drawing values
void Sprite::DrawSetup()
{
	currentSpriteLocation = currentFrame * FrameWidth;
	currentSpriteSheet = spriteSheets[SpriteDirection];
	drawRect = Rectangle(currentSpriteLocation, 0, FrameWidth, FrameHeight);
}

//sets the sprite's still frame to a constant frame, over rides other settings
void Sprite::SetStillFrameIfStill()
{
	if (SpriteDirection == STILL)
	{
		currentFrame = STILLFRAME;
	}
}

//wrapper for running all move actions at once
void Sprite::AllMoveActions()
{
	setMoveSprite();
	CalcCollisionRectangle();
	UpdateFrame();
	//SetStillFrameIfStill(); //uncomment this line if you wish to not animate sprites while they're still
	//bounds collision check not used in this project but not useless
	//boundsCollisionCheck();   
	//if (hitType != COLLISION_NONE)
	//{
	//	boundsCollisionActions();
	//}
}

//sets position of sprite on a random position on a walkable tile
void Sprite::SpawnOnRandomWalkableTile()
{
	int maxRandBoundX = COLCOUNT * TILE_SIDE - FrameWidth; 
	int maxRandBoundY = ROWCOUNT * TILE_SIDE - FrameHeight;
	int testXPosSpawn = 0;
	int testYPosSpawn = 0;
	int xTile = 0;							
	int yTile = 0;
	bool wallSpawn = true;
	while (wallSpawn == true)
	{
		testXPosSpawn = rand->Next(1, maxRandBoundX);
		testYPosSpawn = rand->Next(1, maxRandBoundY);
		array<Point>^ corners = gcnew array<Point>(NCORNERS);
		corners[0] = Point(testXPosSpawn, testYPosSpawn);
		corners[1] = Point(testXPosSpawn + FrameWidth, testYPosSpawn);
		corners[2] = Point(testXPosSpawn, testYPosSpawn + FrameHeight);
		corners[3] = Point(testXPosSpawn + FrameWidth, testYPosSpawn + FrameHeight);
		bool singleCornerCollision = false;
		for (int i = 0; i < corners->Length; i++)
		{
			xTile = corners[i].X / TILE_SIDE;
			yTile = corners[i].Y / TILE_SIDE;
			if (workingTileMap->isTileWalkable(yTile, xTile) == false)
			{
				singleCornerCollision = true;
			}		
		}
		wallSpawn = singleCornerCollision;
	}
	XPos = testXPosSpawn;
	YPos = testYPosSpawn;
}

//spawns on a random tile of given type ----  this code is really very repetitive 
void Sprite::SpawnOnRandomTileTypeOf(int wantedTileType)
{
	int maxRandBoundX = COLCOUNT * TILE_SIDE - FrameWidth;
	int maxRandBoundY = ROWCOUNT * TILE_SIDE - FrameHeight;
	int testXPosSpawn = 0;
	int testYPosSpawn = 0;
	int xTile = 0;
	int yTile = 0;
	bool cornersOffTile = true;
	while (cornersOffTile == true)
	{
		//make sure each corner is in the right type of tile
		testXPosSpawn = rand->Next(1, maxRandBoundX);
		testYPosSpawn = rand->Next(1, maxRandBoundY);
		array<Point>^ corners = gcnew array<Point>(NCORNERS);
		corners[0] = Point(testXPosSpawn, testYPosSpawn);
		corners[1] = Point(testXPosSpawn + FrameWidth, testYPosSpawn);
		corners[2] = Point(testXPosSpawn, testYPosSpawn + FrameHeight);
		corners[3] = Point(testXPosSpawn + FrameWidth, testYPosSpawn + FrameHeight);
		bool singleCornerOffTile = false;
		for (int i = 0; i < corners->Length; i++)
		{
			xTile = corners[i].X / TILE_SIDE;
			yTile = corners[i].Y / TILE_SIDE;
			if (workingTileMap->GetMapEntry(yTile, xTile) != wantedTileType)
			{
				singleCornerOffTile = true;
			}
		}
		cornersOffTile = singleCornerOffTile;
	}
	XPos = testXPosSpawn;
	YPos = testYPosSpawn;
}

//sets the bounds rectangle to one pixel in from all edges (stops exception at very edge of screen)
void Sprite::SetBoundsRectToTileMapEdges()
{
	int farBoundX = TILE_SIDE * COLCOUNT - 2;
	int farBoundY = TILE_SIDE * ROWCOUNT - 2;
	boundsRect = Rectangle(1, 1, farBoundX, farBoundY);
}

//replacing living sprite images with dead ones
void Sprite::loadDeathImages()
{
	for (int i = 0; i < spriteSheets->Length; i++)
	{
		spriteSheets[i] = splatImg;
	}
}

//sets sprite dead if it touches wall
void Sprite::KillOnWall()
{
	if (NextMoveTileWalkable() == false)
	{
		IsAlive = false;
	}
	if (NextMoveInsideDrawableWorld() == false)
	{
		IsAlive = false;
	}
}


//moves sprite on X and Y by corresponding velocity values
void Sprite::Move()
{
	XPos = testXPos;
	YPos = testYPos;
}

//moves sprite only if certain checks pass
void Sprite::LimitedMove()
{
	if (IsAlive)
	{
		if (NextMoveTileWalkable())
		{
			Move();
		}

	}
}

//reports if the tile to be moved into is travellable
bool Sprite::NextMoveTileWalkable()
{
	bool legal = true;						
	
	int xPixel = NextMoveLocation().X;
	int yPixel = NextMoveLocation().Y;

	int xTile = xPixel / TILE_SIDE;	 //the column number of the current location of the sprite on a tilemap
	int yTile = yPixel / TILE_SIDE; //the row number of the current location of the sprite on a tilemap

	legal = workingTileMap->isTileWalkable(yTile, xTile);
	return legal;
}

//makes sure we're not forcing outside the drawable canvas
bool Sprite::NextMoveInsideDrawableWorld()
{
	bool inside = true;
	int xPixel = NextMoveLocation().X;
	int yPixel = NextMoveLocation().Y;
	int xBound = boundsRect.Right; 
	int yBound = boundsRect.Bottom;
	if (xPixel < 1 || xPixel > xBound)
	{
		inside = false;
	}
	if (yPixel < 1 || yPixel > yBound)
	{
		inside = false;
	}
	return inside;
}

//finds the xy coordinates of the move in the current direction
Point Sprite::NextMoveLocation()
{
	testXPos = XPos + (xVel * velocityDirections[SpriteDirection].X);
	testYPos = YPos + (yVel * velocityDirections[SpriteDirection].Y);

	int xPixel = 0;							//the x position of the pixel on the frame we want to check
	int yPixel = 0;							//the y position of the pixel on the frame we want to check
	switch (SpriteDirection)
	{
	case NORTH:
		xPixel = testXPos + (FrameWidth / 2);
		yPixel = testYPos;
		break;

	case NORTHEAST:
		xPixel = testXPos + FrameWidth;
		yPixel = testYPos;
		break;

	case EAST:
		xPixel = testXPos + FrameWidth;
		yPixel = testYPos + (FrameHeight / 2);
		break;

	case SOUTHEAST:
		xPixel = testXPos + FrameWidth;
		yPixel = testYPos + FrameHeight;
		break;

	case SOUTH:
		xPixel = testXPos + (FrameWidth / 2);
		yPixel = testYPos + FrameHeight;
		break;

	case SOUTHWEST:
		xPixel = testXPos;
		yPixel = testYPos + FrameHeight;
		break;

	case WEST:
		xPixel = testXPos;
		yPixel = testYPos + (FrameHeight / 2);
		break;

	case NORTHWEST:
		xPixel = testXPos;
		yPixel = testYPos;
		break;

	case STILL:
		xPixel = XPos;
		yPixel = YPos;
		break;
	}

	return Point(xPixel, yPixel);
}

//manages the changing of previous sprite
void Sprite::setMoveSprite()
{
	//set previous sprite direction on direction change
	previousSpriteDirection = SpriteDirection;
	//load the correct direction sprite for standing still if direction has changed
	spriteSheets[STILL] = spriteSheets[previousSpriteDirection];
}

//detect if sprite is outside of boundary rectangle
void Sprite::boundsCollisionCheck()
{
	hitType = COLLISION_NONE;
	int farBoundXPos = XPos + FrameWidth;
	int farBoundYPos = YPos + FrameHeight;
	if (XPos < boundsRect.Left || farBoundXPos > boundsRect.Right)
	{
		hitType = COLLISION_X;
	}
	else
	if (YPos < boundsRect.Top || farBoundYPos > boundsRect.Bottom)
	{
		hitType = COLLISION_Y;
	}
}

//things to do if collision detected  
void Sprite::boundsCollisionActions()
{
	switch (BoundsAction)  
	{
		//will kill sprite once death is detected by dead sprite cleanup run by list class
		case DIE:
			SpriteDeath();
		break;

		//sits still
		case STOP:
			SpriteDirection = STILL;
		break;

		//sends sprite to other side of screen
		case WRAP:
			if (hitType == COLLISION_X)
			{
				if (XPos < boundsRect.Left)
				{
					XPos = boundsRect.Right - FrameWidth;
				}
				else
				{
					XPos = boundsRect.Left;
				}
			}
			else
			{
				if (YPos < boundsRect.Top)
				{
					YPos = boundsRect.Bottom - FrameHeight;
				}
				else
				{
					YPos = boundsRect.Top;
				}
			}

		break;

		case BOUNCE:
			int modStep = actualDirections / 2;
			SpriteDirection = (SpriteDirection + modStep) % actualDirections;
		break;	
	}
}

//puts sprite position inside its boundary rectangle
void Sprite::SpawnInsideBoundsRectangle()
{
	int maXPosX = (boundsRect.Left + boundsRect.Width) - FrameWidth;
	int maXPosY = (boundsRect.Top + boundsRect.Height) - FrameWidth;
	XPos = rand->Next(boundsRect.Left, maXPosX);
	YPos = rand->Next(boundsRect.Top, maXPosY);
}

//accurately tracks the currentframe
void Sprite::UpdateFrame()
{
	currentFrame = (currentFrame + 1) % nFrames;
}

//randomises the current frame
void Sprite::RandomiseCurrentFrame()
{
	currentFrame = rand->Next(nFrames);
}

//updates number of frames and frame dimensions to fit supplied bitmap
void Sprite::SetSpriteSheet(Bitmap^ inSpriteSheet, int inNFrames)
{
	spriteSheets[SpriteDirection] = inSpriteSheet;
	FrameWidth = (int)spriteSheets[SpriteDirection]->Width / nFrames;
	FrameHeight = spriteSheets[SpriteDirection]->Height;
}

//randomises a sprites position
void Sprite::RandomisePosition()
{
	//set far boundaries to drop drawing off screen
	int spacerX = FrameWidth * 2;
	int spacerY = FrameHeight * 2;
	int boundaryX = clientRect.Width - spacerX;
	int boundaryY = clientRect.Height - spacerY;
	//generate positions
	XPos = rand->Next(spacerX, boundaryX);
	YPos = rand->Next(spacerY, boundaryY);
}

//give sprite a random direction
void Sprite::RandomiseDirection()
{
	int myDir = rand->Next(actualDirections);
	SpriteDirection = myDir;
}

//Causes sprite to change direction probabilistically
void Sprite::Wander()
{
	int prob = rand->Next(PROBABILITY_MAX);
	if (prob == 0)
	{
		RandomiseDirection();
	}
}

//calculates dimensions and position of a sprites collision rectangle
void Sprite::CalcCollisionRectangle()
{
	if (IsAlive == false)
	{
		CollisionRect = Rectangle(-1, -1, 0, 0); //if it's a dead sprite get rid of its collision rectangle so its remains doesn't interfer with live objects
	}
	else
	{
		double reductXDub = ((double)FrameWidth / 100) * reductXPerC;
		double reductYDub = ((double)FrameHeight / 100) * reductYPerC;
		int reductX = (int)reductXDub;
		int reductY = (int)reductYDub;
		int collRectXPos = XPos + reductX;
		int collRectYPos = YPos + reductY;
		int collRectWidth = FrameWidth - (reductX * 2);
		int collRectHeight = FrameHeight - (reductY * 2);
		CollisionRect = Rectangle(collRectXPos, collRectYPos, collRectWidth, collRectHeight);
	}
}

//unsetting the sprites ability to do anything
void Sprite::SpriteDeath()
{
	IsAlive = false;
	loadDeathImages();
	SpriteDirection = STILL;
	//take away collision rectangles
	boundsRect = clientRect;
}
	

//checks for collision between two sprites
bool Sprite::CollideWith(Sprite^ otherSprite)
{
	//setting up imaginary rectangles of points
	int myBottom = CollisionRect.Bottom;
	int myTop = CollisionRect.Top;
	int myLeft = CollisionRect.Left;
	int myRight = CollisionRect.Right;
	int otherBottom = otherSprite->CollisionRect.Bottom;
	int otherTop = otherSprite->CollisionRect.Top;
	int otherLeft = otherSprite->CollisionRect.Left;
	int otherRight = otherSprite->CollisionRect.Right;

	//comparisons
	bool collided = true;	

	if (myBottom < otherTop)
	{
		collided = false;
	}
	if (myTop > otherBottom)
	{
		collided = false;
	}
	if (myRight < otherLeft)
	{
		collided = false;
	}
	if (myLeft > otherRight)
	{
		collided = false;
	}

	return collided;
}

