#include "game.h"
#include "creature.h"
#include "enemy.h"
#include "player.h"
#include "utilities.h"
#include "art.h"
#include "zone.h"
#include "room.h"
#include "virtualwindow.h"
#include "menu.h"
#include "move.h"
#include "soundfile.h"
#include "coordinate.h"
#include "compendium.h"
#include "savefile.h"

#include <Windows.h>
#include <string>
#include <algorithm>

Game::Game(VirtualWindow* virWin)
	:profile()
{
	score = 0;
	greenMoveCost = ddutil::MODERATE_COST;
	blueMoveCost = ddutil::POWERFUL_COST;
	redMoveCost = ddutil::MYTHICAL_COST;
	currentDistortion = profile.getHighestDistortion();
	vwin = virWin;
	compendium = new Compendium(vwin);
	gameWin = false;
	skipToNextChapter = false;

	playerParty.push_back(new Samurai(this));
	playerParty.push_back(new Sorcerer(this));
	playerParty.push_back(new Gunslinger(this));

	currentEnemy = nullptr;

	gameOver = false;
	status = ddutil::GameStatus::CONTINUE;


	// the zone index is one less than what number the game displays for the zone
	currentZoneIndex = 0;
	for (int i = 1; i <= Zone::LAST_ZONE; i++)
	{
		gameWorld.push_back(new Zone(this, i));
	}
	loadedFromFile = false;

	if (profile.isCorrupted())
	{
		vwin->putcen(ColorString("Profile data was corrupted, so values were reset to default.", ddutil::RED), ddutil::EVENT_PICTURE_LINE);
		Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, ddutil::EVENT_PICTURE_LINE + 1), true);
	}
	if (profile.wasNoFileFound())
	{
		vwin->putcen(ColorString("No profile data was found, so a default profile was created.", ddutil::RED), ddutil::EVENT_PICTURE_LINE);
		Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, ddutil::EVENT_PICTURE_LINE + 1), true);
	}

	if (profile.getNumLosses() == 0 && profile.getNumWins() == 0)
	{
		displayHelpScreen();
	}
}

Game::~Game()
{
	for (unsigned int i = 0; i < playerParty.size(); i++)
	{
		delete playerParty[i];
	}

	for (unsigned int i = 0; i < gameWorld.size(); i++)
	{
		delete gameWorld[i];
	}

	for (unsigned int i = 0; i < deadPlayers.size(); i++)
	{
		delete deadPlayers[i];
	}

	delete compendium;

}

ddutil::GameStatus Game::run()
{
	playSound(Mp3File("title"));
	titleScreen();

	if (status == ddutil::GameStatus::EXIT)
	{
		stopSound(SoundType::MP3);
		return status;
	}
	if (status == ddutil::GameStatus::RESTART)
	{
		stopSound(SoundType::MP3);
		return status;
	}
	
	if (!loadedFromFile)
	{
		intro();
		chooseClasses();
	}
	stopSound(SoundType::MP3);

	for (; currentZoneIndex < gameWorld.size() && !gameOver; currentZoneIndex++)
	{
		displayZoneEntrance();
		
		Zone* activeZone = gameWorld[currentZoneIndex];

		while (activeZone->hasMoreRooms())
		{
			Room* currentRoom = activeZone->chooseRoom();
			currentRoom->setCharIn();

			displayInfo();
			drawPlayerParty();
			roomEntranceAnimation();

			currentRoom->playRoom();

			if (gameOver)
			{
				status = ddutil::GameStatus::RESTART;
				if (score > profile.getHighestScore())
				{
					profile.setNewHighScore(score);
				}
				profile.incrementNumLosses();
				Savefile::eraseFile("save.txt");
				break;
			}

			if (gameWin)
			{
				break;
			}

			currentRoom->setCharEmpty();

			Savefile save(this);
			save.writeToFile("save.txt");
			int line = ddutil::BOTTOM_TEXT_LINE - 4;
			Menu::oneOptionMenu(vwin, ColorString("Run Data Saved", ddutil::TEXT_COLOR), Coordinate(0, line), true);
			vwin->clearLine(line);

			if (shouldSkipChapter())
			{
				break;
			}
		}
		if (gameWin)
		{
			status = ddutil::GameStatus::RESTART;
			profile.incrementNumWins();
			if (score > profile.getHighestScore())
			{
				profile.setNewHighScore(score);
			}
			if (currentDistortion == profile.getHighestDistortion())
			{
				if (currentDistortion == 5)
				{
					vwin->clearScreen();
					int line = ddutil::CONSOLEY / 2;
					vwin->putcen(ColorString("You completed the game! Thank you and Congratulations!", ddutil::DISTORTION_COLOR), line++);
					playSound(WavFile("mythicalartifact", false, false));
					Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, line), true);
					vwin->clearScreen();
				}
				else
				{
					profile.incrementHighestDistortion();
					vwin->clearScreen();
					int line = ddutil::CONSOLEY / 2;
					vwin->putcen(ColorString("New Distortion Unlocked!", ddutil::DISTORTION_COLOR), line++);
					playSound(WavFile("lightning", false, false));
					Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, line), true);
					vwin->clearScreen();
				}
			}
			Savefile::eraseFile("save.txt");
			break;
		}
	}


	return status;
}

