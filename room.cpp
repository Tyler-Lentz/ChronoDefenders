#include "room.h"
#include "game.h"
#include "enemy.h"
#include "virtualwindow.h"
#include "menu.h"
#include "player.h"
#include "creature.h"
#include "artifact.h"
#include "zone.h"
#include "coordinate.h"
#include <string>
// Room

Room::Room(Game* theGame, ColorChar theMapChar)
{
	game = theGame;
	mapChar = theMapChar;
}

// EnemyRoom

EnemyRoom::EnemyRoom(Game* theGame, Enemy* theEnemy)
	:Room(theGame, ColorChar(theEnemy->getMapChar(), theEnemy->getColor()))
{
	enemy = theEnemy;
}

EnemyRoom::~EnemyRoom()
{
	delete enemy;
}

void EnemyRoom::playRoom()
{
	enemy->distortionUpdate(game->getCurrentDistortion());
	game->battle(enemy);
	if (!game->isGameOver() && !game->getGameWin())
	{
		for (Player* p : game->getPlayerParty())
		{
			p->tradeExperience();
		}
	}
}

int EnemyRoom::getRoomId()
{
	return enemy->getRoomId();
}

// Fire Room

FireRoom::FireRoom(Game* theGame)
	:Room(theGame, ColorChar(ddutil::MAP_FIRE, ddutil::FIRE_COLOR))
{

}

void FireRoom::playRoom()
{
	game->clearCenterScreen();
	VirtualWindow* vwin = game->getVWin();
	
	int campfireBottom = ddutil::EVENT_PICTURE_LINE;
	vwin->printArtFromBottom(Art::getCampfire(), Coordinate(0, campfireBottom), true);

	int menuTop = campfireBottom + 3;
	vwin->putcenSlowScroll(ColorString("You come across a small campfire", ddutil::TEXT_COLOR), menuTop - 2);

	std::vector<ColorString> options = {
		ColorString("Tend to the fire: All characters heal " + std::to_string(FIRE_HEAL) + " HP", ddutil::HEAL_COLOR),
		ColorString("Search for items: Find one Artifact", ddutil::ARTIFACT_COLOR)
	};

	Menu menu(vwin, options, Coordinate(0, menuTop), true);

	ColorString output;
	switch (menu.getResponse())
	{
	case 0:
		output = game->healAllPlayers(FIRE_HEAL);
		break;
	case 1:
		for (int i = menuTop - 2; i <= menuTop + 2; i++)
		{
			vwin->clearLine(i);
		}
		game->artifactSelectionMenu(menuTop - 1, game->getActiveZone()->getRandomArtifact());
		break;
	}

	game->clearCenterScreen();
	vwin->putcen(output, ddutil::CENTER_TEXT_LINE);
	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, ddutil::CENTER_TEXT_LINE + 1), true);
	game->clearCenterScreen();
}

int FireRoom::getRoomId()
{
	return RoomId::Fire;
}

EventRoom::EventRoom(Game* game)
	:Room(game, ColorChar(ddutil::MAP_EVENT, ddutil::EVENT_COLOR))
{

}

DeadAdventurerEvent::DeadAdventurerEvent(Game* game)
	:EventRoom(game)
{
}

