#include "zone.h"
#include "room.h"
#include "colorstring.h"
#include "enemy.h"
#include "game.h"
#include "virtualwindow.h"
#include "artifact.h"
#include "utilities.h"
#include "menu.h"
#include "player.h"
#include "creature.h"
#include "coordinate.h"

#include <algorithm>

// Zone Environment

ZoneEnvironment::ZoneEnvironment(Game* theGame, ColorString zone, int modChance, int powChance, int mythChance)
{
	zoneString = zone;
	game = theGame;

	// chaces for getting the respective rarity of artifact in that floor
	moderateChance = modChance;
	powerfulChance = powChance;
	mythicalChance = mythChance;
}

ColorString ZoneEnvironment::getZoneString()
{
	return zoneString;
}

Artifact* ZoneEnvironment::getRandomArtifact()
{
	Artifact* newArtifact = nullptr;

	int rarityNum = ddutil::random(1, 100);

	if (rarityNum <= moderateChance)
	{
		newArtifact = getModArtifact();
	}
	else
	{
		if (rarityNum > 100 - mythicalChance)
		{
			newArtifact = getMythArtifact();
		}
		else
		{
			
			newArtifact = getPowArtifact();
		}
	}

	return newArtifact;
}

Artifact* ZoneEnvironment::getRandomArtifact(Strength str)
{
	switch (str)
	{
	case Strength::Moderate:
		return getModArtifact();
	case Strength::Powerful:
		return getPowArtifact();
	case Strength::Mythical:
		return getMythArtifact();
	default:
		return getRandomArtifact();
	}
}

// adds the boss artifact to the chosen players inventory
void ZoneEnvironment::chooseBossArtifact()
{
	std::vector<BossArtifact*> choices;
	const std::vector<int>& randomNums = ddutil::uniqueRandom(1, 6, ddutil::BOSS_ARTIFACT_CHOICES);
	for (int i : randomNums)
	{
		switch (i)
		{
		case 1:
			choices.push_back(new Tesseract(game));
			break;
		case 2:
			choices.push_back(new MagicAmulet(game));
			break;
		case 3:
			choices.push_back(new PossessedDoll(game));
			break;
		case 4:
			choices.push_back(new DarkSeed(game));
			break;
		case 5:
			choices.push_back(new MagicLeech(game));
			break;
		case 6:
			choices.push_back(new BattleStandard(game));
			break;
		}
	}
	if (choices.empty())
	{
		ddutil::errorMessage("BAD BOSS ARTIFACT GENERATION", __LINE__, __FILE__);
	}

	choices[0]->playFindSound();

	// generate a menu so the player can choose between the two options
	std::vector<ColorString> menuOptions = { ColorString("Skip", ddutil::TEXT_COLOR) };
	for (BossArtifact* a : choices)
	{
		menuOptions.push_back(a->getFullInformation());
	}
	int line = ddutil::DIVIDER_LINE3 / 2;
	game->getVWin()->putcen(ColorString("Choose Reward:", ddutil::ARTIFACT_COLOR), line - 1);
	game->displayInfo();

	Menu menu(game->getVWin(), menuOptions, Coordinate(0, line), true);
	if (menu.getResponse() != 0) // corresponds to one greater than index of the boss artifact the player chose
	{
		BossArtifact* chosenArtifact = choices[menu.getResponse() - 1];

		// remove the chosen artifact from choices so that it isnt deleted down further
		auto it = choices.begin();
		while (it != choices.end())
		{
			if ((*it) == chosenArtifact)
			{
				it = choices.erase(it);
			}
			else
			{
				++it;
			}
		}

		game->clearCenterScreen();
		game->artifactSelectionMenu(line - 1, chosenArtifact);
		game->displayInfo();
	}

	for (unsigned int i = 0; i < choices.size(); i++)
	{
		delete choices[i];
	}

	game->clearCenterScreen();
}