void Game::battle(Enemy* enemy)
{
	clearCenterScreen();
	clearBottomDivider();
	
	enemy->getMusic().play();

	currentEnemy = enemy;

	const int FLOOR_COORD = ddutil::CENTER_TEXT_LINE - 1; // coordinate where the bottom layer of the players and monsters are drawn
	const int OFFSET = 2;
	const int MONSTER_X = ddutil::CONSOLEX - enemy->getPicture().getLength() - OFFSET; // x coordinate of left of monster
	const int PLAYER_X = OFFSET; // x coordinate of left side of active player

	const int PLAYER_TEXT_LINE = ddutil::BOTTOM_TEXT_LINE;
	const int MONSTER_TEXT_LINE = 1;

	const int INTENT_LINE = ddutil::CENTER_TEXT_LINE;

	const Coordinate MONSTER_COORD = Coordinate(MONSTER_X, FLOOR_COORD);
	const Coordinate PLAYER_COORD = Coordinate(PLAYER_X, FLOOR_COORD);

	// Entrance screen introducing the monster
	vwin->printArtFromBottom(enemy->getPicture(), MONSTER_COORD, false);

	std::vector<ColorString> intro = enemy->getIntroText();
	for (unsigned int i = 0; i < intro.size(); i++)
	{
		vwin->putcenSlowScroll(intro[i] , ddutil::CENTER_TEXT_LINE);
		std::string continueText;
		if (i - 1 == intro.size())
		{
			continueText = "Fight!";
		}
		else
		{
			continueText = "...";
		}
		Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, ddutil::CENTER_TEXT_LINE + 1), true);
		vwin->clearLine(ddutil::CENTER_TEXT_LINE);
		vwin->clearLine(ddutil::CENTER_TEXT_LINE + 1);
	}


	// initial set up
	// starts battle with the first person in the player party as the active fighter
	for (Player* p : playerParty)
	{
		p->resetVitality();
		p->clearAllStatuses(); // make sure no statuses from previous battles are lingering
		p->applyStartingStatuses(enemy);
		p->doStartBattleArtifactEffects(enemy);
	}
	
	bool firstTimeThrough = true; // flag so that it doesnt apply statuses that are from the self status list
	// battle loop
	while (true)
	{
		int activePlayerIndex = 0;

		if (firstTimeThrough)
		{
			firstTimeThrough = false;
		}
		else
		{
			// display status information for the players and do the status actions
			for (Player* p : playerParty)
			{
				if (p->getNumberOfStatuses() != 0)
				{
					printCreatureBattleInfo(p, PLAYER_COORD, PLAYER_TEXT_LINE);
					doStatusDecrementionAndPrintInfo(p);
					vwin->clearArt(p->getPicture(), PLAYER_COORD);
				}
			}
			if (checkForEnemyDeath(enemy))
			{
				return;
			}
		}
		if (checkForPlayerDeaths())
		{
			return;
		}
		
		// reset block & dodgeChance for players
		for (Player* p : playerParty)
		{
			p->resetBlock();
			p->resetDodgeChance();
		}

		// cast the players to creatures so we can plug this into getTurn(std::vector<Creature*>)
		std::vector<Creature*> temp;
		for (Player* p : playerParty)
		{
			temp.push_back(p);
		}
		EnemyTurn enemyTurn = enemy->getTurn(temp);
		printCreatureBattleInfo(enemy, MONSTER_COORD, MONSTER_TEXT_LINE);

		// Player move selection
		std::vector<Move*> usedMoves; // keep track of moves played so that the player can't play the same move twice
		std::vector<Move*> queuedMoves; // pressing enter plays all queued moves
		int prevPos = 0;
		bool autoPlayMoves = false; // set to true when the player starts to play moves from the queue
		while (true)
		{
			// Print  information
			Player* activePlayer = playerParty[activePlayerIndex];
			printCreatureBattleInfo(activePlayer, PLAYER_COORD, PLAYER_TEXT_LINE);

			vwin->putcen(enemyTurn.getIntent(), INTENT_LINE);
			printCreatureBattleInfo(enemy, MONSTER_COORD, MONSTER_TEXT_LINE);

			const int CHAR_SWAP_LINE = PLAYER_TEXT_LINE + 2;
			const int MOVE_TEXT_LINE = PLAYER_TEXT_LINE + 3;
			
			std::vector<Move*> activeMoves = activePlayer->getMoves();

			// Set up menu for selecting the move / changing active character
			std::vector<ColorString> options;
			for (Move* i : activeMoves)
			{
				if (activePlayer->hasStatus(StatusID::Hexed))
				{
					options.push_back(ColorString("Hexed", HexedStatus::COLOR));
					continue;
				}
				else if (activePlayer->hasStatus(StatusID::Strangled))
				{
					options.push_back(ColorString("Strangled", StrangledStatus::COLOR));
					continue;
				}
				else if (activePlayer->hasStatus(StatusID::Stunned))
				{
					options.push_back(ColorString("Stunned", StunnedStatus::COLOR));
					continue;
				}

				if (std::find(usedMoves.begin(), usedMoves.end(), i) == usedMoves.end()) // the move hasnt been used
				{
					options.push_back(i->getFullInformation());
				}
				else
				{
					options.push_back(ColorString("Already used this turn", ddutil::BROWN));
				}
			}

			// figure out which players are adjacent to the current active player

			std::vector<int> otherInputs; // other keypress inputs that handle switching characters
			Player* previousPlayer = nullptr;
			Player* nextPlayer = nullptr;
			if (playerParty.size() > 1) // make sure there are actual people you can switch to
			{
				otherInputs = { VK_LEFT, VK_RIGHT }; // only enabled if there are multiple players alive

				if (activePlayer == playerParty.front())
				{
					previousPlayer = playerParty.back();
					nextPlayer = playerParty[activePlayerIndex + 1];
				}
				else if (activePlayer == playerParty.back())
				{
					previousPlayer = playerParty[activePlayerIndex - 1];
					nextPlayer = playerParty.front();
				}
				else // somewhere in the middle
				{
					previousPlayer = playerParty[activePlayerIndex - 1];
					nextPlayer = playerParty[activePlayerIndex + 1];
				}

				ColorString playerSwap = previousPlayer->getColorString() +
					ColorString(" <-- Switch --> ", ddutil::MENU_SELECTION_COLOR) + nextPlayer->getColorString();
				vwin->putcen(playerSwap, CHAR_SWAP_LINE);
			}

			otherInputs.push_back(VK_SPACE);
			otherInputs.push_back(VK_MENU);
			otherInputs.push_back(VK_SHIFT);
			otherInputs.push_back(VK_BACK);
			const int END_TURN_LINE = ddutil::DIVIDER_LINE4;
			ColorString inputInfo;
			if (queuedMoves.empty())
			{
				inputInfo = ColorString("Space: End Turn  ", ddutil::TEXT_COLOR) +
					ColorString("Enter: Select Move  ", ddutil::RED) +
					ColorString("Alt: Menu  ", ddutil::ARTIFACT_COLOR) +
					ColorString("Shift: Queue move", ddutil::LIGHTGREEN);
			}
			else
			{
				inputInfo = ColorString("Queue: ", ddutil::LIGHTGREEN);
				for (Move* m : queuedMoves)
				{
					inputInfo += m->getColorString() + ColorString(", ", ddutil::TEXT_COLOR);
				}
				inputInfo += ColorString(" (Backspace: clear, Enter: use)", ddutil::LIGHTGREEN);
			}
			ddutil::trimLength(inputInfo);
			vwin->putcen(inputInfo, END_TURN_LINE);

			int response;
			int otherInputResponse;
			bool endTurn = false;
			bool addedMoveToQueue = false;
			if (autoPlayMoves)
			{
				response = 0; // will go to the move using screen, and will automatically select newest move in queue
				otherInputResponse = 0;
				prevPos = 0;  // so these values don't really matter
			}
			else
			{
				Menu moveMenu(vwin, options, otherInputs, Coordinate(0, MOVE_TEXT_LINE), true, prevPos);

				// getReponse will either be an low value integer corresponding to a move index, OR VK_LEFT/VK_RIGHT indicating character change
				response = moveMenu.getResponse();
				otherInputResponse = moveMenu.getOtherInputResponse();
				prevPos = otherInputResponse;
			}

			switch (response)
			{
			case VK_LEFT:
				queuedMoves.clear();
				activePlayerIndex--;
				if (activePlayerIndex < 0)
				{
					activePlayerIndex = playerParty.size() - 1;
				}
				break;
			case VK_RIGHT:
				queuedMoves.clear();
				activePlayerIndex++;
				if (activePlayerIndex >= static_cast<int>(playerParty.size()))
				{
					activePlayerIndex = 0;
				}
				break;
			case VK_SPACE:
			{
				clearBottomDivider();
				vwin->putcen(ColorString("End Turn?", ddutil::TEXT_COLOR), MOVE_TEXT_LINE - 2);
				std::vector<ColorString> endTurnOptions = {
					ColorString("Yes", ddutil::GREEN),
					ColorString("No", ddutil::RED)
				};
				Menu confirmEndTurnMenu(vwin, endTurnOptions, Coordinate(0, MOVE_TEXT_LINE), true);
				int endTurnResponse = confirmEndTurnMenu.getResponse();
				if (endTurnResponse == 0)
				{
					endTurn = true;
				} // else do nothing, the turn will repeat itself
			}
				break;
			case VK_MENU:
				viewMenu();
				break;
			case VK_SHIFT:
			{
				// check to make sure the move hasnt been used this turn yet
				Move* selectedMove = activeMoves[otherInputResponse];
				if (std::find(usedMoves.begin(), usedMoves.end(), selectedMove) != usedMoves.end())
				{
					clearBottomDivider();
					vwin->putcen(ColorString("That move has already been used this turn", ddutil::TEXT_COLOR), PLAYER_TEXT_LINE);
					Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, PLAYER_TEXT_LINE + 1), true);
					break;
				}
				if (std::find(queuedMoves.begin(), queuedMoves.end(), selectedMove) != queuedMoves.end())
				{
					clearBottomDivider();
					vwin->putcen(ColorString("That move is already in the queue", ddutil::TEXT_COLOR), PLAYER_TEXT_LINE);
					Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, PLAYER_TEXT_LINE + 1), true);
					break;
				}
				queuedMoves.push_back(selectedMove);
				addedMoveToQueue;
				break;
			}
			case VK_BACK:
				queuedMoves.clear();
				break;

			default: // corresponds to an index of the player's moves,	
				Move* selectedMove;
				if (queuedMoves.empty())
				{
					selectedMove = activeMoves[otherInputResponse];
				}
				else
				{
					selectedMove = queuedMoves.front();
					queuedMoves.erase(queuedMoves.begin());
					if (queuedMoves.size() > 0)
					{
						autoPlayMoves = true;
					}
					else
					{
						autoPlayMoves = false;
					}
				}

				if (activePlayer->hasStatus(StatusID::Hexed) || 
					activePlayer->hasStatus(StatusID::Strangled) ||
					activePlayer->hasStatus(StatusID::Stunned))
				{
					clearBottomDivider();
					vwin->putcen(
						ColorString("The ", ddutil::TEXT_COLOR) +
						activePlayer->getColorString() +
						ColorString(" cannot attack this turn", ddutil::TEXT_COLOR), 
						PLAYER_TEXT_LINE
					);
					Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, PLAYER_TEXT_LINE + 1), true);
					break;
				}
				// check to make sure the move hasnt been used this turn yet
				if (std::find(usedMoves.begin(), usedMoves.end(), selectedMove) != usedMoves.end())
				{
					clearBottomDivider();
					vwin->putcen(ColorString("That move has already been used this turn", ddutil::TEXT_COLOR), PLAYER_TEXT_LINE);
					Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, PLAYER_TEXT_LINE + 1), true);
					break;
				}

				if (activePlayer->subVitality(selectedMove->getCost()))
				{
					Creature* target = nullptr;
					if (selectedMove->canChooseTarget())
					{
						// Let the player choose the target of their attack

						// Menu Key
						// 0 : enemy
						// 1 : playerParty[0]
						// 2 : playerParty[1]
						// 3 : playerParty[2]
						// ...

						std::vector<ColorString> options = { enemy->getStatLine() };
						for (Player* p : playerParty)
						{
							options.push_back(p->getStatLine());
						}

						clearBottomDivider();
						vwin->putcen(selectedMove->getFullInformation(), ddutil::DIVIDER_LINE4);
						Menu targetMenu(vwin, options, Coordinate(0, PLAYER_TEXT_LINE), true);
						
						if (targetMenu.getResponse() == 0)
						{
							target = enemy;
						}
						else // getResponse() is one more than the index of the target in playerParty
						{
							target = playerParty[targetMenu.getResponse() - 1];
						}
					}
					else
					{
						target = enemy;
					}
					
					Sleep(100); // buffer so that you dont accidentally spam through the moves

					ColorString targetString;
					if (selectedMove->canChooseTarget())
					{
						targetString += ColorString(" on The ", ddutil::TEXT_COLOR) + target->getColorString();
					}

					// confirm that you want to use the move
					clearBottomDivider();
					vwin->putcen(
						ColorString("Use ", ddutil::TEXT_COLOR) + selectedMove->getColorString() +
						targetString +
						ColorString("?", ddutil::TEXT_COLOR),
						PLAYER_TEXT_LINE
					);
					std::vector<ColorString> options = {
						ColorString("Yes", ddutil::DAMAGE_COLOR),
						ColorString("No", ddutil::BROWN)
					};
					Menu menu(vwin, options, Coordinate(0, PLAYER_TEXT_LINE + 1), true);
					if (menu.getResponse() == 1)
					{
						activePlayer->addVitality(selectedMove->getCost());
						break;
					}
					clearBottomDivider();


					ColorString info = selectedMove->doAction(activePlayer, target);
					if (info.getString() != ddutil::NOT_ENOUGH_BULLETS &&
						info.getString() != ddutil::NOT_ENOUGH_AURA && 
						info.getString() != ddutil::CANT_USE_MOVE)
					{
						usedMoves.push_back(selectedMove);
						selectedMove->playSoundEffect();
					}
					else
					{
						activePlayer->addVitality(selectedMove->getCost()); // refund the cost
						playSound(WavFile("error", ddutil::SF_LOOP, ddutil::SF_ASYNC));
					}

					vwin->putcen(info, PLAYER_TEXT_LINE);
					printCreatureBattleInfo(enemy, MONSTER_COORD, MONSTER_TEXT_LINE);

					Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, PLAYER_TEXT_LINE + 1), true);
				}
				else
				{
					clearBottomDivider();
					vwin->putcen(ColorString("Not enough vitality!", ddutil::VITALITY_COLOR), PLAYER_TEXT_LINE);
					Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, PLAYER_TEXT_LINE + 1), true);
				}
				break;
			} // end of switch statement
			if (addedMoveToQueue)
			{
				continue;
			}

			if (checkForPlayerDeaths() || checkForEnemyDeath(enemy))
			{
				return;
			}

			// Check to see if the player wanted to end their turn. If not, reenter the player move selection loop
			if (endTurn)
			{
				for (Player* p : playerParty)
				{
					if (p->getVitality() == 0 && p->hasArtifact(ArtifactID::BrawlersBelt))
					{
						p->addVitality(BrawlersBelt::VIT_INC);
					}
					p->addVitality(p->getVitalityGain());
				}
				break;
			}
			else
			{
				// clear up the screen to draw the character's information again
				clearBottomDivider();
				vwin->clearArt(activePlayer->getPicture(), PLAYER_COORD);
			}
		}

		// Enemy Turn

		// apply status effects
		doStatusDecrementionAndPrintInfo(enemy);
		if (checkForPlayerDeaths() || checkForEnemyDeath(enemy))
		{
			return;
		}
		printCreatureBattleInfo(enemy, MONSTER_COORD, MONSTER_TEXT_LINE);

		// reset block
		enemy->resetBlock();
		enemy->resetDodgeChance();
		printCreatureBattleInfo(enemy, MONSTER_COORD, MONSTER_TEXT_LINE);

		// make sure none of the targets have died since being targeted
		enemyTurn.checkForDeadTargets();

		// Do enemy move
		clearBottomDivider();
		for (Creature* p : enemyTurn.getTargets())
		{
			ColorString info = enemyTurn.getMove()->doAction(enemy, p);
			if (dynamic_cast<Enemy*>(p) == nullptr) // if not an enemy
			{
				vwin->clearArt(playerParty[activePlayerIndex]->getPicture(), PLAYER_COORD);
				printCreatureBattleInfo(p, PLAYER_COORD, PLAYER_TEXT_LINE);
			}
			vwin->clearLine(INTENT_LINE);
			enemyTurn.getMove()->playSoundEffect();
			vwin->putcen(info, INTENT_LINE);
			Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, INTENT_LINE + 1), true);
			vwin->clearArt(p->getPicture(), PLAYER_COORD);
		}

		if (checkForPlayerDeaths() || checkForEnemyDeath(enemy))
		{
			return;
		}
			

		clearCenterScreen();
		clearBottomDivider();
	}
}