void DeadAdventurerEvent::playRoom()
{
	VirtualWindow* vwin = game->getVWin();

	game->clearCenterScreen();

	Coordinate skullBottom(0, ddutil::EVENT_PICTURE_LINE);
	vwin->printArtFromBottom(Art::getDeadAdventurer(), skullBottom, true);
	
	int line = skullBottom.y + 2;
	vwin->putcenSlowScroll(ColorString("You come across a dead adventurer. They were not as fortunate as you.", ddutil::TEXT_COLOR), line++);
	
	std::vector<ColorString> options = {
		ColorString("Study the wounds: everybody gains "+std::to_string(EXPERIENCE_GAIN)+" experience", ddutil::EXPERIENCE_COLOR),
		ColorString("Loot the corpse: find one artifact", ddutil::ARTIFACT_COLOR),
		ColorString("Stop and rest: everybody gains "+std::to_string(HEALTH_GAIN)+" health", ddutil::HEAL_COLOR)
	};

	Menu menu(vwin, options, Coordinate(0, line), true);

	ColorString output;
	if (menu.getResponse() == 0) // gain experience
	{
		for (Player* p : game->getPlayerParty())
		{
			p->gainExperience(EXPERIENCE_GAIN);
		}
		output += ColorString("Everybody gains " + std::to_string(EXPERIENCE_GAIN) + " experience", ddutil::EXPERIENCE_COLOR);
	}
	else if (menu.getResponse() == 1) // find artifact
	{
		for (int i = line - 1; i <= line + 2; i++)
		{
			vwin->clearLine(i);
		}
		game->artifactSelectionMenu(line - 1, game->getActiveZone()->getRandomArtifact());
	}
	else // heal
	{
		for (Player* p : game->getPlayerParty())
		{
			int actualIncrease = p->increaseHealth(HEALTH_GAIN);
			output += p->getColorString() + ColorString(": ", ddutil::TEXT_COLOR) +
				ColorString("+" + std::to_string(actualIncrease) + " health; ", ddutil::HEAL_COLOR);
		}
	}

	game->clearCenterScreen();
	game->clearBottomDivider();
	game->displayInfo();

	vwin->putcen(output, ddutil::CENTER_TEXT_LINE);
	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, ddutil::CENTER_TEXT_LINE + 1), true);

	game->clearCenterScreen();
}

int DeadAdventurerEvent::getRoomId()
{
	return RoomId::DeadAdventurer;
}

BrokenMirrorEvent::BrokenMirrorEvent(Game* game)
	:EventRoom(game)
{
}

void BrokenMirrorEvent::playRoom()
{
	VirtualWindow* vwin = game->getVWin();

	game->clearCenterScreen();

	int line = ddutil::EVENT_PICTURE_LINE;

	vwin->printArtFromBottom(Art::getBrokenMirror(), Coordinate(0, line), true);

	line += 2;

	RustyDagger* dagger = new RustyDagger(game);

	vwin->putcenSlowScroll(ColorString("You come across a broken mirror. On the ground lies a ", ddutil::TEXT_COLOR) +
		dagger->getName(), line++);

	std::vector<ColorString> options = {
		ColorString("Take the ", ddutil::TEXT_COLOR) + dagger->getFullInformation(),
		ColorString("Leave it", ddutil::TEXT_COLOR)
	};

	Menu menu(vwin, options, Coordinate(0, line), true);

	vwin->clearLine(line - 1);
	vwin->clearLine(line);
	vwin->clearLine(line + 1);
	line--;

	if (menu.getResponse() == 0) // take the dagger
	{
		game->artifactSelectionMenu(line, dagger);

	}
	else // leave the dagger
	{
		delete dagger;
		vwin->putcen(ColorString("You choose to leave the dagger", ddutil::TEXT_COLOR), line++);
		Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, line), true);
	}

	game->clearCenterScreen();
}

int BrokenMirrorEvent::getRoomId()
{
	return RoomId::BrokenMirror;
}

MysteriousKnightEvent::MysteriousKnightEvent(Game* game)
	:EventRoom(game)
{
}

