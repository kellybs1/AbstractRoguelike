#pragma once
#include "Tile.h"

/*
TileList class: Is a collection of tiles to be used in a TileMap.
Developed by Brendan Kelly throughout August,September, 2016.
*/

#define TILECOUNT 5

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class TileList
{
public:
	TileList(int inTileCount);

	//returns the bitmap, from the array, at given index
	Bitmap^ GetTileBitmap(int tileIndex);

	//places given tile at given location in array
	void SetTileArrayEntry(int tileIndex, Tile^ tileToEnter);

	//returns the status of the tile to be tested 
	bool isThisTileWalkable(int inTileIndex);

private:
	array<Tile^>^ tileArray;
};