ModerateArtifact* ZoneEnvironment::getModArtifact()
{
	game->changeScore(ddutil::MOD_ARTIFACT_SCORE);
	ModerateArtifact* newArtifact = nullptr;
	switch (ddutil::random(1, 6))
	{
	case 1:
		newArtifact = new PurpleAmulet(game);
		break;
	case 2:
		newArtifact = new SwiftnessBracelet(game);
		break;
	case 3:
		newArtifact = new MagicBean(game);
		break;
	case 4:
		newArtifact = new MagicPowder(game);
		break;
	case 5:
		newArtifact = new AncientBook(game);
		break;
	case 6:
		newArtifact = new Chainmail(game);
		break;
	}
	newArtifact->playFindSound();
	return newArtifact;
}

PowerfulArtifact* ZoneEnvironment::getPowArtifact()
{
	game->changeScore(ddutil::POW_ARTIFACT_SCORE);
	PowerfulArtifact* newArtifact = nullptr;
	switch (ddutil::random(1, 10))
	{
	case 1:
		newArtifact = new SpectralAmulet(game);
		break;
	case 2:
		newArtifact = new SacrificialKnife(game);
		break;
	case 3:
		newArtifact = new BurningOrb(game);
		break;
	case 4:
		newArtifact = new FrozenOrb(game);
		break;
	case 5:
		newArtifact = new MagicPotion(game);
		break;
	case 6:
		newArtifact = new AncientTome(game);
		break;
	case 7:
		newArtifact = new WarriorHelm(game);
		break;
	case 8:
		newArtifact = new Shuriken(game);
		break;
	case 9:
		newArtifact = new PotentPowder(game);
		break;
	case 10:
		newArtifact = new ElectricOrb(game);
		break;
	}
	newArtifact->playFindSound();
	return newArtifact;
}

MythicalArtifact* ZoneEnvironment::getMythArtifact()
{
	game->changeScore(ddutil::MYTH_ARTIFACT_SCORE);
	MythicalArtifact* newArtifact = nullptr;
	switch (ddutil::random(1, 8))
	{
	case 1:
		newArtifact = new PoisonVial(game);
		break;
	case 2:
		newArtifact = new Katana(game);
		break;
	case 3:
		newArtifact = new AncientWand(game);
		break;
	case 4:
		newArtifact = new EternalSeed(game);
		break;
	case 5:
		newArtifact = new Spurs(game);
		break;
	case 6:
		newArtifact = new AncientScroll(game);
		break;
	case 7:
		newArtifact = new AdamantiteShield(game);
		break;
	case 8:
		newArtifact = new VitalStone(game);
		break;
	}
	newArtifact->playFindSound();
	return newArtifact;
}



// Catacombs (1)

CatacombsEnvironment::CatacombsEnvironment(Game* theGame)
	:ZoneEnvironment(
		theGame,
		ColorString("The Catacombs", ddutil::BROWN),
		ddutil::CATACOMBS_MOD_CHANCE,
		ddutil::CATACOMBS_POW_CHANCE,
		ddutil::CATACOMBS_MYTH_CHANCE
		)
{
}

ZoneMap CatacombsEnvironment::generateRooms()
{
	int numRows = 8;
	int numCols = 11;
	int numPaths = 3;
	ZoneMap map(numPaths, numRows, numCols);

	int middleCol = numCols / 2;
	for (Coordinate c : map.getRoomCoords())
	{
		int row = c.y;
		int col = c.x;
		if (col == numCols - 1) // last room is always boss
		{
			map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Boss));
		}
		else if (col == numCols - 2) // second to last room is always camp fire
		{
			map[row][col] = new FireRoom(game);
		}
		else if (col < numCols - 2 && col > middleCol) // second half of the map
		{
			int randomNum = ddutil::random(1, 100);
			if (randomNum <= 45) // 45% chance to spawn a strong enemy
			{
				map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Strong));
			}
			else if (randomNum <= 60) // 15% chance to spawn an event room
			{
				map[row][col] = generateEventRoom();
			}
			else if (randomNum <= 95) // 35% chance to spawn a normal enemy
			{
				map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Normal));
			}
			else // 5% chance to spawn a campfire
			{
				map[row][col] = new FireRoom(game);
			}
		}
		else if (col == middleCol) // middle room always spawns campfire
		{
			map[row][col] = new FireRoom(game);
		}
		else if (col < middleCol && col > 1) // if not first two rooms but still not middle
		{
			int randomNum = ddutil::random(1, 100);
			if (randomNum <= 40) // 40% chance for normal enemy
			{
				map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Normal));
			}
			else if (randomNum <= 70) // 30% chance for strong
			{
				map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Strong));
			}
			else if (randomNum <= 95) // 25% chance for event
			{
				map[row][col] = generateEventRoom();
			}
			else // 5% chance for fire
			{
				map[row][col] = new FireRoom(game);
			}
		}
		else // first two rooms
		{
			map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Normal));
		}
	}

	return map;
}