void Game::addPlayer(Player* newPlayer)
{
	if (newPlayer->isMinion())
	{
		newPlayer->appendToName(ddutil::getCuteMinionName());
	}
	std::string name = newPlayer->getColorString().getString();
	int count = 0;
	for (Player* p : playerParty)
	{
		if (p->getColorString().getString() == name)
		{
			count++;
		}
	}
	if (count > 0)
	{
		newPlayer->appendCount(count);
	}
	playerParty.push_back(newPlayer);
}

std::vector<Player*> Game::getPlayerParty()
{
	return playerParty;
}

std::vector<Player*> Game::getDeadPlayers()
{
	return deadPlayers;
}

Player* Game::revivePlayer(int index)
{
	if (index < 0 || index >= static_cast<int>(deadPlayers.size()))
	{
		return nullptr;
	}
	Player* player = deadPlayers.at(index);
	deadPlayers.erase(deadPlayers.begin() + index);
	playerParty.push_back(player);
	player->setHealthPercent(ddutil::NORMAL_REVIVAL_PERCENT);
	return player;
}

void Game::removePlayer(Player* player)
{
	playerParty.erase(std::remove(playerParty.begin(), playerParty.end(), player), playerParty.end());
}

Enemy* Game::getCurrentEnemy()
{
	return currentEnemy;
}

