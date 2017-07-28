#pragma once
#include "Sprite.h"

/*
EnemySprite class: Holds and manages information regarding sprite which are collectable items.
Developed by Brendan Kelly 19-30 September, 2016.
*/

#define ITEM_TYPE_HEALTH 0
#define ITEM_TYPE_MANA 1
#define ITEM_TYPE_LOOT 2
#define HEALTH_ITEM_AMOUNT 15
#define MANA_ITEM_AMOUNT 20
#define LOOT_ITEM_AMOUNT 42

ref class ItemSprite :
public Sprite
{

private: 
	int itemType;

public:
	ItemSprite(Graphics^ inGraphics, array<Bitmap^>^ startFileNames, Random^ inRand, int inNFrames, int inXPos, int inYPos, Rectangle inClientRect, 
		int inBoundsAction, Bitmap^ inSplatImg, int inReductXPerC, int inReductYPerC, TileMap^ inTileMap, int inVelocity, int inItemType);

	int getItemType() { return itemType; }
	void setItemType(int inItemType) { itemType = inItemType; }
	
};

