#pragma once

#include "TileMap.h"

/*
ViewPort class: Manages a viewport "window" to scroll around a larger tilemap.
Developed by Brendan Kelly throughout August,September, 2016.
*/

//originally 20, 10, 60,44 to see whole map but does not play well with sprites

//divide resolution by tile size //and these could be set manually 30/16 for 1920x1080
#define VPORT_TILES_WIDE 20
#define VPORT_TILES_HIGH 12

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class Viewport
{
private:

	TileMap^ worldMap;
	Graphics^ canvas;
	int xTile;
	int yTile;
	int xOffset;
	int yOffset;

	void calcTilesAndOffsets();


public:
	Viewport(int inX, int inY, TileMap^ inWorldMap, Graphics^ inCanvas);

	void VPortMove(int xMove, int yMove);
	void VPortMoveOnPlayer(int playerX, int playerY);
	void VPortDraw();
	void VPortDrawFog(array<bool, 2>^ fogMap, Bitmap^ fogImg);

	property int VPortWorldX;
	property int VPortWorldY;
};