VirtualWindow* Game::getVWin()
{
	return vwin;
}

void Game::titleScreen()
{
	vwin->clearScreen();

	Sleep(500);

	std::vector<ColorString> options;
	options.push_back(ColorString("New Game", ddutil::TEXT_COLOR));
	options.push_back(ColorString("Load Game", ddutil::TEXT_COLOR));
	options.push_back(ColorString("View Compendium", ddutil::TEXT_COLOR));
	options.push_back(ColorString("Resize Console", ddutil::TEXT_COLOR));
	options.push_back(ColorString("Exit", ddutil::TEXT_COLOR));

	std::vector<int> otherInput = { VK_ESCAPE };
	
	bool exit = false;
	while (!exit)
	{
		vwin->printArt(Art::getTitle(), Coordinate(0, 5), true);
		vwin->put(ColorString("Version 1.8", ddutil::DARKGRAY), Coordinate(1, 1));

		vwin->putcen(profile.getColorString(), ddutil::CONSOLEY - 2);
		
		const int MENU_TOP_LINE = 25;
		Menu menu(vwin, options, otherInput, Coordinate(0, MENU_TOP_LINE), true);
		int response = menu.getResponse();

		switch (response)
		{
		case 0: // start
			status = ddutil::GameStatus::CONTINUE;
			exit = true;
			for (int i = MENU_TOP_LINE; i < MENU_TOP_LINE + 6; i++)
				vwin->clearLine(i);
			{
				vwin->putcen(ColorString(
					"Distortion levels are stacking difficulty modifiers",
					ddutil::YELLOW), MENU_TOP_LINE - 4
				);
				vwin->putcen(ColorString("unlocked by beating the previous distortion level", ddutil::YELLOW), MENU_TOP_LINE - 3);
				vwin->putcen(ColorString("Choose ", ddutil::TEXT_COLOR) + ColorString("Distortion", ddutil::DISTORTION_COLOR) +
					ColorString(" level", ddutil::TEXT_COLOR), MENU_TOP_LINE - 1);
				std::vector<ColorString> distOptions;
				for (int i = 0; i <= profile.getHighestDistortion(); i++)
				{
					distOptions.push_back(ddutil::getDistortionDescription(i));
				}
				Menu distMenu(vwin, distOptions, Coordinate(0, MENU_TOP_LINE), true);
				currentDistortion = distMenu.getResponse();
				setDistortionModifers();
			}
			break;
			
		case 1: // load game
		{
			status = ddutil::GameStatus::CONTINUE;
			exit = true;
			loadedFromFile = true;
			try
			{
				Savefile save(this, "save.txt");
				save.loadIntoGame(this);
			}
			catch (std::exception& ex)
			{
				playSound(WavFile("error", false, false));
				exit = false;
				loadedFromFile = false;
				for (int i = MENU_TOP_LINE; i < MENU_TOP_LINE + 6; i++)
					vwin->clearLine(i);
				Menu::oneOptionMenu(vwin, ColorString("Error: " + std::string(ex.what()), ddutil::TEXT_COLOR), Coordinate(0, MENU_TOP_LINE), true);
				ddutil::xorFile("save.txt");
			}	

			break;
		}

		case 2:
			compendium->display();
			break;

		case 3:
		{
			for (int i = MENU_TOP_LINE; i < MENU_TOP_LINE + 6; i++)
				vwin->clearLine(i);
			std::vector<ColorString> sizeOptions = {
				ColorString("Y = 4", ddutil::TEXT_COLOR),
				ColorString("Y = 8", ddutil::TEXT_COLOR),
				ColorString("Y = 12", ddutil::TEXT_COLOR),
				ColorString("Y = 16", ddutil::TEXT_COLOR),
				ColorString("Y = 20", ddutil::TEXT_COLOR),
				ColorString("Y = 24", ddutil::TEXT_COLOR),
				ColorString("Y = 28", ddutil::TEXT_COLOR),
			};
			vwin->putcen(ColorString("Choose size:", ddutil::TEXT_COLOR), MENU_TOP_LINE - 1);
			Menu sizeMenu(vwin, sizeOptions, Coordinate(0, MENU_TOP_LINE), true, 3);
			int size = (sizeMenu.getResponse() * 4) + 4;
			vwin->getConsole().setFontSize(size);
			
			break;
		}


		case 4:
		case VK_ESCAPE: // quit
			status = ddutil::GameStatus::EXIT;
			exit = true;
			break;
		}
		if (exit)
		{
			screenTransition(ddutil::BLACK);
		}
		else
		{
			vwin->clearScreen();
		}
	}


}

void Game::intro()
{
	int titleLine = ddutil::CONSOLEY / 2;
	vwin->putcen(ColorString("Prologue", Art::WATCHER_COLOR), titleLine);
	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, titleLine + 1), true);

	vwin->printArt(Art::getTheWatcher(), Coordinate(0, 4), true);

	int line = (ddutil::CONSOLEY * 3) / 4;
	int color = Art::WATCHER_COLOR;

	std::vector<ColorString> text = {
		ColorString("\"I am The Watcher.\"", color),
		ColorString("\"I watch over this world across time itself.\"", color),
		ColorString("\"I have transported you three here because we face a grave threat.\"", color),
		ColorString("\"The Ancient Demon 'The Patriarch' has broken from his Seal.\"", color),
		ColorString("\"If he is not stopped, this world will cease to exist as we know it.\"", color),
		ColorString("\"You three must destroy him and his minions before it's too late!\"", color),
		ColorString("\"Please, the world is counting on you.\"", color),
	};
	if (currentDistortion == 1)
	{
		text = {
			ColorString("\"I am once again calling you three to the world's aid\"", color),
			ColorString("\"The Patriarch has once again broken free of his seal\"", color),
			ColorString("\"This occurred much faster than anticipated\"", color),
			ColorString("\"There must have been some distortions in the fabric of spacetime\"", color),
			ColorString("\"Nevertheless, there is no time to lose! Please get going...\"", color)
		};
	}
	else if (currentDistortion == 2)
	{
		text = {
			ColorString("\"Unfortunately, I must call upon you all once more\"", color),
			ColorString("\"It appears that my summons keep causing more distortions in spacetime\"", color),
			ColorString("\"I believe that your quest will become difficult with more distortions\"", color),
			ColorString("\"This is an unfortunate side effect which cannot be ignored\"", color),
			ColorString("\"Nevertheless, I wish you the best of luck.\"", color)
		};
	}
	else if (currentDistortion == 3)
	{
		text = {
			ColorString("\"I am extremely sorry to have to summon you all here again\"", color),
			ColorString("\"There is no time to waste. You know what to do.\"", color)
		};
	}
	else if (currentDistortion == 4)
	{
		text = {
			ColorString("\"Oh this is terrible!\"", color),
			ColorString("\"The distortions have become so great, my revival powers are waning!\"", color),
			ColorString("\"This time, it looks like you'll have to be extra careful!\"", color),
			ColorString("\"Please, the whole universe is counting on you\"", color)
		};
	}
	else if (currentDistortion == 5)
	{
		text = {
			ColorString("\"Disaster has struck!\"", color),
			ColorString("\"The Patriarch has broken from his seal, and is more powerful than ever!\"", color),
			ColorString("\"However, with this newfound power comes new vulnerabilities\"", color),
			ColorString("\"If you take him out now, he will be gone forever!\"", color),
			ColorString("\"I am sorry there is not more I can do for you\"", color),
			ColorString("\"I humbly beg for your service once last time\"", color)
		};
	}
	
	for (ColorString c : text)
	{
		vwin->putcenSlowScroll(c, line);
		Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
		vwin->clearLine(line);
		vwin->clearLine(line + 1);
	}

	vwin->clearScreen();
}