void MysteriousKnightEvent::playRoom()
{
	VirtualWindow* vwin = game->getVWin();

	game->clearCenterScreen();

	int line = ddutil::EVENT_PICTURE_LINE;

	vwin->printArtFromBottom(Art::getMysteriousKnight(), Coordinate(0, line), true);

	line += 2;
	vwin->putcenSlowScroll(ColorString("A ", ddutil::TEXT_COLOR) + ColorString("Mysterious Knight", Art::MYSTERIOUS_KNIGHT_COLOR) +
		ColorString(" is blocking your way", ddutil::TEXT_COLOR), line++);

	std::vector<ColorString> options = {
		ColorString("Challenge the Knight", ddutil::DAMAGE_COLOR),
		ColorString("Bargain with the Knight", ddutil::ARTIFACT_COLOR)
	};
	Menu menu(vwin, options, Coordinate(0, line), true);

	bool fight = false;
	if (menu.getResponse() == 1)
	{
		for (int i = line - 1; i <= line + 2; i++)
		{
			vwin->clearLine(i);
		}
		vwin->putcenSlowScroll(ColorString("\"I require a blood sacrifice. Who offers half their current HP?\"", Art::MYSTERIOUS_KNIGHT_COLOR), line - 1);

		std::vector<ColorString> options2;

		options2.push_back(ColorString("Attack the ", ddutil::TEXT_COLOR) +
			ColorString("Mysterious Knight", Art::MYSTERIOUS_KNIGHT_COLOR));
		for (Player* p : game->getPlayerParty())
		{
			options2.push_back(p->getColorString());
		}

		Menu menu2(vwin, options2, Coordinate(0, line), true);

		if (menu2.getResponse() == 0)
		{
			fight = true;
		}
		else
		{
			Player* chosenPlayer = game->getPlayerParty()[menu2.getResponse() - 1];
			ddutil::DamageReport damRep = chosenPlayer->reduceHealth(chosenPlayer->getHealth() / 2, chosenPlayer, true);
			for (int i = line - 1; i <= line + 3; i++)
			{
				vwin->clearLine(i);
			}
			vwin->putcen(ColorString("The ", ddutil::TEXT_COLOR) + chosenPlayer->getColorString() + ColorString(" loses ", ddutil::TEXT_COLOR) +
				ColorString(std::to_string(damRep.getDamageTaken()) + " health!", ddutil::DAMAGE_COLOR), ddutil::CENTER_TEXT_LINE);
			Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, ddutil::CENTER_TEXT_LINE + 1), true);
		}

	}
	if (menu.getResponse() == 0 || fight)
	{
		EnemyRoom temp(game, new MysteriousKnight(game));
		temp.playRoom();
	}

	game->clearBottomDivider();
	game->clearCenterScreen();

}

int MysteriousKnightEvent::getRoomId()
{
	return RoomId::MysteriousKnightEventFight;
}

DynamiteEvent::DynamiteEvent(Game* game)
	:EventRoom(game)
{
}

void DynamiteEvent::playRoom()
{
	VirtualWindow* vwin = game->getVWin();

	game->clearCenterScreen();

	int line = ddutil::EVENT_PICTURE_LINE;

	vwin->printArtFromBottom(Art::getDynamite(), Coordinate(0, line), true);


	playSound(Mp3File("bombfusemp3"));

	line += 2;
	vwin->putcenSlowScroll(ColorString("The room is rigged with ",ddutil::TEXT_COLOR) + ColorString("dynamite!", Dynamite::COLOR), line++);

	std::vector<ColorString> options = {
		ColorString("Attempt to put out the fuse [50% lose "+std::to_string(DAMAGE)+"HP; 50% gain "+std::to_string(EXPERIENCE_GAIN)+"XP]", ddutil::DAMAGE_COLOR),
		ColorString("Run away", ddutil::HEAL_COLOR)
	};

	Menu menu(vwin, options, Coordinate(0, line), true);

	if (menu.getResponse() == 0)
	{
		int randomNum = ddutil::random(1, 2);
		if (randomNum == 1)
		{
			stopSound(SoundType::MP3);
			game->clearCenterScreen();
			game->clearBottomDivider();
			playSound(WavFile("explosion", false, false));
			ColorString output;
			for (Player* p : game->getPlayerParty())
			{
				output += (p->getColorString() + ColorString(": ", ddutil::TEXT_COLOR) + 
					ColorString(std::to_string(p->reduceHealth(DAMAGE, nullptr).getDamageTaken()) + " damage; ",ddutil::DAMAGE_COLOR));
			}
			vwin->putcen(output, ddutil::CENTER_TEXT_LINE);

			Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, ddutil::CENTER_TEXT_LINE + 1), true); 

			game->clearCenterScreen();
			game->clearBottomDivider();

			if (game->checkForPlayerDeaths())
			{
				return;
			}
		}
		else
		{
			stopSound(SoundType::MP3);
			game->clearCenterScreen();
			game->clearBottomDivider();
			game->playersGetExperience(EXPERIENCE_GAIN);
			game->clearCenterScreen();
			game->clearBottomDivider();
		}
	}
	else
	{
		game->clearCenterScreen();
		for (int i = 0; i < 6; i++)
		{
			playSound(WavFile("footsteps", false, false));
		}
		stopSound(SoundType::MP3);
		playSound(WavFile("explosion", false, false));
	}

	
}