Enemy* CatacombsEnvironment::generateEnemy(ddutil::EnemyType type)
{
	const int NUM_NORM_ENEMIES = 8;
	const int NUM_STR_ENEMIES = 3;
	static int normCount = ddutil::random(1, NUM_NORM_ENEMIES);
	static int strCount = ddutil::random(1, NUM_STR_ENEMIES);
	switch (type)
	{
	case ddutil::EnemyType::Normal:
		normCount++;
		if (normCount > NUM_NORM_ENEMIES)
		{
			normCount = 1;
		}
		switch (normCount)
		{
		case 1:
			return new GiantSnail(game);
		case 2:
			return new GiantSnailVariant(game);
		case 3:
			return new Ghost(game);
		case 4:
			return new GhostVariant(game);
		case 5:
			return new VampireBat(game);
		case 6:
			return new VampireBatVariant(game);
		case 7:
			return new CaveBat(game);
		case 8:
			return new CaveBatVariant(game);
		}
	case ddutil::EnemyType::Strong:
		strCount++;
		if (strCount > NUM_STR_ENEMIES)
		{
			strCount = 1;
		}
		if (strCount == 1)
			return new Brute(game);
		else if (strCount == 2)
			return new Minotaur(game);
		else
			return new AncientLizard(game);
	case ddutil::EnemyType::Boss:
		return new TheMessenger(game);
	}

	return nullptr;
}

Room* CatacombsEnvironment::generateEventRoom()
{
	int randomNum = ddutil::random(1, 4);
	static int uniqueCounter = ddutil::random(1, 5);
	// 50% unique event
	// 25% - normal enemy
	// 25% generic event
	if (randomNum <= 2) // unique event
	{
		uniqueCounter++;
		if (uniqueCounter >= 6)
		{
			uniqueCounter = 1;
		}
		if (uniqueCounter == 1)
		{
			return new BrokenMirrorEvent(game);
		}
		else if (uniqueCounter == 2)
		{
			return new MysteriousKnightEvent(game);
		}
		else if (uniqueCounter == 3)
		{
			return new BloodAltarEvent(game);
		}
		else if (uniqueCounter == 4)
		{
			return new SpiderEvent(game);
		}
		else
		{
			return new DynamiteEvent(game);
		}
	}
	else if (randomNum == 3) // enemy
	{
		return new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Normal));
	}
	else // generic event
	{
		if (ddutil::random(1,2) == 1)
			return new DeadAdventurerEvent(game);
		else
			return new TreasureEvent(game);
	}
}

// Abyss (2)

AbyssEnvironment::AbyssEnvironment(Game* theGame)
	:ZoneEnvironment(
		theGame,
		ColorString("The Abyss", ddutil::RED),
		ddutil::ABYSS_MOD_CHANCE,
		ddutil::ABYSS_POW_CHANCE,
		ddutil::ABYSS_MYTH_CHANCE
	)
{
}