void Game::chooseClasses()
{
	Player* samurai = nullptr;
	Player* sorceress = nullptr;
	Player* gunslinger = nullptr;
	for (Player* p : playerParty)
	{
		if (p->getColorString().getString() == "Samurai")
		{
			samurai = p;
		}
		else if (p->getColorString().getString() == "Sorceress")
		{
			sorceress = p;
		}
		else if (p->getColorString().getString() == "Gunslinger")
		{
			gunslinger = p;
		}	
		else
		{
			ddutil::errorMessage("Invalid player construction", __LINE__, __FILE__);
		}
	}
	
	const int SLEEP = 250;
	const int TOP_LINE = 2;
	const int CHAR_LINE = ddutil::DIVIDER_LINE3 - 1;
	const int MENU_LINE = CHAR_LINE + 2;
	while (true)
	{
		vwin->clearScreen();
		vwin->printArtFromBottom(Art::getSamurai(samurai->getColor()), Coordinate(0, CHAR_LINE), true);
		vwin->putcen(ColorString("*================================*", ddutil::WHITE), TOP_LINE - 1);
		vwin->putcen(ColorString(" Choose Samurai Starting Artifact ", ddutil::SAMURAI_COLOR), TOP_LINE);
		vwin->putcen(ColorString("*================================*", ddutil::WHITE), TOP_LINE + 1);
		BeserkersBrew* brew = new BeserkersBrew(this);
		NomadsMat* mat = new NomadsMat(this);
		NinjasCaltrops* caltrops = new NinjasCaltrops(this);
		std::vector<ColorString> samOptions = {
			ColorString("View Compendium", ddutil::COMPENDIUM_COLOR),
			brew->getFullInformation(),
			mat->getFullInformation(),
			caltrops->getFullInformation()
		};
		Menu samMenu(vwin, samOptions, Coordinate(0, MENU_LINE), true);
		if (samMenu.getResponse() == 0)
		{
			compendium->display();
			continue;
		}

		switch (samMenu.getResponse())
		{
		case 1:
			samurai->getArtifact(brew);
			playSound(WavFile("dragonform", false, false));
			delete caltrops;
			delete mat;
			break;
		case 2:
			samurai->getArtifact(mat);
			playSound(WavFile("attack1", false, false));
			delete caltrops;
			delete brew;
			break;
		default: // 3
			samurai->getArtifact(caltrops);
			playSound(WavFile("gainblock", false, false));
			delete brew;
			delete mat;
			break;
		}
		Sleep(SLEEP);
		vwin->clearScreen();
		break;	
	}
	while (true)
	{
		vwin->clearScreen();
		vwin->putcen(ColorString("*==================================*", ddutil::WHITE), TOP_LINE - 1);
		vwin->putcen(ColorString(" Choose Sorceress Starting Artifact ", ddutil::SORCERER_COLOR), TOP_LINE);
		vwin->putcen(ColorString("*==================================*", ddutil::WHITE), TOP_LINE + 1);
		vwin->printArtFromBottom(Art::getSorcerer(sorceress->getColor()), Coordinate(0, CHAR_LINE), true);
		ClericsRobes* robes = new ClericsRobes(this);
		SummonersOrb* orb = new SummonersOrb(this);
		TempestsStaff* staff = new TempestsStaff(this);
		std::vector<ColorString> sorOptions = {
			ColorString("View Compendium", ddutil::COMPENDIUM_COLOR),
			robes->getFullInformation(),
			orb->getFullInformation(),
			staff->getFullInformation()
		};
		Menu sorMenu(vwin, sorOptions, Coordinate(0, MENU_LINE), true);
		if (sorMenu.getResponse() == 0)
		{
			compendium->display();
			continue;
		}
		switch (sorMenu.getResponse())
		{
		case 1:
			sorceress->getArtifact(robes);
			playSound(WavFile("heal", false, false));
			delete orb;
			delete staff;
			break;
		case 2:
			sorceress->getArtifact(orb);
			playSound(WavFile("magicspell2", false, false));
			delete robes;
			delete staff;
			break;
		default:
			sorceress->getArtifact(staff);
			playSound(WavFile("lightning", false, false));
			delete robes;
			delete orb;
			break;
		}
		Sleep(SLEEP);
		vwin->clearScreen();
		break;
	}
	while (true)
	{
		vwin->clearScreen();
		vwin->putcen(ColorString("*===================================*", ddutil::WHITE), TOP_LINE - 1);
		vwin->putcen(ColorString(" Choose Gunslinger Starting Artifact ", ddutil::GUNSLINGER_COLOR), TOP_LINE);
		vwin->putcen(ColorString("*===================================*", ddutil::WHITE), TOP_LINE + 1);
		vwin->printArtFromBottom(Art::getGunslinger(gunslinger->getColor()), Coordinate(0, CHAR_LINE), true);
		BrawlersBelt* belt = new BrawlersBelt(this);
		GamblersDeck* deck = new GamblersDeck(this);
		SharpshootersSack* sack = new SharpshootersSack(this);
		std::vector<ColorString> sorOptions = {
			ColorString("View Compendium", ddutil::COMPENDIUM_COLOR),
			belt->getFullInformation(),
			deck->getFullInformation(),
			sack->getFullInformation()
		};
		Menu sorMenu(vwin, sorOptions, Coordinate(0, MENU_LINE), true);
		if (sorMenu.getResponse() == 0)
		{
			compendium->display();
			continue;
		}
		switch (sorMenu.getResponse())
		{
		case 1:
			gunslinger->getArtifact(belt);
			playSound(WavFile("attack5", false, false));
			delete sack;
			delete deck;
			break;
		case 2:
			gunslinger->getArtifact(deck);
			playSound(WavFile("drawcard", false, false));
			delete sack;
			delete belt;
			break;
		default:
			gunslinger->getArtifact(sack);
			playSound(WavFile("shot3", false, false));
			playSound(WavFile("shot3", false, false));
			Sleep(25);
			playSound(WavFile("shot3", false, false));
			delete belt;
			delete deck;
			break;
		}
		Sleep(SLEEP * 2);
		vwin->clearScreen();
		break;
	}
	
	vwin->clearScreen();
}