int DynamiteEvent::getRoomId()
{
	return RoomId::Dynamite;
}

ColorChar Room::getMapChar()
{
	return mapChar;
}

void Room::setCharEmpty()
{
	mapChar = ColorChar('*', ddutil::LIGHTGRAY);
}

void Room::setChar(ColorChar cchar)
{
	mapChar = cchar;
}

TreasureEvent::TreasureEvent(Game* game)
	:EventRoom(game)
{
}

void TreasureEvent::playRoom()
{
	VirtualWindow* vwin = game->getVWin();

	game->clearCenterScreen();

	int line = ddutil::EVENT_PICTURE_LINE;

	vwin->printArtFromBottom(Art::getTreasureChest(), Coordinate(0, line), true);

	line += 2;
	game->artifactSelectionMenu(line, game->getActiveZone()->getRandomArtifact());

	game->clearCenterScreen();
}

int TreasureEvent::getRoomId()
{
	return RoomId::Treasure;
}

LavaBeastEvent::LavaBeastEvent(Game* game)
	:EventRoom(game)
{
}

void LavaBeastEvent::playRoom()
{
	VirtualWindow* vwin = game->getVWin();
	game->clearCenterScreen();

	int line = ddutil::EVENT_PICTURE_LINE;
	vwin->printArtFromBottom(Art::getLavaBeast(), Coordinate(0, line), true);
	line += 2;
	vwin->putcenSlowScroll(ColorString("You come across a Lava Beast", ddutil::TEXT_COLOR), line);
	Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
	vwin->clearLine(line);
	vwin->clearLine(line + 1);
	vwin->putcenSlowScroll(ColorString("\"I can offer great rewards, but at a cost.... Do you dare?\"", Art::LAVA_BEAST_COLOR), line);

	std::vector<ColorString> options = {
		ColorString("Full Heal entire party", ddutil::HEAL_COLOR) +
			ColorString(" BUT ", ddutil::MAGENTA) +
			ColorString("everybody loses "+std::to_string(MAX_HEALTH_DECREASE)+" Max HP", ddutil::DAMAGE_COLOR),
		ColorString("+1 Vitality Gain for entire party", ddutil::VITALITY_COLOR) +
			ColorString(" BUT ", ddutil::MAGENTA) +
			ColorString("all future healing reduced by "+std::to_string(HEALING_PERCENT_DECREASE)+"%", ddutil::DAMAGE_COLOR),
		ColorString("Find "+std::to_string(NUM_ARTIFACTS)+" artifacts", ddutil::ARTIFACT_COLOR) +
			ColorString(" BUT ", ddutil::MAGENTA) +
			ColorString("everybody loses "+std::to_string(DAMAGE)+" HP", ddutil::DAMAGE_COLOR),
		ColorString("Reject the offers", ddutil::TEXT_COLOR)
	};

	Menu menu(vwin, options, Coordinate(0, line + 1), true);

	for (int i = line - 1; i < line + 5; i++)
	{
		vwin->clearLine(i);
	}

	ColorString output;
	switch (menu.getResponse())
	{
	case 0:
		for (Player* p : game->getPlayerParty())
		{
			p->decreaseMaxHealth(MAX_HEALTH_DECREASE);
		}
		output = game->healAllPlayers(FULL_HEAL);
		break;
	case 1:
		for (Player* p : game->getPlayerParty())
		{
			p->increaseVitalityPerTurn(VIT_PER_TURN_INCREASE);
			p->decreasePercentHealBoost(HEALING_PERCENT_DECREASE);
		}
		output = ColorString("Everybody gains ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(VIT_PER_TURN_INCREASE) + " extra vitality ", ddutil::VITALITY_COLOR) +
			ColorString(" per turn, but will get ", ddutil::TEXT_COLOR) +
			ColorString("-50% healing", ddutil::DAMAGE_COLOR);
		break;
	case 2:
		for (Player* p : game->getPlayerParty())
		{
			p->reduceHealth(DAMAGE, nullptr, false);
		}

		if (game->checkForPlayerDeaths())
		{
			return;
		}

		game->displayInfo();

		for (int i = 0; i < NUM_ARTIFACTS; i++)
		{
			game->artifactSelectionMenu(line, game->getActiveZone()->getRandomArtifact());
			for (int i = line - 1; i < line + 5; i++)
			{
				vwin->clearLine(i);
			}
		}

		break;
	case 3:
		output = ColorString("With a great crash, the Lava Beast returns to the Sea of Lava", ddutil::TEXT_COLOR);
		break;
	}

	game->displayInfo();
	vwin->putcenSlowScroll(output, line);
	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);

	game->clearCenterScreen();
}