ZoneMap AbyssEnvironment::generateRooms()
{
	int numPaths = 6;
	int numRows = 7;
	int numCols = 11;
	ZoneMap map(numPaths, numRows, numCols);

	int middleCol = numCols / 2;
	for (Coordinate c : map.getRoomCoords())
	{
		int row = c.y;
		int col = c.x;
		if (col == numCols - 1) // last room is always boss
		{
			map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Boss));
		}
		else if (col == numCols - 2) // second to last room is always camp fire
		{
			map[row][col] = new FireRoom(game);
		}
		else if (col < numCols - 2 && col > middleCol) // second half of the map 
		{
			int randomNum = ddutil::random(1, 100);
			if (randomNum <= 40) // 40% chance to spawn a strong enemy
			{
				map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Strong));
			}
			else if (randomNum <= 70) // 30% chance to spawn an event room
			{
				map[row][col] = generateEventRoom();
			}
			else if (randomNum <= 95) // 25% chance to spawn a normal enemy
			{
				map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Normal));
			}
			else // 5% chance to spawn a campfire
			{
				map[row][col] = new FireRoom(game);
			}
		}
		else if (col == middleCol) // middle room always spawns campfire
		{
			map[row][col] = new FireRoom(game);
		}
		else if (col < middleCol && col > 0) // if not first room but still not middle
		{
			int randomNum = ddutil::random(1, 100);
			if (randomNum <= 45) // 45% chance for normal enemy
			{
				map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Normal));
			}
			else if (randomNum <= 70) // 25% chance for strong
			{
				map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Strong));
			}
			else if (randomNum <= 95) // 25% chance for event
			{
				map[row][col] = generateEventRoom();
			}
			else // 5% chance for fire
			{
				map[row][col] = new FireRoom(game);
			}
		}
		else // first room
		{
			map[row][col] = new FireRoom(game);
		}
	}

	return map;
}

Enemy* AbyssEnvironment::generateEnemy(ddutil::EnemyType type)
{
	const int NUM_NORM_ENEMIES = 7;
	const int NUM_STR_ENEMIES = 2;
	static int strCounter = ddutil::random(1, NUM_STR_ENEMIES);
	static int normCounter = ddutil::random(1, NUM_NORM_ENEMIES);

	if (type == ddutil::EnemyType::Boss)
	{
		return new TheCollector(game);
	}
	else if (type == ddutil::EnemyType::Strong)
	{
		strCounter++;
		if (strCounter > NUM_STR_ENEMIES)
		{
			strCounter = 1;
		}
		if (strCounter == 1)
			return new Basilisk(game);
		else
			return new AbyssBeast(game);
	}
	else // Normal
	{
		normCounter++;
		if (normCounter > NUM_NORM_ENEMIES)
		{
			normCounter = 1;
		}
		switch (normCounter)
		{
		case 1:
			return new Minion(game);
		case 2:
			return new MinionAlt(game);
		case 3:
			return new GiantLizard(game);
		case 4:
			return new AltGiantLizard(game);
		case 5:
			return new FireBat(game);
		case 6:
			return new FireBatVariant(game);
		case 7:
			return new FirePlatypus(game);
		}
	}
	ddutil::errorMessage("INVALID ENEMY GENERATION IN ABYSS", __LINE__, __FILE__);
	return nullptr;
}

Room* AbyssEnvironment::generateEventRoom()
{
	static int uniqueCounter = ddutil::random(1, 3);
	int randomNum = ddutil::random(1, 4);

	if (randomNum <= 2) // unique event
	{
		uniqueCounter++;
		if (uniqueCounter > 3)
		{
			uniqueCounter = 1;
		}
		if (uniqueCounter == 1)
		{
			return new LavaBeastEvent(game);
		}
		else if (uniqueCounter == 2)
		{
			return new GoblinEvent(game);
		}
		else
		{
			return new MaskEvent(game);
		}
	}
	else if (randomNum == 3) // enemy fight
	{
		return new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Normal));
	}
	else // generic Event
	{
		return new TreasureEvent(game);
	}
}

// Void (3)

VoidEnvironment::VoidEnvironment(Game* theGame)
	:ZoneEnvironment(
		theGame, 
		ColorString("The Void", ddutil::MAGENTA),
		ddutil::VOID_MOD_CHANCE,
		ddutil::VOID_POW_CHANCE,
		ddutil::VOID_MYTH_CHANCE
	)
{
}

