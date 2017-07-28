#include "GameManager.h"

/*
GameManager class: Manages creation of all game objects and interaction between objects.
Developed by Brendan Kelly 19-30 September, 2016.
*/

GameManager::GameManager(Graphics^ inCanvas, Random^ inRand, Rectangle inClientRect)
{
	canvas = inCanvas;
	rand = inRand;
	clientRect = inClientRect;
	nFrames = SPRITE_FRAMES;
	nHealthItems = START_NHEALTH_ITEMS;
	nManaItems = START_NMANA_ITEMS;
	nLootItems = START_NLOOT_ITEMS;
	nTotalItems = nHealthItems + nManaItems + nLootItems;
	nEnemyTurrets = START_NENEMY_TURRETS;
	nEnemyRandoms = START_NENEMY_RANDOMS;
	nEnemyChasers = START_NENEMY_CHASERS;
	nTotalEnemies = nEnemyChasers + nEnemyRandoms + nEnemyTurrets;
	playerVel = START_PLAYERVEL;
	enemyVel = START_ENEMYVEL;
	bulletVel = START_BULLETVEL;
	itemVel = START_ITEMVEL;
	Level = START_LEVEL;
	damageIncrease = 0;
	battleReadyTimer = DAMAGE_BATTLE_TIMER; //so damage can start immediately
	PlayerScore = 0;
	playerNewX = 0;
	playerNewY = 0;
	LeftDown = false;
	RightDown = false;
	UpDown = false;
	DownDown = false;
	loadMiscImages();
	//make tileList
	mainTileList = gcnew TileList(TILECOUNT);
	fillTileList();
	//make dungeon
	mainDungeon = gcnew Dungeon(rand);
	mainDungeon->RandomRoomCount();
	mainDungeon->GenerateNewDungeon();
	generatedDungeon = mainDungeon->GenerateTileMapFromDungeon();
	//make tilemap
	//mapFileName = "maps/largeMyMap.csv";
	//mapFileName = "maps/largeMyMapTwoTile.csv";
	mainTileMap = gcnew TileMap(mainTileList, canvas);
	//mainTileMap->LoadMapFromFile(mapFileName);
	mainTileMap->loadMapFromArray(generatedDungeon);
	//make player
	loadPlayerImages();
	makePlayer();
	//make enemies             
	loadEnemyImages();
	makeEnemies();
	//make items
	loadHealthItemImages();
	loadManaItemImages();
	loadLootItemImages();
	makeItems();
	//make fog fap
	fogMap = gcnew array<bool, 2>(ROWCOUNT, COLCOUNT);
	initFogMap();
	//make viewport
    mainVPort = gcnew Viewport(0, 0, mainTileMap, canvas);
	//make bullets
	loadBulletImages();
	centeringBullet = bulletFiles[0]->Height / 2; //used for putting bullet in centre of player sprite when bullet is fired - only want to calculate this once
	bullets = gcnew SpriteList();
	enemyBullets = gcnew SpriteList();
	//make warp to new level
	loadLevelWarpImages();
	makeLevelWarp();
}

//makes a new dungeon level
void GameManager::NextDungeon()
{
	GC::Collect(0);
	mainDungeon->RandomRoomCount();
	mainDungeon->GenerateNewDungeon();
	generatedDungeon = mainDungeon->GenerateTileMapFromDungeon();
	mainTileMap->loadMapFromArray(generatedDungeon);
	//reloading images here stops some imges being transparent when they shouldn't
	loadMiscImages();
	loadBulletImages();
	loadHealthItemImages();
	loadLootItemImages();
	loadManaItemImages();
	loadPlayerImages();
	//relocate player
	player->SpawnOnRandomTileTypeOf(TILE_TYPE_CORRIDOR);
	while (playerSpawnBreakingViewport())
	{
		player->SpawnOnRandomTileTypeOf(TILE_TYPE_CORRIDOR);
	}
	//more enemies!
	nEnemyChasers++;
	//make enemies 
	enemies->DeleteAllSprites();
	loadEnemyImages();
	makeEnemies();
	//make items
	items->DeleteAllSprites();
	makeItems();
	//make levelwarp
	levelWarps->DeleteAllSprites();
	loadLevelWarpImages();
	makeLevelWarp();
	//reset fog
	initFogMap();
	//next level
	damageIncrease += DAMAGE_INCREASE_PER_LEVEL;
	Level++;
}