void Game::displayZoneEntrance()
{
	ColorString zoneString = gameWorld[currentZoneIndex]->getZoneString();
	int zoneColor = zoneString[0].color;

	displayDividerString();

	int midwayLine = (0 + ddutil::DIVIDER_LINE3) / 2;

	ColorString title;
	if (currentZoneIndex + 1 < Zone::LAST_ZONE)
	{
		title = ColorString("Chapter " + std::to_string(currentZoneIndex + 1) + ":", zoneColor);
	}
	else
	{
		title = ColorString("FINAL CHAPTER:", zoneColor);
	}

	vwin->putcen(title, midwayLine - 1);
	vwin->putcen(zoneString, midwayLine);

	if (!loadedFromFile && currentZoneIndex == 0)
	{
		playSound(Mp3File("chapterbegin"));
		Sleep(6500);
		stopSound(SoundType::MP3);
	}

	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, midwayLine + 2), true);

	clearCenterScreen();
}

void Game::displayHelpScreen()
{
	int line = ddutil::EVENT_PICTURE_LINE;
	vwin->putcen(ColorString("TIP: If you are ever confused about a game mechanic, try reading the compendium.", ddutil::GREEN), line);
	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
}

void Game::printCreatureBattleInfo(Creature* creature, Coordinate bottomLeft, int textLine)
{
	vwin->printArtFromBottom(creature->getPicture(), bottomLeft, false);

	vwin->clearLine(textLine);
	vwin->clearLine(textLine + 1);

	vwin->putcen(creature->getStatLine(), textLine);
	vwin->putcen(creature->getStatusString(), textLine + 1);
}

void Game::doStatusDecrementionAndPrintInfo(Creature* creature)
{
	std::vector<ColorString> info = creature->decrementAllStatuses();

	//check to make sure this information is actually worth printing to the screen
	auto it = info.begin();
	while (it != info.end())
	{
		if ((*it).getString() == "")
		{
			it = info.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (info.empty())
	{
		return;
	}

	clearBottomDivider();
	int y = ddutil::BOTTOM_TEXT_LINE;
	for (ColorString cs : info)
	{
		vwin->putcen(cs, y++);
	}
	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, y++), true);
	clearBottomDivider();
}

void Game::displayStars()
{
	for (int i = 0; i < ddutil::CONSOLEX; i++)
	{
		for (int j = 0; j < ddutil::CONSOLEY; j++)
		{
			if (ddutil::random(1, 10) == 1)
			{
				vwin->put(ColorChar('*', ddutil::YELLOW), Coordinate(i, j));
			}
		}
	}
}

void Game::screenTransition(int color)
{
	ColorChar colChar(' ', ddutil::getColor(color, color));
	const int SEPARATION = 3;
	for (int k = 0; k < SEPARATION; k++)
	{
		for (int i = k; i < ddutil::CONSOLEX; i += SEPARATION)
		{
			for (int j = 0; j < ddutil::CONSOLEY; j++)
			{
				vwin->put(colChar, Coordinate(i, j));
			}
		}
		Sleep(600);
	}

}

// returns true if all players are dead
bool Game::checkForPlayerDeaths()
{
	// Check if any of the players are dead
	auto it = playerParty.begin();
	while (it != playerParty.end())
	{
		if ((*it)->getHealth() <= 0)
		{
			(*it)->deathScene();
			deadPlayers.push_back(*it);
			it = playerParty.erase(it);
		}
		else
		{
			++it;
		}
	}

	// assume only minions are alive. If we find a non minion, we know this is false and set the bool accordingly
	bool onlyMinionsAlive = true;
	for (Player* p : playerParty)
	{
		if (!p->isMinion())
		{
			onlyMinionsAlive = false;
			break;
		}
	}

	// Check to see if all of the players are dead or only minions are alive
	if (playerParty.empty() || onlyMinionsAlive)
	{
		stopSound(SoundType::MP3);
		gameOver = true;
		vwin->clearScreen(ddutil::RED);
		int centerLine = ddutil::CONSOLEY / 2;
		playSound(WavFile("demonscream", false, false));
		playSound(Mp3File("gameover"));
		vwin->put(ColorString("Score: " + std::to_string(getScore()), ddutil::getColor(ddutil::YELLOW, ddutil::RED)), Coordinate(3, 5));
		vwin->putcen(ColorString("Game Over!", ddutil::getColor(ddutil::WHITE, ddutil::RED)), centerLine);
		vwin->putcen(ColorString("Press enter to restart...", ddutil::getColor(ddutil::WHITE, ddutil::RED)), centerLine + 1);
		ddutil::waitForKeyPress(VK_RETURN);
		stopSound(SoundType::MP3);
		return true;
	}
	else
	{
		return false;
	}
}

// returns true if the enemy is dead, and awards Experience to players
bool Game::checkForEnemyDeath(Enemy* enemy)
{
	// Check to see if the enemy is dead
	if (enemy->getHealth() <= 0)
	{
		// remove temporary changes that occurred during the battle
		removeMinionsFromParty();
		for (Player* p : playerParty)
		{
			p->resetTempStatAdjustments();
		}

		stopSound(SoundType::MP3);

		enemy->deathScene();

		if (!gameWin)
		{
			// calculate experience gain
			unsigned int expGain = enemy->getMaxHealth(100) / playerParty.size();
			if (currentDistortion >= 4)
			{
				expGain -= static_cast<int>(expGain * (ddutil::DIST4_PERC_ENEMY_HP_INC / 100.0));
			}
			playersGetExperience(expGain);
		}

		for (Player* p : playerParty)
		{
			p->resetVitality();
			p->clearAllStatuses(); // make sure no statuses from previous battles are lingering
		}

		clearCenterScreen();

		currentEnemy = nullptr;
		return true;
	}
	else
	{
		return false;
	}
}

Player* Game::reviveFirstDeadMinion()
{
	for (unsigned i = 0; i < deadPlayers.size(); i++)
	{
		Player* p = deadPlayers.at(i);
		if (p->isMinion())
		{
			p->setHealthPercent(100);
			Player* minion = p;
			deadPlayers.erase(deadPlayers.begin() + i);
			playerParty.push_back(minion);
			return minion;
		}
	}
	return nullptr;
}

void Game::setGameWin()
{
	gameWin = true;
}

bool Game::getGameWin()
{
	return gameWin;
}

void Game::setSkipToNextChapter()
{
	skipToNextChapter = true;
}

bool Game::shouldSkipChapter()
{
	bool val = skipToNextChapter;
	skipToNextChapter = false;
	return val;
}

