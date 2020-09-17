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

#include <Windows.h>
#include <string>
#include <algorithm>

Game::Game(VirtualWindow* virWin)
{
	score = 0;
	vwin = virWin;
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

	intro();
	stopSound(SoundType::MP3);

	for (; currentZoneIndex < gameWorld.size() && !gameOver; currentZoneIndex++)
	{
		displayZoneEntrance();
		
		Zone* activeZone = gameWorld[currentZoneIndex];

		while (activeZone->hasMoreRooms())
		{
			Room* currentRoom = activeZone->chooseRoom();

			displayInfo();
			drawPlayerParty();
			roomEntranceAnimation();

			currentRoom->playRoom();

			if (gameOver)
			{
				status = ddutil::GameStatus::RESTART;
				break;
			}
			if (gameWin || shouldSkipChapter())
			{
				break;
			}
			
			currentRoom->setCharEmpty();
		}
		if (gameWin)
		{
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
				if (checkForPlayerDeaths())
				{
					return;
				}
			}
			if (checkForEnemyDeath(enemy))
			{
				return;
			}
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
			const int END_TURN_LINE = ddutil::DIVIDER_LINE4;
			vwin->putcen(ColorString("Spacebar: end turn  Enter: select move  Alt: view artifacts", ddutil::CYAN), END_TURN_LINE);

			Menu moveMenu(vwin, options, otherInputs, Coordinate(0, MOVE_TEXT_LINE), true);

			bool endTurn = false;
			// getReponse will either be an low value integer corresponding to a move index, OR VK_LEFT/VK_RIGHT indicating character change
			switch (moveMenu.getResponse())
			{
			case VK_LEFT:
				activePlayerIndex--;
				if (activePlayerIndex < 0)
				{
					activePlayerIndex = playerParty.size() - 1;
				}
				break;
			case VK_RIGHT:
				activePlayerIndex++;
				if (activePlayerIndex >= static_cast<int>(playerParty.size()))
				{
					activePlayerIndex = 0;
				}
				break;
			case VK_SPACE:
				endTurn = true;
				break;
			case VK_MENU:
				playerParty[activePlayerIndex]->displayArtifacts();
				break;
			default: // corresponds to an index of the player's moves
				Move* selectedMove = activeMoves[moveMenu.getResponse()];

				if (activePlayer->hasStatus(StatusID::Hexed))
				{
					clearBottomDivider();
					vwin->putcen(
						ColorString("The ", ddutil::TEXT_COLOR) +
						activePlayer->getColorString() +
						ColorString(" is ", ddutil::TEXT_COLOR) +
						ColorString("Hexed", HexedStatus::COLOR) + 
						ColorString(" for this turn", ddutil::TEXT_COLOR),
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
						break;
					}
					clearBottomDivider();


					ColorString info = selectedMove->doAction(activePlayer, target);
					usedMoves.push_back(selectedMove);

					clearBottomDivider();
					selectedMove->playSoundEffect();
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
	playerParty.push_back(newPlayer);
}

std::vector<Player*> Game::getPlayerParty()
{
	return playerParty;
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

	vwin->printArt(Art::getTitle(), Coordinate(0, 5), true);

	std::vector<ColorString> options;
	options.push_back(ColorString("Press Enter to start", ddutil::TEXT_COLOR));
	//options.push_back(ColorString("Quit", ddutil::TEXT_COLOR));

	std::vector<int> otherInput = { VK_ESCAPE };
	Menu menu(vwin, options, otherInput, Coordinate(0, 25), true);
	int response = menu.getResponse();

	switch (response)
	{
	case 0: // start
		status = ddutil::GameStatus::CONTINUE;
		break;
	case VK_ESCAPE: // quit
		status = ddutil::GameStatus::EXIT;
		break;
	}

	vwin->clearScreen();
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
		ColorString("\"Please take my blessing.\"", color),
	};
	
	for (ColorString c : text)
	{
		vwin->putcenSlowScroll(c, line);
		Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
		vwin->clearLine(line);
		vwin->clearLine(line + 1);
	}

	vwin->putcen(ColorString("Choose your blessing:", Art::WATCHER_COLOR), line++);
	
	const int EXPERIENCE_GAIN = 20;

	std::vector<ColorString> options = {
		ColorString("Each gain "+std::to_string(EXPERIENCE_GAIN)+" experience", ddutil::EXPERIENCE_COLOR),
		ColorString("Obtain one random moderate artifact", ddutil::ARTIFACT_COLOR)
	};

	Menu menu(vwin, options, Coordinate(0, line), true);

	if (menu.getResponse() == 0)
	{
		for (Player* p : playerParty)
		{
			p->gainExperience(EXPERIENCE_GAIN);
		}
	}
	else
	{
		clearBottomDivider();
		artifactSelectionMenu(line, gameWorld[currentZoneIndex]->getRandomArtifact(Strength::Moderate));
	}

	vwin->clearScreen();
}

void Game::displayZoneEntrance()
{
	ColorString zoneString = gameWorld[currentZoneIndex]->getZoneString();
	int zoneColor = zoneString[0].color;

	ColorString dividerString(std::string(ddutil::CONSOLEX, ddutil::DIVIDER_CHARACTER), zoneColor);

	vwin->put(dividerString, Coordinate(0, ddutil::DIVIDER_LINE3));
	//vwin->put(dividerString, Coordinate(0, ddutil::DIVIDER_LINE4));

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

	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, midwayLine + 2), true);

	clearCenterScreen();
}