int LavaBeastEvent::getRoomId()
{
	return RoomId::LavaBeast;
}

GoblinEvent::GoblinEvent(Game* game)
	:EventRoom(game)
{
}

void GoblinEvent::playRoom()
{
	VirtualWindow* vwin = game->getVWin();
	game->clearCenterScreen();

	int line = ddutil::EVENT_PICTURE_LINE;
	vwin->printArtFromBottom(Art::getGoblin(), Coordinate(0, line), true);

	line += 2;
	vwin->putcenSlowScroll(ColorString("A Goblin jumps out of nowhere!", ddutil::TEXT_COLOR), line);
	Menu::oneOptionMenu(vwin, ColorString("Choose one person to lose ",ddutil::TEXT_COLOR)+
		ColorString(std::to_string(HEALTH_DECREASE)+" HP", ddutil::DAMAGE_COLOR), Coordinate(0, line + 1), true);

	vwin->clearLine(line);
	vwin->clearLine(line + 1);

	std::vector<ColorString> options;
	for (Player* p : game->getPlayerParty())
	{
		options.push_back(p->getColorString());
	}

	Menu menu(vwin, options, Coordinate(0, line), true);

	Player* selectedPlayer = game->getPlayerParty()[menu.getResponse()];
	ddutil::DamageReport damRep = selectedPlayer->reduceHealth(HEALTH_DECREASE, nullptr, false);

	// need to get the name of the player before they possibly are deleted
	ColorString output = ColorString("The ", ddutil::TEXT_COLOR) + selectedPlayer->getColorString() +
		ColorString(" takes ", ddutil::TEXT_COLOR) +
		ColorString(std::to_string(damRep.getDamageTaken()) + " damage", ddutil::DAMAGE_COLOR);

	if (game->checkForPlayerDeaths())
	{
		return;
	}

	game->clearCenterScreen();
	game->displayInfo();

	vwin->putcen(output, line);

	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);

	game->clearCenterScreen();
}

int GoblinEvent::getRoomId()
{
	return RoomId::Goblin;
}

MirrorEvent::MirrorEvent(Game* game)
	:EventRoom(game)
{
}

void MirrorEvent::playRoom()
{
	
	VirtualWindow* vwin = game->getVWin();

	game->clearCenterScreen();

	int line = ddutil::EVENT_PICTURE_LINE;

	vwin->printArtFromBottom(Art::getMirror(), Coordinate(0, line), true);

	line += 2;

	bool tookDagger = false;
	for (Player* p : game->getPlayerParty())
	{
		if (p->hasArtifact(ArtifactID::RustyDagger) || p->hasArtifact(ArtifactID::EtherealDagger))
		{
			tookDagger = true;
			break;
		}
	}

	vwin->putcenSlowScroll(ColorString("You come across a giant mirror.", ddutil::TEXT_COLOR), line);
	Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
	vwin->clearLine(line); vwin->clearLine(line + 1);
	if (tookDagger)
	{
		vwin->putcenSlowScroll(ColorString("You look through the mirror and see The Catacombs, but there is nothing on the other side....", ddutil::TEXT_COLOR), line);
		Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
		vwin->clearLine(line); vwin->clearLine(line + 1);
	}
	else
	{
		vwin->putcenSlowScroll(ColorString("You look through the mirror and see The Catacombs", ddutil::TEXT_COLOR), line);
		Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
		vwin->clearLine(line); vwin->clearLine(line + 1);
		vwin->putcenSlowScroll(ColorString("On the floor lies a ", ddutil::TEXT_COLOR) + ColorString(" Rusty Dagger", ddutil::MODERATE_COLOR), line);
		Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
		vwin->clearLine(line); vwin->clearLine(line + 1);
		vwin->putcenSlowScroll(ColorString("You put your hand through the mirror and grab the dagger", ddutil::TEXT_COLOR), line);
		Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
		vwin->clearLine(line); vwin->clearLine(line + 1);
		vwin->putcenSlowScroll(ColorString("As the dagger comes through the mirror it suddenly releases a pulse of energy", ddutil::TEXT_COLOR), line);
		Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
		vwin->clearLine(line); vwin->clearLine(line + 1);
		vwin->putcenSlowScroll(ColorString("You look at the dagger, and realize it has changed form.", ddutil::TEXT_COLOR), line);
		Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
		vwin->clearLine(line); vwin->clearLine(line + 1);

		Artifact* a = new EtherealDagger(game);
		a->playFindSound();
		game->artifactSelectionMenu(line, a);
	}
	game->clearCenterScreen();
}