void Game::endingScene()
{
	Coordinate coord(0, 5);
	score = static_cast<int>(score * 1 + (currentDistortion / 10.0));
	if (currentDistortion == 5)
	{
		int line = 30;
		vwin->printArt(Art::getTheWatcher(), coord, true);
		int color = ddutil::MAGENTA;
		std::vector<ColorString> text = {
			ColorString("\"......\"", color),
			ColorString("\"Is it over?\"", color),
			ColorString("\"Is he finally gone?\"", color),
			ColorString("\"......\"", color),
		};
	
		for (ColorString c : text)
		{
			vwin->putcenSlowScroll(c, line);
			Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
			vwin->clearLine(line);
			vwin->clearLine(line + 1);
		}

		playSound(Mp3File("trueending"));
		vwin->put(ColorString("Score: " + std::to_string(getScore()), ddutil::YELLOW), Coordinate(3, coord.y));
		vwin->put(ColorString("Distortion: " + std::to_string(currentDistortion), ddutil::DISTORTION_COLOR), Coordinate(3, coord.y+1));
		int newLine = 30;

		vwin->putcenSlowScroll(ColorString("Thank you ", Art::WATCHER_COLOR) + getPartyNames(playerParty), newLine);
		vwin->putcenSlowScroll(ColorString("Your courage will always be remembered", Art::WATCHER_COLOR), newLine + 1);
		Sleep(2500);
		if (deadPlayers.empty())
		{
			vwin->putcenSlowScroll(ColorString("Fortunately, nobody died! Less work for me!", Art::WATCHER_COLOR), newLine + 2);
		}
		else
		{
			vwin->putcenSlowScroll(ColorString("And of course, we will not forget the sacrifices of ", Art::WATCHER_COLOR) +
				getPartyNames(deadPlayers), newLine + 2);
		}
		Sleep(4000);
		vwin->putcenSlowScroll(ColorString("When you're ready, I can send you all back to your own times... One last time", Art::WATCHER_COLOR), newLine + 3);
		Menu::oneOptionMenu(vwin, ColorString("Return to your own times", ddutil::TEXT_COLOR), Coordinate(0, newLine + 4), true);
		stopSound(SoundType::MP3);
		playSound(WavFile("enterportal", false, true));
		for (int i = 0; i < 4; i++)
		{
			vwin->clearScreen(ddutil::YELLOW);
			Sleep(100);
			vwin->clearScreen(ddutil::RED);
			Sleep(100);
			vwin->clearScreen(ddutil::BLUE);
			Sleep(100);
		}
		vwin->clearScreen(ddutil::BLACK);
		Sleep(1000);
		return;
	}
	
	playSound(Mp3File("ending"));	
	vwin->putcen(ColorString("Epilogue", Art::WATCHER_COLOR), ddutil::CONSOLEY / 2);

	Menu::oneOptionMenu(vwin, ColorString("Continue", Art::WATCHER_COLOR), Coordinate(0, ddutil::CONSOLEY / 2 + 1), true);

	vwin->printArt(Art::getTheWatcher(), coord, true);

	vwin->put(ColorString("Score: " + std::to_string(getScore()), ddutil::YELLOW), Coordinate(3, coord.y));
	vwin->put(ColorString("Distortion: " + std::to_string(currentDistortion), ddutil::DISTORTION_COLOR), Coordinate(3, coord.y+1));

	int newLine = 30;

	vwin->putcenSlowScroll(ColorString("Thank you ", Art::WATCHER_COLOR) + getPartyNames(playerParty), newLine);
	vwin->putcenSlowScroll(ColorString("Your courage will always be remembered", Art::WATCHER_COLOR), newLine + 1);
	Sleep(2500);
	if (deadPlayers.empty())
	{
		vwin->putcenSlowScroll(ColorString("Fortunately, nobody died! Less work for me!", Art::WATCHER_COLOR), newLine + 2);
	}
	else
	{
		vwin->putcenSlowScroll(ColorString("And of course, we will not forget the sacrifices of ", Art::WATCHER_COLOR) +
			getPartyNames(deadPlayers), newLine + 2);
	}
	Sleep(4000);
	vwin->putcenSlowScroll(ColorString("When you're ready, I can send you all back to your own times", Art::WATCHER_COLOR), newLine + 3);
	Menu::oneOptionMenu(vwin, ColorString("Return to your own times", ddutil::TEXT_COLOR), Coordinate(0, newLine + 4), true);
	
	stopSound(SoundType::MP3);
	playSound(WavFile("enterportal", false, true));
	for (int i = 0; i < 4; i++)
	{
		vwin->clearScreen(ddutil::YELLOW);
		Sleep(100);
		vwin->clearScreen(ddutil::RED);
		Sleep(100);
		vwin->clearScreen(ddutil::BLUE);
		Sleep(100);
	}
	vwin->clearScreen(ddutil::BLACK);
	Sleep(1000);

	int lastLine = ddutil::CONSOLEY / 2;
	vwin->putcenSlowScroll(ColorString("All evil will eventually be regenerated", Art::WATCHER_COLOR), lastLine, true);
	Menu::oneOptionMenu(vwin, ColorString("...", Art::WATCHER_COLOR), Coordinate(0, lastLine + 1), true);
	vwin->clearLine(lastLine); vwin->clearLine(lastLine + 1);
	vwin->putcenSlowScroll(ColorString("That is the nature of our world", Art::WATCHER_COLOR), lastLine, true);
	Menu::oneOptionMenu(vwin, ColorString("...", Art::WATCHER_COLOR), Coordinate(0, lastLine + 1), true);
	vwin->clearLine(lastLine); vwin->clearLine(lastLine + 1);
	vwin->putcenSlowScroll(ColorString("This is not the last time I will need to call upon Human bravery", Art::WATCHER_COLOR), lastLine, true);
	Menu::oneOptionMenu(vwin, ColorString("...", Art::WATCHER_COLOR), Coordinate(0, lastLine + 1), true);
	vwin->clearLine(lastLine); vwin->clearLine(lastLine + 1);
	vwin->putcenSlowScroll(ColorString("If only there were a way to destroy The Patriarch for good....", Art::WATCHER_COLOR), lastLine, true);
	Menu::oneOptionMenu(vwin, ColorString("...", Art::WATCHER_COLOR), Coordinate(0, lastLine + 1), true);
	vwin->clearLine(lastLine); vwin->clearLine(lastLine + 1);
	playSound(WavFile("demonscream", false, false));
	Sleep(1000);
}

void Game::changeScore(int amount)
{
	score += amount;
}

int Game::getScore()
{
	return score;
}

void Game::viewMenu()
{
	std::vector<ColorString> options = {
		ColorString("Exit", ddutil::TEXT_COLOR),
		ColorString("View Compendium", ddutil::COMPENDIUM_COLOR),
		ColorString("View Map", gameWorld[currentZoneIndex]->getZoneString().getColors().front()),
	};
	for (Player* p : playerParty)
	{
		options.push_back(p->getColorString() + ColorString(" Stats", ddutil::ARTIFACT_COLOR));
	}

	bool exit = false;
	while (!exit)
	{
		clearCenterScreen();
		clearBottomDivider();
		displayInfo();
		Menu menu(vwin, options, Coordinate(0, ddutil::EVENT_PICTURE_LINE), true);
		int menuResponse = menu.getResponse();
		switch (menuResponse)
		{
		case 0:
			exit = true;
			break;
		case 1:
			viewCompendium();
			break;
		case 2:
			gameWorld[currentZoneIndex]->displayMap(false);
			break;
		default: // corresponds to an index in the player party
			int index = menuResponse - 3;
			playerParty.at(index)->displayStatsMenu();
			break;
		}
	}	
	clearCenterScreen();
	clearBottomDivider();
}

void Game::viewCompendium()
{
	compendium->display();
	vwin->clearScreen();
	displayDividerString();
}

int Game::getGreenMoveCost()
{
	return greenMoveCost;
}

int Game::getBlueMoveCost()
{
	return blueMoveCost;
}

int Game::getRedMoveCost()
{
	return redMoveCost;
}

int Game::getCurrentDistortion()
{
	return currentDistortion;
}

