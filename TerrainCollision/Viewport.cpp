#include "Viewport.h"

/*
ViewPort class: Manages a viewport "window" to scroll around a larger tilemap.
Developed by Brendan Kelly throughout August,September, 2016.
*/

Viewport::Viewport(int inX, int inY, TileMap^ inWorldMap, Graphics^ inCanvas)
{
	VPortWorldX = inX;
	VPortWorldY = inY;
	worldMap = inWorldMap;
	canvas = inCanvas;
	xTile = VPortWorldX / TILE_SIDE;
	yTile = VPortWorldY / TILE_SIDE;
	xOffset = VPortWorldX  % TILE_SIDE;
	yOffset = VPortWorldY % TILE_SIDE;
}

void Viewport::VPortMove(int xMove, int yMove)
{
	int testVPortWorldX = VPortWorldX + xMove;
	int testVPortWorldY = VPortWorldY + yMove;
	int farboundX = (TILE_SIDE * COLCOUNT) - (VPORT_TILES_WIDE * TILE_SIDE);
	int farboundY = (TILE_SIDE * ROWCOUNT) - (VPORT_TILES_HIGH * TILE_SIDE);
	if (testVPortWorldX < farboundX && testVPortWorldX > 0)
	{
		VPortWorldX = testVPortWorldX;
	}
	if (testVPortWorldY < farboundY  && testVPortWorldY > 0)
	{
		VPortWorldY = testVPortWorldY;
	}
}

//keeeps the viewport positioned around the player
void Viewport::VPortMoveOnPlayer(int playerX, int playerY)
{
	int testVPortWorldX = playerX - ((VPORT_TILES_WIDE * TILE_SIDE) / 2);
	int testVPortWorldY = playerY - ((VPORT_TILES_HIGH * TILE_SIDE) / 2);
	int farboundX = (TILE_SIDE * COLCOUNT) - (VPORT_TILES_WIDE * TILE_SIDE);
	int farboundY = (TILE_SIDE * ROWCOUNT) - (VPORT_TILES_HIGH * TILE_SIDE);
	if (testVPortWorldX > 0 && testVPortWorldX < farboundX)
	{
		VPortWorldX = testVPortWorldX;
	}
	if (testVPortWorldY > 0 && testVPortWorldY < farboundY)
	{
		VPortWorldY = testVPortWorldY;
	}
}


//draws the viewport on screen
void Viewport::VPortDraw()
{
	calcTilesAndOffsets();

	for (int r = yTile; r < yTile + VPORT_TILES_HIGH + 1; r++)
	{
		for (int c = xTile; c < xTile + VPORT_TILES_WIDE + 1; c++)
		{
			//find the right bitmap
			int tileIndex = worldMap->GetMapEntry(r, c);
			Bitmap^ workingBitmap = worldMap->GetMapBitmap(tileIndex);

			//calculate dimensions and location
			int tilePosX = ((c - xTile) * TILE_SIDE) - xOffset;
			int tilePosY = ((r - yTile) * TILE_SIDE) - yOffset;

			//draw tile to screen
			Rectangle drawRect = Rectangle(tilePosX, tilePosY, TILE_SIDE, TILE_SIDE);
			canvas->DrawImage(workingBitmap, drawRect);
		}
	}
}

//draws the fog over tiles based on the corresponding boolean setting in the fogMap
void Viewport::VPortDrawFog(array<bool, 2>^ fogMap, Bitmap^ fogImg)
{
	calcTilesAndOffsets();

	for (int r = yTile; r < yTile + VPORT_TILES_HIGH + 1; r++)
	{
		for (int c = xTile; c < xTile + VPORT_TILES_WIDE + 1; c++)
		{
			//calculate dimensions and location
			int tilePosX = ((c - xTile) * TILE_SIDE) - xOffset;
			int tilePosY = ((r - yTile) * TILE_SIDE) - yOffset;

			//draw tile to screen if its fog
			if (fogMap[r, c] == true)
			{
				Rectangle drawRect = Rectangle(tilePosX, tilePosY, TILE_SIDE, TILE_SIDE);
				canvas->DrawImage(fogImg, drawRect);
			}
		}
	}
}

//calculation for draw method
void Viewport::calcTilesAndOffsets()
{
	xTile = VPortWorldX / TILE_SIDE;
	yTile = VPortWorldY / TILE_SIDE;
	xOffset = VPortWorldX  % TILE_SIDE;
	yOffset = VPortWorldY % TILE_SIDE;
}