int MirrorEvent::getRoomId()
{
	return RoomId::Mirror;
}

PortalEvent::PortalEvent(Game* game)
	:EventRoom(game)
{
}

void PortalEvent::playRoom()
{
	VirtualWindow* vwin = game->getVWin();

	game->clearCenterScreen();

	int line = ddutil::EVENT_PICTURE_LINE;

	vwin->printArtFromBottom(Art::getPortal(), Coordinate(0, line), true);

	line += 2;

	vwin->putcenSlowScroll(ColorString("You come across a Time Portal that exudes powerful energy", ddutil::TEXT_COLOR), line);
	Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
	vwin->clearLine(line); vwin->clearLine(line + 1);

	vwin->putcenSlowScroll(ColorString("You have no idea to what time this will take you, do you enter?", ddutil::TEXT_COLOR), line);
	Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
	vwin->clearLine(line); vwin->clearLine(line + 1);

	std::vector<ColorString> options = {
		ColorString("Enter [Fight ANY random enemy, from past OR future]", ddutil::DAMAGE_COLOR),
		ColorString("Leave", ddutil::TEXT_COLOR)
	};

	Menu menu(vwin, options, Coordinate(0, line), true);
	if (menu.getResponse() == 0)
	{
		// 23 --> 34
		Enemy* enemy = nullptr;
		switch (ddutil::random(1, 34))
		{
		case 1:
			enemy = new GiantSnail(game);
			break;
		case 2:
			enemy = new GiantSnailVariant(game);
			break;
		case 3:
			enemy = new Ghost(game);
			break;
		case 4:
			enemy = new GhostVariant(game);
			break;
		case 5:
			enemy = new Minotaur(game);
			break;
		case 6:
			enemy = new Brute(game);
			break;
		case 7:
			enemy = new TheMessenger(game);
			break;
		case 8:
			enemy = new GiantLizard(game);
			break;
		case 9:
			enemy = new AltGiantLizard(game);
			break;
		case 10:
			enemy = new Minion(game);
			break;
		case 11:
			enemy = new MinionAlt(game);
			break;
		case 12:
			enemy = new AbyssBeast(game);
			break;
		case 13:
			enemy = new Basilisk(game);
			break;
		case 14:
			enemy = new TheCollector(game);
			break;
		case 15:
			enemy = new SentientMouth(game);
			break;
		case 16:
			enemy = new PossessedMace(game);
			break;
		case 17:
			enemy = new LaughingMask(game);
			break;
		case 18:
			enemy = new DevilishMask(game);
			break;
		case 19:
			enemy = new AncientBird(game);
			break;
		case 20:
			enemy = new HyperBeast(game);
			break;
		case 21:
			enemy = new TheProtector(game);
			break;
		case 22:
			enemy = new TruePatriarch(game);
			break;
		case 23:
			enemy = new MysteriousKnight(game);
			break;
		case 24:
			enemy = new Spider(game);
			break;
		case 25:
			enemy = new CaveBat(game);
			break;
		case 26:
			enemy = new CaveBatVariant(game);
			break;
		case 27:
			enemy = new VampireBat(game);
			break;
		case 28:
			enemy = new VampireBatVariant(game);
			break;
		case 29:
			enemy = new FireBat(game);
			break;
		case 30:
			enemy = new FireBatVariant(game);
			break;
		case 31:
			enemy = new FirePlatypus(game);
			break;
		case 32:
			enemy = new Sniffer(game);
			break;
		case 33:
			enemy = new SnifferVariant(game);
			break;
		case 34:
			enemy = new CorruptedDisciple(game);
			break;
		}
		game->clearCenterScreen();
		EnemyRoom temp(game, enemy);
		temp.playRoom();
	}
	
	game->clearCenterScreen();
}

