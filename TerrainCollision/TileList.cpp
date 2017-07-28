#include "TileList.h"

/*
TileList class: Is a collection of tiles to be used in a TileMap.
Developed by Brendan Kelly throughout August,September, 2016.
*/

TileList::TileList(int inTileCount)
{
	
	tileArray = gcnew array<Tile^>(inTileCount);
}

//returns the bitmap, from the array, at given index
Bitmap^  TileList::GetTileBitmap(int tileIndex)
{
	Bitmap^ tileOut = tileArray[tileIndex]->TileImage;
	return tileOut;
}

//places given tile at given location in array
void TileList::SetTileArrayEntry(int tileIndex, Tile^ tileToEnter)
{
	tileArray[tileIndex] = tileToEnter;
}

//returns the status of the tile to be tested 
bool TileList::isThisTileWalkable(int inTileIndex)
{	
	return tileArray[inTileIndex]->IsWalkable;
}
