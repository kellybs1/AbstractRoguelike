#pragma once

/*
Tile class: Holds and manages information regarding single Tile images that are used to create tilemaps.
Developed by Brendan Kelly throughout August,September, 2016.
*/

#define TILE_SIDE 64 //this must be the size of tile image - default 64, 16 for test map view
#define TILE_TYPE_DIRT 0
#define TILE_TYPE_WALL 1
#define TILE_TYPE_FLOOR 2
#define TILE_TYPE_CORRIDOR 3
#define TILE_TYPE_DOOR 4

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class Tile
{
public:
	Tile(Bitmap^ startImage);
	property Bitmap^ TileImage;
	property bool IsWalkable;
};