ZoneMap VoidEnvironment::generateRooms()
{
	int numPaths = 2;
	int numRows = 4;
	int numCols = 11;
	ZoneMap map(numPaths, numRows, numCols);

	int middleCol = numCols / 2;
	for (Coordinate c : map.getRoomCoords())
	{
		int row = c.y;
		int col = c.x;
		if (col == numCols - 1) // last room is always boss
		{
			map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Boss));
		}
		else if (col == numCols - 2) // second to last room is always camp fire
		{
			map[row][col] = new FireRoom(game);
		}
		else if (col < numCols - 2 && col > middleCol) // second half of the map 
		{
			int randomNum = ddutil::random(1, 100);
			if (randomNum <= 40) // 40% chance to spawn a strong enemy
			{
				map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Strong));
			}
			else if (randomNum <= 70) // 30% chance to spawn an event room
			{
				map[row][col] = generateEventRoom();
			}
			else // 30% chance to spawn a normal enemy
			{
				map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Normal));
			}
		}
		else if (col == middleCol) // middle room always spawns campfire
		{
			map[row][col] = new FireRoom(game);
		}
		else if (col < middleCol && col > 0) // if not first room but still not middle
		{
			int randomNum = ddutil::random(1, 100);
			if (randomNum <= 45) // 45% chance for normal enemy
			{
				map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Normal));
			}
			else if (randomNum <= 75) // 30% chance for strong
			{
				map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Strong));
			}
			else // 25% chance for event
			{
				map[row][col] = generateEventRoom();
			}
		}
		else // first room
		{
			map[row][col] = new FireRoom(game);
		}
	}

	return map;
}

Enemy* VoidEnvironment::generateEnemy(ddutil::EnemyType type)
{
	const int NUM_NORM_ENCOUNTERS = 6;
	const int NUM_STR_ENCOUNTERS = 3;
	static int normCounter = ddutil::random(1, NUM_NORM_ENCOUNTERS);
	static int strCounter = ddutil::random(1, NUM_STR_ENCOUNTERS);

	if (type == ddutil::EnemyType::Boss)
	{
		return new TheProtector(game);
	}
	else if (type == ddutil::EnemyType::Strong)
	{
		strCounter++;
		if (strCounter > NUM_STR_ENCOUNTERS)
		{
			strCounter = 1;
		}
		if (strCounter == 1)
		{
			return new HyperBeast(game);
		}
		else if (strCounter == 2)
		{
			return new AncientBird(game);
		}
		else
		{
			return new CorruptedDisciple(game);
		}
	}
	else // normal
	{
		normCounter++;
		if (normCounter > NUM_NORM_ENCOUNTERS)
		{
			normCounter = 1;
		}
		switch (normCounter)
		{
		case 1:
			return new LaughingMask(game);
		case 2:
			return new DevilishMask(game);
		case 3:
			return new SentientMouth(game);
		case 4:
			return new PossessedMace(game);
		case 5:
			return new Sniffer(game);
		case 6:
			return new SnifferVariant(game);
		}
	}
	ddutil::errorMessage("INVALID ENEMY GENERATION IN VOID", __LINE__, __FILE__);
	return nullptr;
}

Room* VoidEnvironment::generateEventRoom()
{
	static int uniqueCounter = ddutil::random(1, 2);

	int randomNum = ddutil::random(1, 5);
	if (randomNum == 1) // unique event 20%
	{
		uniqueCounter++;
		if (uniqueCounter > 2)
		{
			uniqueCounter = 1;
		}
		if (uniqueCounter == 1)
		{
			return new MirrorEvent(game);
		}
		else
		{
			return new PortalEvent(game);
		}
	}
	else if (randomNum == 2 || randomNum == 3) // enemy fight 40%
	{
		return new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Normal));
	}
	else // generic Event 40%
	{
		return new TreasureEvent(game);
	}
}

// Palace (final)

PalaceEnvironment::PalaceEnvironment(Game* theGame)
	:ZoneEnvironment(
		theGame,
		ColorString("The Palace", ddutil::YELLOW),
		ddutil::PALACE_MOD_CHANCE,
		ddutil::PALACE_POW_CHANCE,
		ddutil::PALACE_MYTH_CHANCE
	)
{
}