int PortalEvent::getRoomId()
{
	return RoomId::Portal;
}

BloodAltarEvent::BloodAltarEvent(Game* game)
	:EventRoom(game)
{
}

void BloodAltarEvent::playRoom()
{
	VirtualWindow* vwin = game->getVWin();
	game->clearCenterScreen();

	int line = ddutil::EVENT_PICTURE_LINE;
	vwin->printArtFromBottom(Art::getBloodAltar(), Coordinate(0, line), true);

	line += 2;
	vwin->putcenSlowScroll(ColorString("You stumble across an ancient blood altar", ddutil::TEXT_COLOR), line);
	Menu::oneOptionMenu(vwin, ColorString("One person may trade " + std::to_string(NET_HEALTH_LOSS) +
		" health for " + std::to_string(MAX_HEALTH_GAIN)+ " max health.",ddutil::TEXT_COLOR),
		Coordinate(0, line + 1), true);

	vwin->clearLine(line);
	vwin->clearLine(line + 1);

	std::vector<ColorString> options;
	options.push_back(ColorString("Skip", ddutil::TEXT_COLOR));
	for (Player* p : game->getPlayerParty())
	{
		options.push_back(p->getColorString());
	}

	Menu menu(vwin, options, Coordinate(0, line), true);

	Player* selectedPlayer = nullptr;
	int response = menu.getResponse();
	if (response > 0) // update to non nullptr if the player chose a person
	{
		selectedPlayer = game->getPlayerParty()[menu.getResponse()-1];
	}

	ColorString output;
	if (selectedPlayer == nullptr)
	{
		output = ColorString("You continue without using the shrine", ddutil::TEXT_COLOR);
	}
	else
	{			
		selectedPlayer->increaseMaxHealth(MAX_HEALTH_GAIN);
		ddutil::DamageReport damRep = selectedPlayer->reduceHealth(MAX_HEALTH_GAIN+NET_HEALTH_LOSS, nullptr, false);

		// need to get the name of the player before they possibly are deleted
		output = ColorString("The ", ddutil::TEXT_COLOR) + selectedPlayer->getColorString() +
			ColorString(" gains " + std::to_string(MAX_HEALTH_GAIN) + " max health ", ddutil::TEXT_COLOR) +
			ColorString("but loses " + std::to_string(NET_HEALTH_LOSS) + " current health.", ddutil::TEXT_COLOR);
	}

	game->clearCenterScreen();
	game->displayInfo();

	vwin->putcen(output, line);

	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);

	game->clearCenterScreen();
}

int BloodAltarEvent::getRoomId()
{
	return RoomId::BloodAltar;
}

SpiderEvent::SpiderEvent(Game* game)
	:EventRoom(game)
{
}

void SpiderEvent::playRoom()
{
	VirtualWindow* vwin = game->getVWin();

	game->clearCenterScreen();

	int line = ddutil::EVENT_PICTURE_LINE;

	vwin->printArtFromBottom(Art::getSpider(), Coordinate(0, line), true);

	line += 2;
	vwin->putcenSlowScroll(ColorString("A ", ddutil::TEXT_COLOR) + ColorString("Giant Spider", Art::SPIDER_COLOR) +
		ColorString(" descends from the ceiling!", ddutil::TEXT_COLOR), line++);

	std::vector<ColorString> options = {
		ColorString("Fight", ddutil::DAMAGE_COLOR),
		ColorString("Attempt Escape ("+std::to_string(ESCAPE_CHANCE)+"% success; " + 
		            std::to_string(100 - ESCAPE_CHANCE) + "% failure and everyone takes " +
					std::to_string(INITIAL_DAMAGE) + " damage)", ddutil::YELLOW)
	};
	Menu menu(vwin, options, Coordinate(0, line), true);

	bool fight = false;
	int response = menu.getResponse();
	// Check if they are able to escape, if not then set fight to true
	if (response == 1)
	{
		fight = (ddutil::random(1, 100) > ESCAPE_CHANCE);
		if (fight)
		{
			for (Player* p : game->getPlayerParty())
			{
				p->reduceHealth(INITIAL_DAMAGE, nullptr);
			}
		}
		else
		{
			game->clearCenterScreen();
			game->clearBottomDivider();
			game->displayInfo();
			ColorString output = ColorString("You escape the spider without taking any damage", ddutil::TEXT_COLOR);
			vwin->putcen(output, line);
			Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
		}
	}
	if (response == 0 || fight)
	{
		EnemyRoom temp(game, new Spider(game));
		temp.playRoom();
	}

	game->clearBottomDivider();
	game->clearCenterScreen();
}

