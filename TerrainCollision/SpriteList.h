#pragma once

/*
SpriteList class: Manages a dynmaic list of Sprite objects.
Developed by Brendan Kelly throughout August and September, 2016.
*/

#include "Sprite.h"
#include "Viewport.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class SpriteList
{
private:
	Sprite^ head;
	Sprite^ tail;

public:
	SpriteList(void);
	void AddSprite(Sprite^ newSprite);
	void DeleteOneSprite(Sprite^ spriteToDelete);
	void DeleteAllSprites();
	void DeleteAllDeadSprites();
	void WanderAllSprites();
	void UpdateAllSpriteFrames();
	void KillOnWallAllSprites();
	void MoveSprites();
	void LimitedMoveSprites();
	void AllMoveActionsAllSprites();
	void DrawSprites();
	int CountSprites();
	void DrawSpritesInViewPort(int vX, int vY);
	Sprite^ CheckCollisions(Sprite^ otherSprite);
	array<int>^ CheckAllCollisions(Sprite^ otherSprite, int nSprites);
	Sprite^ AtIndex(int index);
};