ZoneMap PalaceEnvironment::generateRooms()
{
	int numPaths = 1;
	int numRows = 1;
	int numCols = 2;
	ZoneMap map(numPaths, numRows, numCols);

	int middleCol = numCols / 2;
	for (Coordinate c : map.getRoomCoords())
	{
		int row = c.y;
		int col = c.x;
		if (col == numCols - 1) // last room is always boss
		{
			map[row][col] = new EnemyRoom(game, generateEnemy(ddutil::EnemyType::Boss));
		}
		else if (col == numCols - 2) // second to last room is always camp fire
		{
			map[row][col] = new FireRoom(game);
		}
		
	}

	return map;
}

Enemy* PalaceEnvironment::generateEnemy(ddutil::EnemyType type)
{
	return new TruePatriarch(game);
}

Room* PalaceEnvironment::generateEventRoom()
{
	return new TreasureEvent(game);
}


// Zone

Zone::Zone(Game* theGame, int num)
{
	zoneNumber = num;
	game = theGame;

	if (num == 1)
	{
		environment = new CatacombsEnvironment(game);
	}
	else if (num == 2)
	{
		environment = new AbyssEnvironment(game);
	}
	else if (num == 3)
	{
		environment = new VoidEnvironment(game);
	}
	else // num == 4
	{
		environment = new PalaceEnvironment(game);
	}

	map = environment->generateRooms();
	currentCol = -1;
	currentRow = -1;
}

Zone::~Zone()
{
	delete environment;
	for (unsigned int i = 0; i < map.size(); i++)
	{
		for (unsigned int j = 0; j < map[i].size(); j++)
		{
			delete map[i][j];
		}
	}
}

int Zone::getZoneNumber()
{
	return zoneNumber;
}

ColorString Zone::getZoneString()
{
	return environment->getZoneString();
}

ZoneMap Zone::getMap()
{
	return map;
}

Artifact* Zone::getRandomArtifact() const
{
	return environment->getRandomArtifact();
}

Artifact* Zone::getRandomArtifact(Strength str) const
{
	return environment->getRandomArtifact(str);
}

void Zone::chooseBossArtifact() const
{
	environment->chooseBossArtifact();
}

bool Zone::hasMoreRooms()
{
	return currentCol < static_cast<int>(map[0].size() - 1);
}

