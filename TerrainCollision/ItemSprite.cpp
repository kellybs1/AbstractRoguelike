#include "ItemSprite.h"

/*
EnemySprite class: Holds and manages information regarding sprite which are collectable items.
Developed by Brendan Kelly 19-30 September, 2016.
*/

ItemSprite::ItemSprite(Graphics^ inGraphics, array<Bitmap^>^ startFileNames, Random^ inRand, int inNFrames, int inXPos, int inYPos, Rectangle inClientRect, 
	int inBoundsAction, Bitmap^ inSplatImg, int inReductXPerC, int inReductYPerC, TileMap^ inTileMap, int inVelocity, int inItemType) : Sprite(inGraphics, 
	startFileNames, inRand, inNFrames, inXPos, inYPos, inClientRect, inBoundsAction, inSplatImg, inReductXPerC, inReductYPerC, inTileMap, inVelocity)
{
	itemType = inItemType;
}