//the cycle of actions run every tick
void GameManager::GameCycle()
{
	int playerXWorld = player->XPos;
	int playerYWorld = player->YPos;
	mainVPort->VPortMoveOnPlayer(playerXWorld, playerYWorld);
	mainVPort->VPortDraw();
	enemyActions();
	itemActions();
	bulletActions(bullets);
	bulletActions(enemyBullets);
	levelWarpActions();
	//unFog();
	//mainVPort->VPortDrawFog(fogMap, fogImg);
	playerActions();
	playerEnemyCollision();
	playerBulletCollision();
	playerWarpCollision();
	updateDisplayValues();
}

//populates the tile list with tiles
	void GameManager::fillTileList()
	{
		Bitmap^ file0 = gcnew Bitmap("images/map/Dirt.bmp");
		Tile^ tile0 = gcnew Tile(file0);
		mainTileList->SetTileArrayEntry(TILE_TYPE_DIRT, tile0);

		Bitmap^ file1 = gcnew Bitmap("images/map/Wall.bmp");
		Tile^ tile1 = gcnew Tile(file1);
		mainTileList->SetTileArrayEntry(TILE_TYPE_WALL, tile1);

		Bitmap^ file2 = gcnew Bitmap("images/map/Floor.bmp");
		Tile^ tile2 = gcnew Tile(file2);
		tile2->IsWalkable = true;
		mainTileList->SetTileArrayEntry(TILE_TYPE_FLOOR, tile2);

		Bitmap^ file3 = gcnew Bitmap("images/map/Corridor.bmp");
		Tile^ tile3 = gcnew Tile(file3);
		tile3->IsWalkable = true;
		mainTileList->SetTileArrayEntry(TILE_TYPE_CORRIDOR, tile3);

		Bitmap^ file4 = gcnew Bitmap("images/map/Door.bmp");
		Tile^ tile4 = gcnew Tile(file4);
		tile4->IsWalkable = true;
		mainTileList->SetTileArrayEntry(TILE_TYPE_DOOR, tile4);	
	}


	//fill player character images array with locations
	void GameManager::loadPlayerImages()
	{
				 playerFiles = gcnew array<Bitmap^>(MAX_DIRECTIONS);	
				 playerFiles[0] = gcnew Bitmap("images/characters/North.png");
				 playerFiles[1] = gcnew Bitmap("images/characters/NorthEast.png");
				 playerFiles[2] = gcnew Bitmap("images/characters/East.png");
				 playerFiles[3] = gcnew Bitmap("images/characters/SouthEast.png");
				 playerFiles[4] = gcnew Bitmap("images/characters/South.png");
				 playerFiles[5] = gcnew Bitmap("images/characters/SouthWest.png");
				 playerFiles[6] = gcnew Bitmap("images/characters/West.png");
				 playerFiles[7] = gcnew Bitmap("images/characters/NorthWest.png");
				 playerFiles[8] = gcnew Bitmap("images/characters/NorthWest.png"); //still image - is overwritten at direction change


	}

	void GameManager::loadHealthItemImages()
	{
		healthItemFiles = gcnew array<Bitmap^>(MAX_DIRECTIONS);
		for (int i = 0; i < healthItemFiles->Length; i++)
		{
			healthItemFiles[i] = gcnew Bitmap("images/items/healthballsml16.bmp");
		}
	}

	void GameManager::loadManaItemImages()
	{
		manaItemFiles = gcnew array<Bitmap^>(MAX_DIRECTIONS);
		for (int i = 0; i < manaItemFiles->Length; i++)
		{
			manaItemFiles[i] = gcnew Bitmap("images/items/manaballsml16.bmp");
		}
	}


	void GameManager::loadLootItemImages()
	{
		lootItemFiles = gcnew array<Bitmap^>(MAX_DIRECTIONS);
		for (int i = 0; i < lootItemFiles->Length; i++)
		{
			lootItemFiles[i] = gcnew Bitmap("images/items/lootballsml16.bmp");
		}
	}

	//creating images for level transfer sprite
	void GameManager::loadLevelWarpImages()
	{
		levelWarpFiles = gcnew array<Bitmap^>(MAX_DIRECTIONS);
		for (int i = 0; i < levelWarpFiles->Length; i++)
		{
			levelWarpFiles[i] = gcnew Bitmap("images/items/warpway.png");
		}

	}

	//fills bullet image arrays with bitmaps
	void GameManager::loadBulletImages()
	{
		bulletFiles = gcnew array<Bitmap^>(MAX_DIRECTIONS);
		for (int i = 0; i < bulletFiles->Length; i++)
		{
			bulletFiles[i] = gcnew Bitmap("images/items/fireballsml16.bmp");
		}

		enemyBulletFiles = gcnew array<Bitmap^>(MAX_DIRECTIONS);
		for (int i = 0; i < enemyBulletFiles->Length; i++)
		{
			enemyBulletFiles[i] = gcnew Bitmap("images/items/enemyfireballsml16.bmp");
		}
	}

	//fill enemy images array with images
	void GameManager::loadEnemyImages()
	{
		enemyFiles = gcnew array<Bitmap^>(MAX_DIRECTIONS);
		for (int i = 0; i < enemyFiles->Length; i++)
		{
			//omnidirectional enemy sprites
			enemyFiles[i] = gcnew Bitmap("images/characters/turret.png");
		}

		chaserFiles = gcnew array<Bitmap^>(MAX_DIRECTIONS);
		for (int i = 0; i < chaserFiles->Length; i++)
		{
			//omnidirectional enemy sprites
			chaserFiles[i] = gcnew Bitmap("images/characters/chaser.png");
		}

		randoFiles = gcnew array<Bitmap^>(MAX_DIRECTIONS);
		for (int i = 0; i < randoFiles->Length; i++)
		{
			//omnidirectional enemy sprites
			randoFiles[i] = gcnew Bitmap("images/characters/rando.png");
		}
	}

	//loads other miscellaneous images
	void GameManager::loadMiscImages()
	{
		splatImg = gcnew Bitmap("images/characters/splatsheet2.bmp");
		playerSplatImg = gcnew Bitmap("images/characters/splatsheet.bmp");
		blankImg = gcnew Bitmap("images/characters/blankSheet.bmp");
		fogImg = gcnew Bitmap("images/map/fog.bmp");
	}

	void GameManager::makeItems()
	{
		items = gcnew SpriteList();
		for (int i = 0; i < nHealthItems; i++)
		{
			Sprite^ newHealth = gcnew ItemSprite(canvas, healthItemFiles, rand, nFrames, 0, 0, clientRect, STOP, blankImg, 2, 2, mainTileMap, itemVel, ITEM_TYPE_HEALTH);
			newHealth->SetBoundsRectToTileMapEdges();
			newHealth->SpriteDirection = EAST;
			newHealth->RandomiseCurrentFrame();
			newHealth->SpawnOnRandomWalkableTile();
			items->AddSprite(newHealth);
		}
		for (int i = 0; i < nManaItems; i++)
		{
			Sprite^ newMana = gcnew ItemSprite(canvas, manaItemFiles, rand, nFrames, 0, 0, clientRect, STOP, blankImg, 2, 2, mainTileMap, itemVel, ITEM_TYPE_MANA);
			newMana->SetBoundsRectToTileMapEdges();
			newMana->SpriteDirection = EAST;
			newMana->RandomiseCurrentFrame();
			newMana->SpawnOnRandomWalkableTile();
			items->AddSprite(newMana);
		}
		for (int i = 0; i < nLootItems; i++)
		{
			Sprite^ newLoot = gcnew ItemSprite(canvas, lootItemFiles, rand, nFrames, 0, 0, clientRect, STOP, blankImg, 2, 2, mainTileMap, itemVel, ITEM_TYPE_LOOT);
			newLoot->SetBoundsRectToTileMapEdges();
			newLoot->SpriteDirection = EAST;
			newLoot->RandomiseCurrentFrame();
			newLoot->SpawnOnRandomWalkableTile();
			items->AddSprite(newLoot);
		}

		nTotalItems = items->CountSprites();
	}

	void GameManager::itemActions()
	{
		int vPortX = mainVPort->VPortWorldX;
		int vPortY = mainVPort->VPortWorldY;
		collectItems();
		items->AllMoveActionsAllSprites();
		items->DrawSpritesInViewPort(vPortX, vPortY);
	}


	void GameManager::collectItems()
	{
		//first find the item as a sprite
		Sprite^ spriteItem = items->CheckCollisions(player);
		if (spriteItem != nullptr)
		{
			//cast to ItemSprite so properties can be accessed
			ItemSprite^ hitItem = (ItemSprite^)spriteItem;
			switch (hitItem->getItemType())
			{
			case ITEM_TYPE_HEALTH:
				//if we have enough health don't pick it up, otherwise use the health and remove the item
				if (player->Health < SPRITE_HEALTH_MAX)
				{
					if (player->Health + HEALTH_ITEM_AMOUNT >= SPRITE_HEALTH_MAX)
					{
						player->Health = SPRITE_HEALTH_MAX;
					}
					else
					{
						player->Health += HEALTH_ITEM_AMOUNT;
					}
					items->DeleteOneSprite(hitItem);
				}
				break;

			case ITEM_TYPE_MANA:
				//if we have enough mana don't pick it up, otherwise use the mana and remove the item
				if (player->Mana < SPRITE_MANA_MAX)
				{

					if (player->Mana + MANA_ITEM_AMOUNT >= SPRITE_MANA_MAX)
					{
						player->Mana = SPRITE_MANA_MAX;
					}
					else
					{
						player->Mana += MANA_ITEM_AMOUNT;
					}
					items->DeleteOneSprite(hitItem);
				}
				break;

			case ITEM_TYPE_LOOT:
				//pick it up, increase score
				PlayerScore += LOOT_ITEM_AMOUNT;
				items->DeleteOneSprite(hitItem);
				break;
			}
		}
	}

	//sets all fogmap as fog to start
	void GameManager::initFogMap()
	{
		for (int r = 0; r < ROWCOUNT; r++)
		{
			for (int c = 0; c < COLCOUNT; c++)
			{
				fogMap[r, c] = true;
			}
		}
	}

	//sets the fogMap not fog in an area around the player
	void GameManager::unFog()
	{
		//calculate where player is
		int playerXWorld = player->XPos;
		int playerYWorld = player->YPos;
		int playerXTile = playerXWorld / TILE_SIDE;
		int playerYTile = playerYWorld / TILE_SIDE;
		//calculate tiles to start and end unfogging on
		int startXTile = playerXTile - VISION_HORIZONTAL_RADIUS;
		int startYTile = playerYTile - VISION_VERTICAL_RADIUS;
		int endXTile = startXTile + (VISION_HORIZONTAL_RADIUS * 2);
		int endYTile = startYTile + (VISION_VERTICAL_RADIUS * 2);
		for (int c = startXTile; c < endXTile; c++)
		{
			for (int r = startYTile; r < endYTile; r++)
			{
				//DO NOT DO ANYTHING if r or c are outside of the array bounds! this causes exceptions
				if (r > -1 && c > -1)
				{
					if (r < ROWCOUNT && c < COLCOUNT)
					{
						fogMap[r, c] = false;
					}
				}
			}
		}
	}


	//making level warp
	void GameManager::makeLevelWarp()
	{
		levelWarps = gcnew SpriteList();
		levelWarp = gcnew Sprite(canvas, levelWarpFiles, rand, nFrames, 0, 0, clientRect, STOP, splatImg, 10, 10, mainTileMap, itemVel);
		levelWarp->SpawnOnRandomTileTypeOf(TILE_TYPE_FLOOR);
		levelWarps->AddSprite(levelWarp);
	}

	//everthing a level wap does it a tick
	void GameManager::levelWarpActions()
	{	
		int vPortX = mainVPort->VPortWorldX;
		int vPortY = mainVPort->VPortWorldY;
		levelWarps->AllMoveActionsAllSprites();
		levelWarps->DrawSpritesInViewPort(vPortX, vPortY);
	}

	//warps to new level when player collides with warp
	void GameManager::playerWarpCollision()
	{
		Sprite^ hitWarp = levelWarps->CheckCollisions(player);
		if (hitWarp != nullptr)
		{
			NextDungeon();
		}
	}


			 //creates the enemies link list
	void GameManager::makeEnemies()
	{
		enemies = gcnew SpriteList();
		//random movers
		for (int m = 0; m < nEnemyRandoms; m++)
		{
			Sprite^ newEnemy = gcnew EnemySprite(canvas, randoFiles, rand, nFrames, 0, 0, clientRect, BOUNCE, splatImg, 10, 10, mainTileMap, enemyVel, ENEMY_TYPE_RANDO, ENEMY_STATE_WANDER);
			newEnemy->SpawnOnRandomTileTypeOf(TILE_TYPE_FLOOR);
			newEnemy->RandomiseDirection();
			newEnemy->RandomiseCurrentFrame();
			newEnemy->SetBoundsRectToTileMapEdges();
			enemies->AddSprite(newEnemy);
		}
		//chasers
		for (int c = 0; c < nEnemyChasers; c++)
		{
			Sprite^ newEnemy = gcnew EnemySprite(canvas, chaserFiles, rand, nFrames, 0, 0, clientRect, BOUNCE, splatImg, 10, 10, mainTileMap, enemyVel, ENEMY_TYPE_CHASER, ENEMY_STATE_WANDER);
			newEnemy->SpawnOnRandomTileTypeOf(TILE_TYPE_FLOOR);
			newEnemy->SpriteDirection = STILL;
			newEnemy->RandomiseCurrentFrame();
			newEnemy->SetBoundsRectToTileMapEdges();
			enemies->AddSprite(newEnemy);
		}
		//turrets
		for (int t = 0; t < nEnemyTurrets; t++)
		{
			Sprite^ newEnemy = gcnew EnemySprite(canvas, enemyFiles, rand, nFrames, 0, 0, clientRect, BOUNCE, splatImg, 10, 10, mainTileMap, enemyVel, ENEMY_TYPE_TURRET, ENEMY_STATE_HUNT);
			newEnemy->SpawnOnRandomTileTypeOf(TILE_TYPE_FLOOR);
			newEnemy->SpriteDirection = STILL;
			newEnemy->RandomiseCurrentFrame();
			EnemySprite^ eSprite = (EnemySprite^)newEnemy;
			eSprite->randomiseBulletTimer();
			newEnemy->SetBoundsRectToTileMapEdges();
			enemies->AddSprite(newEnemy);
		}		

		nTotalEnemies = enemies->CountSprites();
	}


	//everything enemies have to do in a timer tick
	void GameManager::enemyActions()
	{
		damageEnemies();
		enemies->AllMoveActionsAllSprites();
		enemies->LimitedMoveSprites();
		for (int i = 0; i < nTotalEnemies; i++)
		{
			Sprite^ spriteEnemy = enemies->AtIndex(i);
			EnemySprite^ currentEnemy = (EnemySprite^)spriteEnemy;
			currentEnemy->updateState(player);
			currentEnemy->doAction(player);
			//create bullets if turret is ready to fire
			if (currentEnemy->getEnemyType() == ENEMY_TYPE_TURRET && currentEnemy->IsAlive)
			{
				if (currentEnemy->getBulletTimer() == 0)
				{
					makeEnemyBullet(currentEnemy);
				}
			}		
		}
		int vPortX = mainVPort->VPortWorldX;
		int vPortY = mainVPort->VPortWorldY;
		enemies->DrawSpritesInViewPort(vPortX, vPortY);
	}

	//creates the player
	void GameManager::makePlayer()
	{
		player = gcnew Sprite(canvas, playerFiles, rand, nFrames, 0, 0, clientRect, STOP, playerSplatImg, 20, 20, mainTileMap, playerVel);
		player->SpawnOnRandomTileTypeOf(TILE_TYPE_CORRIDOR);
		while (playerSpawnBreakingViewport())
		{
			player->SpawnOnRandomTileTypeOf(TILE_TYPE_CORRIDOR);
		}
		player->SetBoundsRectToTileMapEdges();
	}

	//stops the player spawning around edges where viewport can't centre on player
	//that causes generation of unplayable dungeons
	bool GameManager::playerSpawnBreakingViewport()
	{
		int playerXWorld = player->XPos;
		int playerYWorld = player->YPos;
		int halfVportX = (VPORT_TILES_WIDE  * TILE_SIDE) / 2;
		int halfVportY = (VPORT_TILES_HIGH * TILE_SIDE) / 2;
		int worldWidth = COLCOUNT * TILE_SIDE;
		int worldHeight = ROWCOUNT * TILE_SIDE;
		bool spawningOnEdge = false;
		if (playerXWorld <= halfVportX)
		{
			spawningOnEdge = true;
		}
		if (playerXWorld >= worldWidth - halfVportX - (player->FrameWidth * 2))
		{
			spawningOnEdge = true;
		}
		if (playerYWorld <= halfVportY)
		{
			spawningOnEdge = true;
		}
		if (playerYWorld >= worldHeight - halfVportY - (player->FrameHeight * 2))
		{
			spawningOnEdge = true;
		}
		return spawningOnEdge;
	
	}

	//everything a knight have to do in a timer tick
	void GameManager::playerActions()
	{
				 playerDirectionManager();
				 player->AllMoveActions();
				 player->LimitedMove();
				 calculatePlayerNewXYPos();
				 player->ForceDraw(playerNewX, playerNewY);
	}

			 //calculates which players positions to draw, for when viewport is in corner
	void GameManager::calculatePlayerNewXYPos()
	{
				 int playerXWorld = player->XPos;
				 int playerYWorld = player->YPos;
				 int halfVportX = (VPORT_TILES_WIDE  * TILE_SIDE) / 2;
				 int halfVportY = (VPORT_TILES_HIGH * TILE_SIDE) / 2;
				 int worldWidth = VPORT_TILES_WIDE * TILE_SIDE;
				 int worldHeight = VPORT_TILES_HIGH * TILE_SIDE;
				 //center it first
				 playerNewX = clientRect.Width / 2;
				 playerNewY = clientRect.Height / 2;
				 //change x and y if vport if frozen on either axis
				 if (playerXWorld < halfVportX || playerXWorld > worldWidth - halfVportX - player->FrameWidth)
				 {
					 playerNewX = playerXWorld - mainVPort->VPortWorldX;
				 }
				 if (playerYWorld < halfVportY || playerYWorld > worldHeight - halfVportY - player->FrameHeight)
				 {
					 playerNewY = playerYWorld - mainVPort->VPortWorldY;
				 }
	}

		//damages all enemies hit by bullet
	void GameManager::damageEnemies()
	{
				 int bulletCount = bullets->CountSprites();
				 for (int i = 0; i < bulletCount; i++)
				 {
					 Sprite^ currentBullet = bullets->AtIndex(i);
					 Sprite^ deadChook = enemies->CheckCollisions(currentBullet);
					 if (deadChook != nullptr)
					 {
						 currentBullet->IsAlive = false;
						 deadChook->Health -= PLAYER_BULLET_DAMAGE;
						 spriteHealthCheck(deadChook);
					 }
				 }
	}

	//damages players and enemies on collision
	void GameManager::playerEnemyCollision()
	{
		battleReadyTimer++;
		for (int i = 0; i < nTotalEnemies; i++)
		{
			Sprite^ currentEnemy = enemies->AtIndex(i);
			//only bother checking collisions if it's close
			if (playerNear(currentEnemy))
			{
				//if it actually collides
				if (currentEnemy->CollideWith(player))
				{
					//if it's actually active
					if (currentEnemy != nullptr && currentEnemy->IsAlive)
					{
						//checks battle timer so damage doesn't drop every tick (would die too quickly)
						if (battleReadyTimer >= DAMAGE_BATTLE_TIMER)
						{
							//apply contact damage
							int playerDamage = rand->Next(PLAYER_HURT_MIN, PLAYER_HURT_MAX + 1) + damageIncrease;
							int enemyDamage = rand->Next(ENEMY_HURT_MIN, ENEMY_HURT_MAX + 1);
							player->Health -= playerDamage;
							currentEnemy->Health -= enemyDamage;
							spriteHealthCheck(player);
							spriteHealthCheck(currentEnemy);
							battleReadyTimer = 0;
						}
					}
				}
			}
		}
	}

	//enemybullets vs player
	void GameManager::playerBulletCollision()
	{
		int enemyBulletCount = enemyBullets->CountSprites();
		for (int i = 0; i < enemyBulletCount; i++)
		{
			Sprite^ currentBullet = enemyBullets->AtIndex(i);
			//only bother checking collisions if it's close
			if (playerNear(currentBullet))
			{
				//if it actually collides
				if (currentBullet->CollideWith(player))
				{
					//if it's active
					if (currentBullet != nullptr && currentBullet->IsAlive)
					{
						//apply damage
						currentBullet->IsAlive = false;
						player->Health -= (ENEMY_BULLET_DAMAGE + damageIncrease);
						spriteHealthCheck(player);
					}
				}
			}
		}
	}

	//makes things dead if they're dead
	void GameManager::spriteHealthCheck(Sprite^ spriteToCheck)
	{
		if (spriteToCheck->Health < 1)
		{
			spriteToCheck->Health = 0;
			spriteToCheck->IsAlive = false;
			//if the dead sprite isn't the player (ha ha) increase score
			if (spriteToCheck != player)
			{
				PlayerScore += KILL_SCORE;
			}
			//if it is the player set mana to zero
			if (spriteToCheck == player)
			{
				player->Mana = 0;
			}
			spriteToCheck->SpriteDeath();
		}
	}

	//makes turret's bullets, they shoot in random directions
	void GameManager::makeEnemyBullet(Sprite^ currentTurret)
	{
		int startX = currentTurret->XPos + centeringBullet;
		int startY = currentTurret->YPos + centeringBullet;
		Sprite^ newBullet = gcnew Sprite(canvas, enemyBulletFiles, rand, nFrames, startX, startY, clientRect, DIE, blankImg, 10, 10, mainTileMap, bulletVel);
		newBullet->SetBoundsRectToTileMapEdges();
		newBullet->RandomiseDirection();
		enemyBullets->AddSprite(newBullet);
	}

	//amkes player bullet shooting in geiven direction - direction comes from form keyup
	void GameManager::MakeBullet(int startDirection)
	{
				 int startX = player->XPos + centeringBullet;
				 int startY = player->YPos + centeringBullet;
				 Sprite^ newBullet = gcnew Sprite(canvas, bulletFiles, rand, nFrames, startX, startY, clientRect, DIE, blankImg, 10, 10, mainTileMap, bulletVel);
				 newBullet->SetBoundsRectToTileMapEdges();
				 newBullet->SpriteDirection = startDirection;
				 bullets->AddSprite(newBullet);
	}

	//makes bullets in relation to available mana
	void GameManager::BulletManaManage(int startDirection)
	{
		if (player->Mana >= PLAYER_BULLET_MANA_COST)
		{
			MakeBullet(startDirection);
			player->Mana -= PLAYER_BULLET_MANA_COST;
			if (player->Mana < 0)
			{
				player->Mana = 0;
			}
		}
	}

	//updates values used to display player status on form
	void GameManager::updateDisplayValues()
	{
		//score managed by gamemanager
		PlayerHealth = player->Health;
		PlayerMana = player->Mana;
	}

	//everything a bullet does in a timer time. notice snightly different beahviour to other sprites
	void GameManager::bulletActions(SpriteList^ currentBulletList)
	{
		currentBulletList->KillOnWallAllSprites();
		currentBulletList->DeleteAllDeadSprites();
		currentBulletList->AllMoveActionsAllSprites();
		currentBulletList->MoveSprites();
		int vPortX = mainVPort->VPortWorldX;
		int vPortY = mainVPort->VPortWorldY;
		currentBulletList->DrawSpritesInViewPort(vPortX, vPortY);
	}

	//detects if a player is in a certain radious of sprite - to use before collision detection to hopefully save cpu cycles
	bool GameManager::playerNear(Sprite^ testSprite) 
	{
		bool playerNear = true;

		int playerLeft = player->XPos - (COLLISION_RADIUS_TILES_X * TILE_SIDE);
		int playerRight = player->XPos + (COLLISION_RADIUS_TILES_X * TILE_SIDE);
		int playerTop = player->YPos - (COLLISION_RADIUS_TILES_Y * TILE_SIDE);
		int playerBottom = player->YPos + (DETECTION_RADIUS_TILES_Y * TILE_SIDE);

		if (testSprite->XPos < playerLeft)
		{
			playerNear = false;
		}
		if (testSprite->XPos > playerRight)
		{
			playerNear = false;
		}
		if (testSprite->YPos < playerTop)
		{
			playerNear = false;
		}
		if (testSprite->YPos > playerBottom)
		{
			playerNear = false;
		}

		return playerNear;
	}

	//picks direction based on which keys are currently down/up 
	void GameManager::playerDirectionManager()
	{
				 if (LeftDown && UpDown)
				 {
					 player->SpriteDirection = NORTHWEST;
				 }
				 else
				 {
					 if (LeftDown && DownDown)
					 {
						 player->SpriteDirection = SOUTHWEST;
					 }
					 else
					 {
						 if (RightDown && UpDown)
						 {
							 player->SpriteDirection = NORTHEAST;
						 }
						 else
						 {
							 if (RightDown && DownDown)
							 {
								 player->SpriteDirection = SOUTHEAST;
							 }
							 else
							 {
								 if (UpDown)
								 {
									 player->SpriteDirection = NORTH;
								 }
								 else
								 {
									 if (RightDown)
									 {
										 player->SpriteDirection = EAST;
									 }
									 else
									 {
										 if (DownDown)
										 {
											 player->SpriteDirection = SOUTH;
										 }
										 else
										 {
											 if (LeftDown)
											 {
												 player->SpriteDirection = WEST;
											 }
											 else
											 {
												 player->SpriteDirection = STILL;
											 }
										 }
									 }
								 }
							 }
						 }
					 }
				 }
	}