Room* Zone::chooseRoom()
{
	int scale = 3;
	Coordinate origin(((ddutil::CONSOLEX - (map[0].size() * scale)) / 2), 3);

	VirtualWindow* vwin = game->getVWin();

	// displaying the map
	vwin->putcen(getZoneString(), origin.y - 2);

	for (Coordinate c : map.getRoomCoords())
	{
		vwin->put(map[c.y][c.x]->getMapChar(), origin + Coordinate(c.x * scale, c.y * scale));
		bool directlyRight = ddutil::hasCoords(Coordinate(c.x + 1, c.y), map.getRoomCoords());
		bool downRight = ddutil::hasCoords(Coordinate(c.x + 1, c.y + 1), map.getRoomCoords());
		bool upRight = ddutil::hasCoords(Coordinate(c.x + 1, c.y - 1), map.getRoomCoords());
		
		int zoneColor = getZoneString()[0].color;

		if (directlyRight) // room directly to right
		{
			for (int i = 1; i < scale; i++)
			{
				vwin->put(ColorChar('-', zoneColor), origin + Coordinate(c.x * scale + i, c.y * scale));
			}
		}
		if (downRight) // room is down and right
		{
			for (int i = 1; i < scale; i++)
			{
				vwin->put(ColorChar('\\', zoneColor), origin + Coordinate(c.x * scale + i, c.y * scale + i));
			}
		}
		if (upRight) // room is up and right
		{
			for (int i = 1; i < scale; i++)
			{
				vwin->put(ColorChar('/', zoneColor), origin + Coordinate(c.x * scale + i, c.y * scale - i));
			}	
		}
	}
	ColorString controls1 = ColorString("E = Normal Enemy; S = Strong Enemy; B = Boss Enemy; ", ddutil::CYAN);
	ColorString controls2 = ColorString("~ = Campfire; ", ddutil::FIRE_COLOR);
	ColorString controls3 = ColorString("? = Random Event", ddutil::EVENT_COLOR);

	game->getVWin()->putcen(controls1 + controls2 + controls3, ddutil::DIVIDER_LINE3 - 1);
	game->displayInfo();

	currentCol++;

	// Figuring out what coordinates are valid
	std::vector<int> allowedNewRows; // vector that contains the current row +- 1
	if (currentCol == 0) // first room
	{
		// let the player select any row
		for (Coordinate c : map.getRoomCoords())
		{
			if (c.x == 0)
			{
				allowedNewRows.push_back(c.y);
			}
		}
	}
	else // only let the player select rows +-1 from their current row
	{
		for (int i = currentRow - 1; i <= currentRow + 1; i++)
		{
			if (ddutil::hasCoords(Coordinate(currentCol, i), map.getRoomCoords()))
			{
				allowedNewRows.push_back(i);
			}
		}
	}

	// list of all the coords that the player can move to
	std::vector<Coordinate> validCoords;
	for (int row : allowedNewRows)
	{
		validCoords.push_back(Coordinate(currentCol, row));
	}

	// sort so that pressing up actually makes the selection go up
	std::sort(validCoords.begin(), validCoords.end());
	const int SLEEP_TIME = 220;
	int currentValidCoordIndex = 0;
	bool keepGoing = true;
	while (keepGoing)
	{
		Coordinate currentCoord(validCoords[currentValidCoordIndex]);
		// adjust for the spread out display of the map
		currentCoord = origin + Coordinate(currentCoord.x * scale, currentCoord.y * scale);
		ColorChar original = vwin->highlight(currentCoord, ddutil::MAP_HIGHLIGHT_COLOR);
		while (true)
		{
			if (ddutil::keypress(VK_DOWN))
			{
				playSound(WavFile("menumove", false, true));
				currentValidCoordIndex++;
				if (currentValidCoordIndex >= static_cast<int>(validCoords.size()))
				{
					currentValidCoordIndex = 0;
				}
				Sleep(SLEEP_TIME);
				break;
			}
			else if (ddutil::keypress(VK_UP))
			{
				playSound(WavFile("menumove", false, true));
				currentValidCoordIndex--;
				if (currentValidCoordIndex < 0)
				{
					currentValidCoordIndex = validCoords.size() - 1;
				}
				Sleep(SLEEP_TIME);
				break;
			}
			else if (ddutil::keypress(VK_RETURN))
			{
				playSound(WavFile("menuselect", false, true));
				keepGoing = false;
				Sleep(SLEEP_TIME);
				break;
			}
		}
		game->getVWin()->put(original, currentCoord);
	}

	Coordinate finalCoord(validCoords[currentValidCoordIndex]);
	currentRow = finalCoord.y;
	game->clearCenterScreen();
	return map[finalCoord.y][finalCoord.x];
}

// makes the 2d vector the right size and adds coordinates to rooms which should be filled in by the environment generation function
ZoneMap::ZoneMap(int numPaths, int numRows, int numCols)
{
	for (int i = 0; i < numRows; i++)
	{
		push_back(std::vector<Room*>(numCols, nullptr));
	}

	// walk random paths through the map from right (boss room) to left
	for (int path = 0; path < numPaths; path++)
	{
		// start at the boss room
		int row = numRows / 2;

		for (int col = numCols - 1; col >= 0; col--)
		{
			if (!ddutil::hasCoords(Coordinate(col, row), roomCoords))
			{
				roomCoords.push_back(Coordinate(col, row));
			}
			// either move up one row, down one row, or stay on the row
			row += ddutil::random(-1, 1);

			// make sure we didn't leave the map
			if (row < 0)
			{
				row = 0;
			}
			else if (row >= numRows)
			{
				row = numRows - 1;
			}
		}
	}
}

int ZoneMap::getNumRows()
{
	return size();
}

int ZoneMap::getNumCols()
{
	if (empty())
	{
		return 0;
	}
	return this[0].size();
}

std::vector<Coordinate> ZoneMap::getRoomCoords()
{
	return roomCoords;
}