void Game::setDistortionModifers()
{
	// do before distortion 1 because of the percentages
	if (currentDistortion >= 3)
	{
		for (Player* p : playerParty)
		{
			p->decreaseMaxHealth(ddutil::DIST3_MAXHP_MINUS);
		}
	}
	if (currentDistortion >= 1)
	{
		for (Player* p : playerParty)
		{
			p->setHealthPercent(ddutil::DIST1_HP_PERCENT);
		}
	}
	if (currentDistortion >= 2)
	{
		greenMoveCost = static_cast<int>(greenMoveCost * (1 + (ddutil::DIST2_MOVEXP_PERCENT / 100.0)));
		blueMoveCost = static_cast<int>(blueMoveCost * (1 + (ddutil::DIST2_MOVEXP_PERCENT / 100.0)));
		redMoveCost = static_cast<int>(redMoveCost * (1 + (ddutil::DIST2_MOVEXP_PERCENT / 100.0)));
	}
	if (currentDistortion >= 4)
	{
		// do nothing, handled in the intro function 
	}
	if (currentDistortion >= 5)
	{
		// TODO:
	}
}

void Game::displayDividerString()
{
	int zoneColor = gameWorld[currentZoneIndex]->getZoneString()[0].color;
	ColorString dividerString(std::string(ddutil::CONSOLEX, ddutil::DIVIDER_CHARACTER), zoneColor);

	vwin->put(dividerString, Coordinate(0, ddutil::DIVIDER_LINE3));
}

bool Game::canChooseAnyRoom()
{
	for (Player* p : playerParty)
	{
		if (p->hasArtifact(ArtifactID::WaxWings))
		{
			return true;
		}
	}
	return false;
}

void Game::removeMinionsFromParty()
{
	auto it = playerParty.begin();
	while (it != playerParty.end())
	{
		if ((*it)->isMinion())
		{
			delete (*it);
			it = playerParty.erase(it);
		}
		else
		{
			++it;
		}
	}

	auto it2 = deadPlayers.begin();
	while (it2 != deadPlayers.end())
	{
		if ((*it2)->isMinion())
		{
			delete (*it2);
			it2 = deadPlayers.erase(it2);
		}
		else
		{
			++it2;
		}
	}
}

void Game::clearBottomDivider()
{
	for (int i = ddutil::DIVIDER_LINE3 + 1; i <= ddutil::DIVIDER_LINE4; i++)
	{
		vwin->clearLine(i);
	}
}

void Game::clearCenterScreen()
{
	for (int i = 0; i < ddutil::DIVIDER_LINE3; i++)
	{
		vwin->clearLine(i);
	}
}

bool Game::isGameOver()
{
	return gameOver;
}

void Game::playersGetExperience(int amount)
{
	std::vector<int> expGainPerPlayer;
	for (Player* p : playerParty) // gain exp proportional to max HP of enemy
	{
		expGainPerPlayer.push_back(p->gainExperience(amount));
	}

	ColorString expInfo;
	for (unsigned int i = 0; i < expGainPerPlayer.size(); i++)
	{
		expInfo += (playerParty[i]->getColorString() + ColorString(": ", ddutil::TEXT_COLOR) + 
			ColorString("+"+std::to_string(expGainPerPlayer[i])+"XP", ddutil::EXPERIENCE_COLOR) + ColorString("; ", ddutil::TEXT_COLOR));
	}

	vwin->putcen(expInfo, ddutil::CENTER_TEXT_LINE);
	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, ddutil::CENTER_TEXT_LINE + 1), true);
}

Player* Game::selectPlayer(std::vector<ColorString> info, int startingLine, bool allowSkip)
{
	int originalLine = startingLine;
	for (ColorString cs : info)
	{
		vwin->putcenSlowScroll(cs, startingLine++);
	}

	std::vector<ColorString> options;
	for (Player* p : playerParty)
	{
		options.push_back(p->getColorString());
	}
	if (allowSkip)
	{
		options.push_back(ColorString("Skip", ddutil::TEXT_COLOR));
	}
	std::vector<int> otherOptions = { VK_MENU };

	while (true)
	{
		Menu menu(vwin, options, otherOptions, Coordinate(0, startingLine), true);
		int menuPosition = menu.getOtherInputResponse();
		int menuResponse = menu.getResponse();

		if (menuResponse == VK_MENU)
		{
			viewMenu();
			clearCenterScreen();
			clearBottomDivider();
			displayInfo();
			int redrawLine = originalLine;
			for (ColorString cs : info)
			{
				vwin->putcen(cs, redrawLine++);
			}
		}
		else
		{
			if (menu.getResponse() >= static_cast<int>(playerParty.size()) || menu.getResponse() < 0)
			{
				return nullptr;
			}
			else
			{
				return playerParty[menu.getResponse()];
			}
		}
		
	}
}

void Game::artifactSelectionMenu(int line, Artifact* artifact)
{
	std::vector<ColorString> info = {
		ColorString("Who should receive the ", ddutil::TEXT_COLOR) + artifact->getName() + ColorString("? (Alt: Menu)", ddutil::TEXT_COLOR),
			artifact->getFullInformation()
	};
	Player* player = selectPlayer(info, line, true);
	if (player == nullptr)
	{
		delete artifact;
	}
	else
	{
		player->getArtifact(artifact);
	}
}

Zone* Game::getActiveZone()
{
	return gameWorld[currentZoneIndex];
}

ColorString Game::healAllPlayers(int amount)
{
	ColorString output;
	for (Player* p : playerParty)
	{
		int actualIncrease = p->increaseHealth(amount);
		output += p->getColorString() + ColorString(": ", ddutil::TEXT_COLOR) +
			ColorString("+" + std::to_string(actualIncrease) + " health; ", ddutil::HEAL_COLOR);
	}
	return output;
}

// displays the party information in the bottom of the screen
void Game::displayInfo()
{
	int healthBarLength = 10;
	int line = ddutil::BOTTOM_TEXT_LINE;

	vwin->clearLine(line);
	vwin->putcen(ColorString("Distortion: " + std::to_string(currentDistortion), ddutil::DISTORTION_COLOR), line++);
	vwin->putcen(ColorString("- Name - Health - Vitality -", ddutil::WHITE), line++);
	for (auto& i : playerParty) // display the name, healthbar, health in number, and vitality information at the bottom of the screen
	{
		vwin->putcen(i->getStatLine(), line++);
	}
}

void Game::drawPlayerParty()
{
	int totalLength = 0;
	for (auto& i : playerParty)
	{
		totalLength += i->getPicture().getLength();
	}
	int xOffset = (ddutil::CONSOLEX - totalLength) / 2;

	int xCoord = xOffset;
	int yCoord = ddutil::DIVIDER_LINE3 - 4;
	for (auto& i : playerParty)
	{
		vwin->printArtFromBottom(i->getPicture(), Coordinate(xCoord, yCoord), false);
		xCoord += i->getPicture().getLength() + 1;
	}
}

void Game::roomEntranceAnimation()
{
	for (int i = 0; i < 3; i++)
	{
		playSound(WavFile("footsteps", false, false));
		Sleep(50);
	}
}

ColorString Game::getPartyNames(std::vector<Creature*> theParty)
{
	ColorString s = ColorString("The ", ddutil::TEXT_COLOR);
	for (unsigned int i = 0; i < theParty.size(); i++)
	{
		s += theParty[i]->getColorString();
		
		if (!(i == theParty.size() - 1)) // if it is NOT the last in the list
		{
			s += ColorString(", ", ddutil::TEXT_COLOR);
		}
		if (i == theParty.size() - 2) // If it is the second last in the list
		{
			s += ColorString("and ", ddutil::TEXT_COLOR);
		}
	}
	s += ColorChar(' ', ddutil::TEXT_COLOR);
	return s;
}

ColorString Game::getPartyNames(std::vector<Player*> theParty)
{
	std::vector<Creature*> newParty;
	for (Player* p : theParty)
	{
		newParty.push_back(p);
	}
	return getPartyNames(newParty);
}

