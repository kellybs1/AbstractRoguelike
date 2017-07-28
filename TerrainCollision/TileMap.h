#pragma once

#include "Tile.h"
#include "TileList.h"

/*
TileMap class: Manages a two dimension map of tiles.
Developed by Brendan Kelly throughout August,September, 2016.
*/

//number of rows and columns in the map file
#define ROWCOUNT 48
#define COLCOUNT 64

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;

ref class TileMap
{

public:
	TileMap(TileList^ startTileList, Graphics^ startCanvas);

	//sets the tile index at the given column and row
	void SetMapEntry(int col, int row, int tileIndex);

	//returns the tile's index from the given row and column
	int GetMapEntry(int col, int row);

	//returns bitmap from given index in list
	Bitmap^ GetMapBitmap(int tileIndex);

	//draws the map to the canvas
	void DrawMap();

	//loads map from csv file
	void LoadMapFromFile(String^ mapFileName);

	//loads maps from array generated elsewhere
	void loadMapFromArray(array<int, 2>^ generatedArray);

	//returns the walkable status of a tile at given co-ordinates
	bool isTileWalkable(int col, int row);

private:
	TileList^ workingTileList;
	array<int, 2>^ map;
	Graphics^ canvas;
};

