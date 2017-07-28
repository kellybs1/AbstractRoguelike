#include "Tile.h"

/*
Tile class: Holds and manages information regarding single Tile images that are used to create tilemaps.
Developed by Brendan Kelly throughout August,September, 2016.
*/

Tile::Tile(Bitmap^ startTileImage)
{
	TileImage = startTileImage;
	IsWalkable = false;
}
