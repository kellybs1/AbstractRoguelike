#include "SpriteList.h"

/*
SpriteList class: Manages a dynmaic list of Sprite objects.
Developed by Brendan Kelly throughout August and September, 2016.
*/

SpriteList::SpriteList()
{
}

//adds a new Sprite
void SpriteList::AddSprite(Sprite^ newSprite)
{
	if (head == nullptr)
	{
		tail = newSprite;
		head = newSprite;
	}
	else
	{
		tail->Next = newSprite;
		tail = newSprite;
	}
}

//deletes one Sprites 
void SpriteList::DeleteOneSprite(Sprite^ spriteToDelete)
{
	if (head == spriteToDelete) //if you are deleting the first node
	{
		if (tail == spriteToDelete) //if it is the only node in the list
		{
			head = nullptr; //head and tail both become nullptr
			tail = nullptr;
		}
		else
		{
			head = spriteToDelete->Next; //move head along to the second node
		}
	}
	else
	{
		Sprite^ nodeWalker = head;
		while (nodeWalker->Next != spriteToDelete) //find the node prior to the one to be deleted
		{
			nodeWalker = nodeWalker->Next;
		}
		nodeWalker->Next = spriteToDelete->Next; //change its next pointer to the deleted node's next pointer
		if (nodeWalker->Next == nullptr) //if you just deleted the last node in the list
		{
			tail = nodeWalker; //set the tail to the prior node
		}
	}
}

//deletes all Sprites
void SpriteList::DeleteAllSprites()
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		DeleteOneSprite(nodeWalker);
		nodeWalker = nodeWalker->Next;
	}
}

//deletes all dead Sprites
void SpriteList::DeleteAllDeadSprites()
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		if (nodeWalker->IsAlive == false)
		{
			DeleteOneSprite(nodeWalker);
		}
		nodeWalker = nodeWalker->Next;
	}
}

//wanders all sprites
void SpriteList::WanderAllSprites()
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		nodeWalker->Wander();
		nodeWalker = nodeWalker->Next;
	}
}

//updates frame number for all sprites
void SpriteList::UpdateAllSpriteFrames()
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		nodeWalker->UpdateFrame();
		nodeWalker = nodeWalker->Next;
	}
}

//moves all Sprites
void SpriteList::MoveSprites()
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		nodeWalker->Move();
		nodeWalker = nodeWalker->Next;
	}
}

//runs the wrapped up version of movement including collisions
void SpriteList::AllMoveActionsAllSprites()
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		nodeWalker->AllMoveActions();
		nodeWalker = nodeWalker->Next;
	}
}

//draws all Sprites
void SpriteList::DrawSprites()
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		nodeWalker->Draw();
		nodeWalker = nodeWalker->Next;
	}
}

//draws sprites in case of using scolling tilemap with viewport
void SpriteList::DrawSpritesInViewPort(int vPortWorldX, int vPortWorldY)
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		//calcuate sprite viewport location
		int spriteVportX = nodeWalker->XPos - vPortWorldX;
		int spriteVportY = nodeWalker->YPos - vPortWorldY;
		int frameWidth = nodeWalker->FrameWidth;
		int frameHeight = nodeWalker->FrameHeight;
		//calculate far edges of viewport
		int vPortFarBoundX = vPortWorldX + (VPORT_TILES_WIDE * TILE_SIDE);
		int vPortFarBoundY = vPortWorldX + (VPORT_TILES_HIGH * TILE_SIDE);
		//force draw the sprite if it's inside both bounds
		if (spriteVportX > 0-frameWidth && spriteVportX < vPortFarBoundX)
		{
			if (spriteVportY > 0-frameHeight && spriteVportY < vPortFarBoundY)
			{
				nodeWalker->ForceDraw(spriteVportX, spriteVportY);
			}
		}
		nodeWalker = nodeWalker->Next;
	}
}

//returns a count of total Sprites
int SpriteList::CountSprites()
{
	Sprite^ nodeWalker = head;
	int count = 0;

	while (nodeWalker != nullptr)
	{
		count++;
		nodeWalker = nodeWalker->Next;
	}
	return count;
}

//returns the first sprite that collided
Sprite^ SpriteList::CheckCollisions(Sprite^ otherSprite)
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		if (nodeWalker->CollideWith(otherSprite))
		{
			return nodeWalker;
		}
		nodeWalker = nodeWalker->Next;
	}
	return nodeWalker;
}

//returns an array of indexes where collisions were detected
array<int>^ SpriteList::CheckAllCollisions(Sprite^ otherSprite, int nSprites)
{
	int sIndex = 0;
	int aIndex = 0;
	array<int>^ collIndexes = gcnew array<int>(nSprites);
	//initialise all values to -1
	for (int i = 0; i < collIndexes->Length; i++)
	{
		collIndexes[i] = -1;
	}
	//locate collisions and add to indexes
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		if (nodeWalker->CollideWith(otherSprite))
		{
			collIndexes[aIndex] = sIndex;
			aIndex++;
		}
		sIndex++;
		nodeWalker = nodeWalker->Next;
	}
	return collIndexes;
}

//returns the sprite at any given index
Sprite^ SpriteList::AtIndex(int index)
{
	Sprite^ nodeWalker = head;
	for (int i = 0; i < index; i++)
	{
		nodeWalker = nodeWalker->Next;
	}
	return nodeWalker;
}

//kills all sprites that have hit a non walkable tile
void SpriteList::KillOnWallAllSprites()
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		nodeWalker->KillOnWall();
		nodeWalker = nodeWalker->Next;
	}
}

//moves sprites if limited checks pass
void SpriteList::LimitedMoveSprites()
{
	Sprite^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		nodeWalker->LimitedMove();
		nodeWalker = nodeWalker->Next;
	}
}

