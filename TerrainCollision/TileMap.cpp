#include "TileMap.h"

/*
TileMap class: Manages a two dimension map of tiles.
Developed by Brendan Kelly throughout August,September, 2016.
*/

TileMap::TileMap(TileList^ startTileList, Graphics^ startCanvas)
{
	workingTileList = startTileList;
	canvas = startCanvas;
	map = gcnew array<int, 2>(ROWCOUNT, COLCOUNT);
}

//sets the tile index at the given column and row
void TileMap::SetMapEntry(int col, int row, int tileIndex)
{
	map[col, row] = tileIndex;
}

//returns the tile's index from the given row and column
int TileMap::GetMapEntry(int col, int row)
{
	int out = map[col, row];
	return out;
}

//returns bitmap from given index in list
Bitmap^ TileMap::GetMapBitmap(int tileIndex)
{
	Bitmap^ outBitmap = workingTileList->GetTileBitmap(tileIndex);
	return outBitmap;
}

//draws the map to the canvas
void TileMap::DrawMap()
{
	//for each column on each row
	for (int r = 0; r < ROWCOUNT; r++)
	{
		for (int c = 0; c < COLCOUNT; c++)
		{
			//find the right bitmap
			int tileIndex = map[r, c];
			Bitmap^ workingBitmap = workingTileList->GetTileBitmap(tileIndex);

			//calculate dimensions and location
			int tileSize = workingBitmap->Width; //it's a square! Or else this won't work
			int tilePosX = c * tileSize;
			int tilePosY = r * tileSize;

			//draw tile to screen
			Rectangle drawRect = Rectangle(tilePosX, tilePosY, tileSize, tileSize);
			canvas->DrawImage(workingBitmap, drawRect);
		}
	}
}

//loads map from csv file
void TileMap::LoadMapFromFile(String^ mapFileName)
{
	//open text file
	StreamReader^ sr = File::OpenText(mapFileName);
	String^ currentLine = "";
	//creates a number-of-columns place to put singular indexes read from file
	array<String^>^ rowOfIndexes = gcnew array<String^>(COLCOUNT);
	int rowCount = 0;
	while (currentLine = sr->ReadLine())							//while a line is read
	{
		rowOfIndexes = currentLine->Split(',');					//break the line into an array
		for (int i = 0; i < rowOfIndexes->Length; i++)			//for each item in the array
		{
			int myIndex = Convert::ToInt32(rowOfIndexes[i]);		//make it an int
			map[rowCount, i] = myIndex;							//assign it to the correct spot
		}
		rowCount++;												//drop down to the next row
	}
}

//loads the map from an array
void TileMap::loadMapFromArray(array<int, 2>^ generatedArray)
{
	map = generatedArray;

	for (int r = 0; r < ROWCOUNT; r++)
	{
		for (int c = 0; c < COLCOUNT; c++)
		{
			map[r, c] = generatedArray[r, c];
		}
	}
}

//returns the walkable status of a tile at given co-ordinates
bool TileMap::isTileWalkable(int col, int row)
{
	bool walkable = true;
	int tileIndex = map[col, row];
	walkable = workingTileList->isThisTileWalkable(tileIndex);
	return walkable;
}