void Game::displayHelpScreen()
{
	// TODO
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
		vwin->put(ColorString("Score: " + std::to_string(getScore()), ddutil::YELLOW), Coordinate(3, 5));
		vwin->putcen(ColorString("Game Over!", ddutil::getColor(ddutil::WHITE, ddutil::RED)), centerLine);
		vwin->putcen(ColorString("[Restart]", ddutil::getColor(ddutil::WHITE, ddutil::RED)), centerLine + 1);
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
	playSound(Mp3File("ending"));

	vwin->putcen(ColorString("Epilogue", Art::WATCHER_COLOR), ddutil::CONSOLEY / 2);

	Menu::oneOptionMenu(vwin, ColorString("Continue", Art::WATCHER_COLOR), Coordinate(0, ddutil::CONSOLEY / 2 + 1), true);

	vwin->printArt(Art::getTheWatcher(), coord, true);

	vwin->put(ColorString("Score: " + std::to_string(getScore()), ddutil::YELLOW), Coordinate(3, coord.y));

	int newLine = 30;

	vwin->putcenSlowScroll(ColorString("Thank you ", Art::WATCHER_COLOR) + getPartyNames(playerParty), newLine);
	vwin->putcenSlowScroll(ColorString("Your courage will always be remembered", Art::WATCHER_COLOR), newLine + 1);
	Sleep(2500);
	if (deadPlayers.empty())
	{
		vwin->putcenSlowScroll(ColorString("Fortunately, nobody died! That is truely incredible.", Art::WATCHER_COLOR), newLine + 2);
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
	for (int i = 0; i < 5; i++)
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

Player* Game::selectPlayer(int startingLine)
{
	std::vector<ColorString> options;
	for (Player* p : playerParty)
	{
		options.push_back(p->getColorString());
	}

	Menu menu(vwin, options, Coordinate(0, startingLine), true);
	
	return playerParty[menu.getResponse()];
}

void Game::artifactSelectionMenu(int line, Artifact* artifact)
{
	vwin->putcen(ColorString("Who should receive the ", ddutil::TEXT_COLOR) +
		artifact->getName() + ColorString("?", ddutil::TEXT_COLOR), line++);
	vwin->putcen(artifact->getFullInformation(), line++);
	selectPlayer(line)->getArtifact(artifact);
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