int SpiderEvent::getRoomId()
{
	return RoomId::SpiderEventFight;
}

MaskEvent::MaskEvent(Game* game)
	:EventRoom(game)
{
}

void MaskEvent::playRoom()
{
	VirtualWindow* vwin = game->getVWin();

	game->clearCenterScreen();

	int line = ddutil::EVENT_PICTURE_LINE;

	vwin->printArtFromBottom(Art::getAncientMask(), Coordinate(0, line), true);

	line += 2;

	AncientMask* mask = new AncientMask(game);

	vwin->putcenSlowScroll(ColorString("You notice something buried in the ash. Upon closer inspection, you find an ", ddutil::TEXT_COLOR) +
		mask->getName(), line++);

	std::vector<ColorString> options = {
		ColorString("Take the ", ddutil::TEXT_COLOR) + mask->getFullInformation(),
		ColorString("Leave it", ddutil::TEXT_COLOR)
	};

	Menu menu(vwin, options, Coordinate(0, line), true);

	vwin->clearLine(line - 1);
	vwin->clearLine(line);
	vwin->clearLine(line + 1);
	line--;

	if (menu.getResponse() == 0) // take the mask 
	{
		game->artifactSelectionMenu(line, mask);

	}
	else // leave the dagger
	{
		delete mask;
		vwin->putcen(ColorString("You choose to leave the mask", ddutil::TEXT_COLOR), line++);
		Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, line), true);
	}

	game->clearCenterScreen();

}

int MaskEvent::getRoomId()
{
	return RoomId::Mask;
}

GoldAltarEvent::GoldAltarEvent(Game* game)
	:EventRoom(game)
{
}

void GoldAltarEvent::playRoom()
{
	VirtualWindow* vwin = game->getVWin();
	game->clearCenterScreen();

	int line = ddutil::EVENT_PICTURE_LINE;
	vwin->printArtFromBottom(Art::getGoldAltar(), Coordinate(0, line), true);

	line += 2;
	vwin->putcenSlowScroll(ColorString("You stumble across an ancient golden altar", ddutil::TEXT_COLOR), line);
	Menu::oneOptionMenu(vwin, ColorString("One person may receive a full heal", ddutil::HEAL_COLOR),
		Coordinate(0, line + 1), true);

	vwin->clearLine(line);
	vwin->clearLine(line + 1);

	std::vector<ColorString> options;
	options.push_back(ColorString("Skip", ddutil::TEXT_COLOR));
	for (Player* p : game->getPlayerParty())
	{
		options.push_back(p->getColorString());
	}

	Menu menu(vwin, options, Coordinate(0, line), true);

	Player* selectedPlayer = nullptr;
	int response = menu.getResponse();
	if (response > 0) // update to non nullptr if the player chose a person
	{
		selectedPlayer = game->getPlayerParty()[menu.getResponse()-1];
	}

	ColorString output;
	if (selectedPlayer == nullptr)
	{
		output = ColorString("You continue without using the shrine", ddutil::TEXT_COLOR);
	}
	else
	{			
		int healthIncrease = selectedPlayer->increaseHealth(selectedPlayer->getMaxHealth(100));

		output = ColorString("The ", ddutil::TEXT_COLOR) + selectedPlayer->getColorString() +
			ColorString(" gains " + std::to_string(healthIncrease) + " health ", ddutil::HEAL_COLOR);
	}

	game->clearCenterScreen();
	game->displayInfo();

	vwin->putcen(output, line);

	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);

	game->clearCenterScreen();
}

int GoldAltarEvent::getRoomId()
{
	return RoomId::GoldAltar;
}


