#include "player.h"
#include "creature.h"
#include "move.h"
#include "gunslinger_moves.h"
#include "sorcerer_moves.h"
#include "art.h"
#include "minion_moves.h"
#include "samurai_moves.h"
#include "game.h"
#include "virtualwindow.h"
#include "menu.h"
#include "artifact.h"
#include "enemy.h"
#include "cosmic_moves.h"
#include <string>
#include <algorithm>
#include <sstream>
// Player

Player::Player(Game* game, PlayerId id, int svit, int maxVit, int vitGain, int maxHp, int moveLim, std::string name, int color, Picture pic, bool min)
	:Creature(maxHp, name, color, pic, game)
{
	startingVitality = svit;
	vitality = svit;
	maxVitality = maxVit;
	vitalityGain = vitGain;
	movesetLimit = moveLim;
	experience = 0;
	minion = min;
	vitalityGainAdjustment = 0;
	percentXPBoost = 0;
	movesToChooseFrom = ddutil::START_MOVES_TO_CHOOSE_FROM;
	this->id = id;
	shouldReceiveXP = true;
	healthbarLength = ddutil::PLAYER_HEALTHBAR_LENGTH;
}

Player::~Player()
{
	for (Artifact* a : artifacts)
	{
		if (a != nullptr)
		{
			delete a;
		}
	}

	for (unsigned int i = 0; i < selfStartingStatuses.size(); i++)
	{
		delete selfStartingStatuses[i].first;
	}
	for (unsigned int i = 0; i < enemyStartingStatuses.size(); i++)
	{
		delete enemyStartingStatuses[i].first;
	}
}

Player* Player::getPlayerFromSavechunk(Game* game, PlayerId type, Savechunk chunk)
{
	int i = 0; // index to iterate throughout the chunk
	Player* player = nullptr;
	if (type == PlayerId::Gunslinger)
	{
		player = new Gunslinger(game);
		int reserveBullets = std::stoi(chunk.at(i++));
		int maxBullets = std::stoi(chunk.at(i++));
		Gunslinger* gunslinger = dynamic_cast<Gunslinger*>(player);
		gunslinger->setReserveBullets(reserveBullets);
		gunslinger->setMaxBullets(maxBullets);
	}
	else if (type == PlayerId::Sorceress)
	{
		player = new Sorcerer(game);
	}
	else if (type == PlayerId::Samurai)
	{
		player = new Samurai(game);
	}
	else
	{
		throw std::exception("Invalid player id when loading from save");
	}

	// Now go through the savechunk and set all the values appropriately

	player->health = std::stoi(chunk.at(i++));
	player->maxHealth = std::stoi(chunk.at(i++));
	player->percentHealBoost = std::stoi(chunk.at(i++));
	player->immuneToStatuses = std::stoi(chunk.at(i++));
	player->baseBlock = std::stoi(chunk.at(i++));
	player->name = chunk.at(i++);
	// Moves 
	if (chunk.at(i++) != "MOVES START")
	{
		throw std::exception("Invalid save file format");
	}
	// Delete all the default moves and replace with the moves stored in the save file
	for (Move* m : player->moves)
	{
		delete m;
	}
	player->moves.clear();
	// Go through and add move depending on the ID
	while (chunk.at(i) != "MOVES END")
	{
		MoveId id = static_cast<MoveId>(std::stoi(chunk.at(i++)));
		Move* newMove = Move::getMoveFromId(id, game);
		if (newMove != nullptr)
		{
			player->moves.push_back(newMove);
		}
		else
		{
			throw std::exception("Invalid move ID in save file");
		}
	}
	i++; // get it off of the MOVES END marker
	player->color = std::stoi(chunk.at(i++));
	player->baseDodgeChance = std::stoi(chunk.at(i++));
	// Attack Statuses
	if (chunk.at(i++) != "ATTACK STATUSES START")
	{
		throw std::exception("Invalid status ID in save file");
	}
	// the player's attack statuses will be empty because we are modifying a default character, so no need to delete anything
	while (chunk.at(i) != "ATTACK STATUSES END")
	{	
		std::stringstream ss(chunk.at(i++));
		std::string buff;
		std::vector<std::string> tokens;
		// first index is status id
		// second index is amount of status
		while (std::getline(ss, buff, ' '))
		{
			tokens.push_back(buff);
		}
		if (tokens.size() != 2)
		{
			throw std::exception("Invalid attack status format in save file");
		}
		StatusID statusID = static_cast<StatusID>(std::stoi(tokens.front()));
		int statusAmount = std::stoi(tokens.back());
		Status* newStatus = Status::getStatusFromID(statusID);
		if (newStatus != nullptr)
		{
			player->attackStatuses.push_back(std::make_pair(newStatus, statusAmount));
		}
		else
		{
			throw std::exception("Invalid attack status format in save file");
		}
	}
	i++; // get it off of ATTACK STATUSES END MARKER
	// Artifacts
	if (chunk.at(i++) != "ARTIFACTS START")
	{
		throw std::exception("Invalid save file format");
	}
	if (!player->artifacts.empty())
	{
		for (Artifact* a : player->artifacts)
		{
			delete a;
		}
		player->artifacts.clear();
	}
	while (chunk.at(i) != "ARTIFACTS END")
	{
		std::stringstream ss(chunk.at(i++));
		std::string buff;
		std::vector<std::string> tokens;
		// first index is artifact id
		// second index is consumed value
		while (std::getline(ss, buff, ' '))
		{
			tokens.push_back(buff);
		}
		if (tokens.size() != 2 && tokens.size() != 1)
		{
			throw std::exception("Invalid save file format");
		}
		ArtifactID id = static_cast<ArtifactID>(std::stoi(tokens.front()));

		Artifact* artifact = Artifact::getArtifactFromID(game, id);

		if (artifact != nullptr)
		{
			if (tokens.size() == 2)
			{
				bool consumed = std::stoi(tokens.back());
				if (consumed)
				{
					artifact->setAsConsumed();
				}
			} // dont do anything extra if not consumed
			player->artifacts.push_back(artifact);
			// NOT the get artifact function because that would apply the artifact's equip effect again, and that has already
			// been taken care of in the player's stats themselves
		}
		else
		{
			throw std::exception("Invalid artifact ID in save file");
		}
	}
	i++; // get it off of ARTIFACTS END
	player->movesToChooseFrom = std::stoi(chunk.at(i++));
	if (chunk.at(i++) != "SELF STARTING STATUSES START")
	{
		throw std::exception("Invalid save file format");
	}
	while (chunk.at(i) != "SELF STARTING STATUSES END")
	{	
		std::stringstream ss(chunk.at(i++));
		std::string buff;
		std::vector<std::string> tokens;
		// first index is status id
		// second index is amount of status
		while (std::getline(ss, buff, ' '))
		{
			tokens.push_back(buff);
		}
		if (tokens.size() != 2)
		{
			throw std::exception("Invalid save file format");
		}

		StatusID statusID = static_cast<StatusID>(std::stoi(tokens.front()));
		int statusAmount = std::stoi(tokens.back());
		Status* newStatus = Status::getStatusFromID(statusID);
		if (newStatus != nullptr)
		{
			player->selfStartingStatuses.push_back(std::make_pair(newStatus, statusAmount));
		}
		else
		{
			throw std::exception("Invalid save file format");
		}
	}
	i++; // get it off of SELF STARTING STATUSES END
	if (chunk.at(i++) != "ENEMY STARTING STATUSES START")
	{
		throw std::exception("Invalid save file format");
	}
	while (chunk.at(i) != "ENEMY STARTING STATUSES END")
	{
		std::stringstream ss(chunk.at(i++));
		std::string buff;
		std::vector<std::string> tokens;
		// first index is status id
		// second index is amount of status
		while (std::getline(ss, buff, ' '))
		{
			tokens.push_back(buff);
		}
		if (tokens.size() != 2)
		{
			throw std::exception("Invalid save file format");
		}
		StatusID statusID = static_cast<StatusID>(std::stoi(tokens.front()));
		int statusAmount = std::stoi(tokens.back());
		Status* newStatus = Status::getStatusFromID(statusID);
		if (newStatus != nullptr)
		{
			player->enemyStartingStatuses.push_back(std::make_pair(newStatus, statusAmount));
		}
		else
		{
			throw std::exception("Invalid save file format");
		}
	}
	i++; // get it off of ENEMY STARTING STATUSES END
	player->startingVitality = std::stoi(chunk.at(i++));
	player->maxVitality = std::stoi(chunk.at(i++));
	player->vitalityGain = std::stoi(chunk.at(i++));
	player->experience = std::stoi(chunk.at(i++));
	player->movesetLimit = std::stoi(chunk.at(i++));
	player->percentXPBoost = std::stoi(chunk.at(i++));
	return player;
}


void Player::tradeExperience()
{
	// Clear the screen
	Game* game = getGamePtr();
	VirtualWindow* vwin = game->getVWin();

	
	Strength str;
	bool successfulSelection = false;
	do // repeat this loop until the player has selected an option that they have enough experience for, or until they choose to save experience
	{
		game->clearCenterScreen();
		game->clearBottomDivider();
		Coordinate menuCoord = Coordinate((ddutil::CONSOLEX * 2) / 3, ddutil::DIVIDER_LINE3 / 2);
		Coordinate baseCoord = Coordinate(ddutil::CONSOLEX / 15, ddutil::DIVIDER_LINE3 - 3); // base of the player art

		// Print the character's art and stats
		vwin->printArtFromBottom(getPicture(), baseCoord, false);
		vwin->putcen(getStatLine(), ddutil::BOTTOM_TEXT_LINE);
		int movesLine = ddutil::BOTTOM_TEXT_LINE + 1;
		for (Move* m : moves)
		{
			vwin->putcen(m->getFullInformation(), movesLine++);
		}

		// set up the Menu to choose what type of move to learn
		vwin->put(ColorString("Spend XP to view " + std::to_string(movesToChooseFrom) + " new moves?", ddutil::TEXT_COLOR), menuCoord);
		menuCoord.y++;
		std::vector<ColorString> options;
		bool expFail = false;
		if (isCosmic())
		{
			options.push_back(
				ColorString("(" + std::to_string(game->getRedMoveCost()) + " XP) ", ddutil::EXPERIENCE_COLOR) +
				ColorString("Cosmic Strength", ddutil::COSMIC_COLOR)
			);
			options.push_back(ColorString("View Menu", ddutil::TEXT_COLOR));
			options.push_back(ColorString("Save Experience", ddutil::BROWN));

			Menu moveTypeMenu(vwin, options, menuCoord, false);
			menuCoord.y--;
			// interpret the player input
			switch (moveTypeMenu.getResponse())
			{
			case 0:
				if (loseExperience(game->getRedMoveCost()))
				{
					game->changeScore(ddutil::MYTH_MOVE_SCORE);
					str = Strength::Cosmic;
					successfulSelection = true;
				}
				else
				{
					expFail = true;
				}
				break;
			case 2: // save experience
				game->clearCenterScreen();
				game->clearBottomDivider();
				return; // just leave because theres nothing else to do here, no need to navigate out of the loop
			case 1:
			default:// view menu 
				game->viewMenu();
				break;
			}
		}
		else
		{
			options.push_back(
				ColorString("(" + std::to_string(game->getGreenMoveCost()) + " XP) ", ddutil::EXPERIENCE_COLOR) +
				ColorString("Moderate Strength", ddutil::MODERATE_COLOR)
			);
			options.push_back(
				ColorString("(" + std::to_string(game->getBlueMoveCost()) + " XP) ", ddutil::EXPERIENCE_COLOR) +
				ColorString("Powerful Strength", ddutil::POWERFUL_COLOR)
			);
			options.push_back(
				ColorString("(" + std::to_string(game->getRedMoveCost()) + " XP) ", ddutil::EXPERIENCE_COLOR) +
				ColorString("Mythical Strength", ddutil::MYTHICAL_COLOR)
			);
			options.push_back(ColorString("View Menu", ddutil::TEXT_COLOR));
			options.push_back(ColorString("Save Experience", ddutil::BROWN));
			Menu moveTypeMenu(vwin, options, menuCoord, false);
			menuCoord.y--;
			// interpret the player input
			switch (moveTypeMenu.getResponse())
			{
			case 0:
				if (loseExperience(game->getGreenMoveCost()))
				{
					game->changeScore(ddutil::MOD_MOVE_SCORE);
					str = Strength::Moderate;
					successfulSelection = true;
				}
				else
				{
					expFail = true;
				}
				break;
			case 1:
				if (loseExperience(game->getBlueMoveCost()))
				{
					game->changeScore(ddutil::POW_MOVE_SCORE);
					str = Strength::Powerful;
					successfulSelection = true;
				}
				else
				{
					expFail = true;
				}
				break;
			case 2:
				if (loseExperience(game->getRedMoveCost()))
				{
					game->changeScore(ddutil::MYTH_MOVE_SCORE);
					str = Strength::Mythical;
					successfulSelection = true;
				}
				else
				{
					expFail = true;
				}
				break;
			case 4: // save experience
				game->clearCenterScreen();
				game->clearBottomDivider();
				return; // just leave because theres nothing else to do here, no need to navigate out of the loop
			case 3:
			default:// view menu 
				game->viewMenu();
				break;
			}
		}

		if (!successfulSelection && expFail)
		{
			vwin->putcen(ColorString("Not enough ", ddutil::TEXT_COLOR) + ColorString("Experience!", ddutil::EXPERIENCE_COLOR), ddutil::CENTER_TEXT_LINE);
			Menu::oneOptionMenu(vwin, ColorString("Select Again", ddutil::TEXT_COLOR), Coordinate(0, ddutil::CENTER_TEXT_LINE + 1), true);
		}
	} 
	while (!successfulSelection);

	// generate the moves to select from based upon the player input
	std::vector<Move*> randomMoves;
	if (isCosmic())
	{
		randomMoves = CosmicMoves::getRandomMoves(movesToChooseFrom);
	}
	else
	{
		randomMoves	= getRandomMoves(str);
	}

	bool exit = false;
	while (!exit)
	{
		// Print the character's art and (updated) stats
		game->clearCenterScreen();
		game->clearBottomDivider();

		// Print the player's current moves in the bottom divider
		int currentMovesLine = ddutil::BOTTOM_TEXT_LINE;
		ColorString moveSetLimit = ColorString("(" + std::to_string(moves.size()) + "/" + std::to_string(movesetLimit) + ")", ddutil::TEXT_COLOR);
		vwin->putcen(moveSetLimit + ColorString(" Current Moves:", ddutil::EXPERIENCE_COLOR), currentMovesLine++);
		for (Move* m : getMoves())
		{
			vwin->putcen(m->getFullInformation(), currentMovesLine++);
		}

		// prepare the colorstrings to represent the possible moves that the player can add to their moveset
		std::vector<ColorString> moveSelectionOptions;
		moveSelectionOptions.push_back(ColorString("Skip (still lose XP)", ddutil::BROWN));
		moveSelectionOptions.push_back(ColorString("View Menu", ddutil::TEXT_COLOR));
		for (Move* m : randomMoves)
		{
			moveSelectionOptions.push_back(m->getFullInformation());
		}
		// an element in moveSelectionOptions at index i will correspond to index i - 2 in randomMoves

		// Display menu and get user input
		Coordinate baseMenuCoord = Coordinate(0, ddutil::DIVIDER_LINE3 / 2);
		Coordinate newMoveMenuCoord = baseMenuCoord;
		vwin->putcen(ColorString("Choose one new move:", ddutil::EXPERIENCE_COLOR), newMoveMenuCoord.y++);
		Menu newMoveSelectionMenu(vwin, moveSelectionOptions, newMoveMenuCoord, true);
		
		

		// interpret the user input
		int userInput = newMoveSelectionMenu.getResponse();
		if (userInput > 1) // the player chose one of the moves, and userInput is one greater than the index of the correct move in the randomMoves vector
		{
			game->clearCenterScreen();
			game->clearBottomDivider();

			int newMoveIndex = userInput - 2;

			Move* newMove = randomMoves[newMoveIndex];

			// remove selected move from the random moves vector so that it is not deleted afterwards
			auto selectedMoveIt = randomMoves.begin() + newMoveIndex;
			randomMoves.erase(selectedMoveIt);

			if (moves.size() >= static_cast<unsigned int>(movesetLimit))
			{

				// display new move in the bottom of the screen
				vwin->putcen(ColorString("New Move:", ddutil::TEXT_COLOR), ddutil::BOTTOM_TEXT_LINE);
				vwin->putcen(newMove->getFullInformation(), ddutil::BOTTOM_TEXT_LINE + 1);

				// display the old moves in the center of the screen and have the player choose one to remove in order to make room for the new move
				Coordinate oldMoveMenuCoord = baseMenuCoord;
				vwin->putcen(ColorString("Choose move to remove:", ddutil::EXPERIENCE_COLOR), oldMoveMenuCoord.y++);
				std::vector<ColorString> oldMoveOptions;
				for (Move* m : getMoves())
				{
					oldMoveOptions.push_back(m->getFullInformation());
				}

				Menu oldMoveMenu(vwin, oldMoveOptions, oldMoveMenuCoord, true);
				// the input from the menu will equal the index of the old move that should be removed
				int removeIndex = oldMoveMenu.getResponse();


				// remove the old move from the player's moves
				Move* moveToDelete = moves[removeIndex];
				auto eraseIt = moves.begin() + removeIndex;
				moves.erase(eraseIt);
				delete moveToDelete;
			}
			

			// add the new move into the player's moves
			moves.push_back(newMove);

			exit = true;
		}
		else if (userInput == 0) // skip
		{
			exit = true;
		}
		else if (userInput == 1) // the user wants to view the menu
		{
			game->viewMenu();
		}
	}

	game->clearCenterScreen();
	game->clearBottomDivider();

	// Delete all of the moves that were not selected
	auto it = randomMoves.begin();
	while (it != randomMoves.end())
	{
		delete (*it);
		it = randomMoves.erase(it);
	}
	
}

void Player::displayStatsMenu()
{	
	VirtualWindow* vwin = game->getVWin();
	bool exit = false;
	game->clearCenterScreen();
	game->clearBottomDivider();
	while (!exit)
	{
		
		vwin->printArtFromBottom(getPicture(), Coordinate(2, ddutil::DIVIDER_LINE3-2), false);
		int titleLine = 2;
		vwin->putcen(ColorString("The ", color) + getColorString(), titleLine);
		vwin->putcen(ColorString("SPACE: return   RIGHT: view artifacts", ddutil::TEXT_COLOR), titleLine+1);
		int statLine = ddutil::DIVIDER_LINE3 + 1;
		vwin->putcen(getStatLine(), statLine);
		int movesLine = statLine + 2;
		for (Move* m : moves)
		{
			vwin->putcen(m->getFullInformation(), movesLine++);
		}
		int longStatsColumn = ddutil::CONSOLEX / 2;
		int longStatsLine = 8;
		vwin->put(ColorString("HP: " + std::to_string(getHealth()), ddutil::HEAL_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("MAX HP: " + std::to_string(getMaxHealth(100)), ddutil::HEAL_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("HEAL %: " + std::to_string(getPercentHealBoost()), ddutil::HEAL_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("", ddutil::BLOCK_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("BASE BLOCK: " + std::to_string(getBaseBlock()), ddutil::BLOCK_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("DODGE %: " + std::to_string(getDodgeChance()), ddutil::TEXT_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("", ddutil::BLOCK_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("START VIT: " + std::to_string(getStartingVitality()), ddutil::VITALITY_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("VIT GAIN: " + std::to_string(getVitalityGain()), ddutil::VITALITY_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("MAX VIT: " + std::to_string(getMaxVitality()), ddutil::VITALITY_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("", ddutil::BLOCK_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("EXP: " + std::to_string(getExperience()), ddutil::EXPERIENCE_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("EXP %: " + std::to_string(percentXPBoost), ddutil::EXPERIENCE_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("MOVE LIMIT: " + std::to_string(movesetLimit), ddutil::TEXT_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		vwin->put(ColorString("MOVE SELECT: " + std::to_string(movesToChooseFrom), ddutil::TEXT_COLOR), Coordinate(longStatsColumn, longStatsLine++));
		while (true)
		{
			if (ddutil::keypress(VK_SPACE))
			{
				exit = true;
				break;
			}
			else if (ddutil::keypress(VK_RIGHT))
			{	
				playSound(WavFile("menuselect", false, true));
				game->clearCenterScreen();
				game->clearBottomDivider();
				displayArtifacts();
				break;
			}
		}
		playSound(WavFile("menuselect", false, true));
		game->clearCenterScreen();
		game->clearBottomDivider();
	}
}

void Player::displayArtifacts()
{
	VirtualWindow* vwin = game->getVWin();
	game->clearCenterScreen();
	game->clearBottomDivider();

	std::sort(artifacts.begin(), artifacts.end(), [](Artifact* a, Artifact* b) {
		return *a < *b;
		});

	int titleLine = 2;
	vwin->putcen(
		ColorString("The ", ddutil::TEXT_COLOR) + 
		getColorString() + ColorString("'s ", ddutil::TEXT_COLOR) + 
		ColorString("Artifacts", ddutil::ARTIFACT_COLOR),
		titleLine
	);
	vwin->putcen(ColorString("LEFT: view stats", ddutil::TEXT_COLOR), titleLine+1);
	int artifactLine = titleLine + 3;
	for (auto a : artifacts)
	{
		vwin->putcen(a->getFullInformation(), artifactLine++);
	}
	ddutil::waitForKeyPress(VK_LEFT);
	game->clearCenterScreen();
	game->clearBottomDivider();
}

void Player::setVitality(int amount)
{
	vitality = amount;
	if (vitality < 0)
	{
		vitality = 0;
	}
	if (vitality > maxVitality)
	{
		vitality = maxVitality;
	}
}

void Player::addVitality(int amount)
{
	vitality += amount;
	if (vitality > maxVitality)
	{
		vitality = maxVitality;
	}
}

bool Player::subVitality(int amount)
{
	if (vitality >= amount)
	{
		vitality -= amount;
		return true;
	}
	else
	{
		return false;
	}
}

int Player::stealVitality(int amount)
{
	int startingVit = vitality;
	vitality -= amount;
	if (vitality < 0)
	{
		vitality = 0;
	}
	return startingVit - vitality;
}

void Player::resetVitality()
{
	vitality = startingVitality;
}

int Player::getStartingVitality()
{
	return startingVitality;
}

int Player::getVitality()
{
	return vitality;
}

int Player::getMaxVitality()
{
	return maxVitality;
}

int Player::getVitalityGain()
{
	return vitalityGain + vitalityGainAdjustment;
}

int Player::getExperience()
{
	return experience;
}

void Player::adjustVitalityGainTemp(int amount)
{
	vitalityGainAdjustment += amount;
}

void Player::resetTempStatAdjustments()
{
	vitalityGainAdjustment = 0;
	resetPicture();
}

int Player::gainExperience(double amount)
{
	if (!shouldReceiveXP)
	{
		shouldReceiveXP = true;
		return 0;
	}

	amount *= 1 + (percentXPBoost / 100.0);
	experience += static_cast<int>(amount);
	return static_cast<int>(amount);
}

bool Player::loseExperience(int amount)
{
	if (amount <= experience)
	{
		experience -= amount;
		return true;
	}
	else
	{
		return false;
	}
}

ColorString Player::getStatLine()
{
	ColorString divideChar = ColorString(" ", ddutil::WHITE);
	ColorString c = Creature::getStatLine();

	ColorString vitality = ColorString(std::to_string(getVitality()) + "/+" + std::to_string(getVitalityGain())
		+ "/" + std::to_string(getMaxVitality()), ddutil::VITALITY_COLOR);

	ColorString expString = ColorString("(" + std::to_string(experience) + " XP)", ddutil::EXPERIENCE_COLOR);

	return c + vitality + divideChar + expString + divideChar;
}

bool Player::isMinion()
{
	return minion;
}

bool Player::hasArtifact(ArtifactID id)
{
	bool hasArtifact = false;
	for (Artifact* a : artifacts)
	{
		if (id == a->getID())
		{
			hasArtifact = true;
			break;
		}
	}
	return hasArtifact;
}

std::vector<ColorString> Player::getArtifactList()
{
	std::vector<ColorString> output;
	for (Artifact* a : artifacts)
	{
		output.push_back(a->getFullInformation());
	}
	return output;
}

void Player::getArtifact(Artifact* artifact)
{
	artifacts.push_back(artifact);
	artifact->equipAction(this);
}

int Player::getNumofArtifacts()
{
	return artifacts.size();
}

void Player::increasePercentXPBoost(int amount)
{
	percentXPBoost += amount;
}

void Player::decreasePercentXPBoost(int amount)
{
	percentXPBoost -= amount;
	if (percentXPBoost < -100)
	{
		percentXPBoost = -100;
	}
}

void Player::increaseStartingVitality(int amount)
{
	startingVitality += amount;
	if (startingVitality > maxVitality)
	{
		startingVitality = maxVitality;
	}
}

void Player::increaseVitalityPerTurn(int amount)
{
	vitalityGain += amount;
}

void Player::applyStartingStatuses(Enemy* enemy)
{
	for (auto s : selfStartingStatuses)
	{
		applyStatus(s.first->makeCopy(), s.second);
	}

	for (auto s : enemyStartingStatuses)
	{
		enemy->applyStatus(s.first->makeCopy(), s.second);
	}
}

void Player::addSelfStartingStatus(Status* stat, int amount)
{
	selfStartingStatuses.push_back(std::make_pair(stat, amount));
}

void Player::addEnemyStartingStatus(Status* stat, int amount)
{
	enemyStartingStatuses.push_back(std::make_pair(stat, amount));
}

void Player::increaseMovesetLimit(int amount)
{
	movesetLimit += amount;
	if (movesetLimit > ddutil::ABSOLUTE_MAX_MOVES)
	{
		movesetLimit = ddutil::ABSOLUTE_MAX_MOVES;
	}
}

void Player::deathScene()
{
	VirtualWindow* vwin = game->getVWin();
	game->clearCenterScreen();
	game->clearBottomDivider();

	Coordinate coord = Coordinate(0, ddutil::CENTER_TEXT_LINE - 2);

	playSound(WavFile("playerdie", false, true));
	for (int i = 0; i < 10; i++)
	{
		vwin->printArtFromBottom(picture, coord, true);
		Sleep(150);
		game->clearCenterScreen();
		Sleep(150);
	}


	ColorString message = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" has been slain", ddutil::TEXT_COLOR);
	message += ColorString("....", ddutil::TEXT_COLOR);

	vwin->putcen(message, coord.y);
	coord.y++;
	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), coord, true);

	game->clearCenterScreen();
}

void Player::increaseMovesToChooseFrom(int amount)
{
	movesToChooseFrom += amount;
}

Savechunk Player::makeSaveChunk()
{
	Savechunk chunk;
	chunk.add("PLAYER START");
	chunk.add(static_cast<int>(id));
	Savechunk uniqueChunk = getUniqueSaveChunkInfo();
	if (!uniqueChunk.empty())
	{
		chunk.add(uniqueChunk);
	}
	chunk.add(getHealth());
	chunk.add(getMaxHealth(100));
	chunk.add(percentHealBoost);
	chunk.add(immuneToStatuses);
	chunk.add(baseBlock);
	chunk.add(name);
	chunk.add("MOVES START");
	for (Move* m : moves)
	{
		chunk.add(static_cast<int>(m->getId()));
	}
	chunk.add("MOVES END");
	chunk.add(color);
	chunk.add(baseDodgeChance);
	chunk.add("ATTACK STATUSES START");
	for (std::pair<Status*, int> stat : attackStatuses)
	{
		chunk.add(std::to_string(static_cast<int>(stat.first->getID())) + " " + std::to_string(stat.second));
	}
	chunk.add("ATTACK STATUSES END");
	chunk.add("ARTIFACTS START");
	for (Artifact* a : artifacts)
	{
		chunk.add(std::to_string(static_cast<int>(a->getID())) + " " + std::to_string(static_cast<int>(a->isConsumed())));
	}
	chunk.add("ARTIFACTS END");
	chunk.add(movesToChooseFrom);
	chunk.add("SELF STARTING STATUSES START");
	for (std::pair<Status*, int> stat : selfStartingStatuses)
	{
		chunk.add(std::to_string(static_cast<int>(stat.first->getID())) + " " + std::to_string(stat.second));
	}
	chunk.add("SELF STARTING STATUSES END");
	chunk.add("ENEMY STARTING STATUSES START");
	for (std::pair<Status*, int> stat : enemyStartingStatuses)
	{
		chunk.add(std::to_string(static_cast<int>(stat.first->getID())) + " " + std::to_string(stat.second));
	}
	chunk.add("ENEMY STARTING STATUSES END");
	chunk.add(startingVitality);
	chunk.add(maxVitality);
	chunk.add(vitalityGain);
	chunk.add(experience);
	chunk.add(movesetLimit);
	chunk.add(percentXPBoost);
	chunk.add("PLAYER END");
	return chunk;
}

PlayerId Player::getPlayerId()
{
	return id;
}

void Player::setShouldReceiveXP(bool val)
{
	shouldReceiveXP = val;
}

void Player::doStartBattleArtifactEffects(Enemy* enemy)
{
	const int LINE = ddutil::CENTER_TEXT_LINE;
	for (Artifact* a : artifacts)
	{
		ColorString info = a->startOfBattleAction(this, enemy);
		if (!info.empty())
		{
			game->getVWin()->putcen(info, LINE);
			Menu::oneOptionMenu(game->getVWin(), ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, LINE + 1), true);
			game->getVWin()->clearLine(LINE);
			game->getVWin()->clearLine(LINE + 1);
		}
	}
}

void Player::cosmicAscension()
{
	game->getVWin()->clearScreen();
	const int CHAR_LINE = ddutil::DIVIDER_LINE3 - 2;
	game->getVWin()->printArtFromBottom(getPicture(), Coordinate(0, CHAR_LINE), true);
	int line = ddutil::DIVIDER_LINE3 + 1;
	game->getVWin()->putcen(getStatLine(), line++);
	for (Move* m : moves)
	{
		game->getVWin()->putcen(m->getFullInformation(), line++);
	}
	Sleep(500);
	line--;
	while (moves.size() > 0)
	{
		removeNewestMove();
		game->getVWin()->clearLine(line--);
		playSound(WavFile("shot3", false, false));
	}

	startingVitality = 10 + startingVitality;
	vitalityGain = 10 + vitalityGain;
	maxVitality = 100;
	movesetLimit = 4;
	const int NUM_NEW_MOVES = 2;
	std::vector<Move*> newMoves = CosmicMoves::getRandomMoves(NUM_NEW_MOVES);
	for (Move* m : newMoves)
	{
		moves.push_back(m);
	}
	color = ddutil::COSMIC_COLOR;
	resetPicture(); // do after cosmic move added

	playSound(WavFile("lightning", false, true));
	for (int i = 0; i < ddutil::CONSOLEY; i++)
	{
		game->getVWin()->clearLine(i, ddutil::getColor(ddutil::BLACK, ddutil::COSMIC_COLOR));
		Sleep(1);
	}
	for (int i = 0; i < ddutil::CONSOLEY; i++)
	{
		game->getVWin()->clearLine(i, ddutil::BLACK);
		Sleep(1);
	}
	game->getVWin()->printArtFromBottom(getPicture(), Coordinate(0, CHAR_LINE), true);
	line = ddutil::DIVIDER_LINE3 + 1;
	game->getVWin()->putcen(getStatLine(), line++);
	game->getVWin()->putcen(ColorString("(" + std::to_string(moves.size()) + "/" + std::to_string(movesetLimit) + ") Moves", ddutil::TEXT_COLOR), line++);
	for (Move* m : moves)
	{
		game->getVWin()->putcen(m->getFullInformation(), line++);
	}
	Menu::oneOptionMenu(game->getVWin(), ColorString("Continue", ddutil::TEXT_COLOR), Coordinate(0, line), true);
}

bool Player::isCosmic()
{
	for (Move* m : moves)
	{
		if (m->getStrength() == Strength::Cosmic)
		{
			return true;
		}
	}
	return false;
}

void Player::addMove(Move* move)
{
	if (static_cast<int>(moves.size()) < movesetLimit)
	{
		moves.push_back(move);
	}
	else
	{
		delete move;
	}
}

void Player::setColor(int color)
{
	this->color = color;
	resetPicture();
}


Savechunk Player::getUniqueSaveChunkInfo()
{
	return Savechunk();
}

void Player::doMiscDamageEffects(int damage)
{
	if (damage <= 0)
	{
		return;
	}
	for (unsigned i = 0; i < artifacts.size(); i++)
	{
		if (artifacts.at(i)->getID() == ArtifactID::WaxWings)
		{
			delete artifacts[i];
			artifacts.erase(artifacts.begin() + i);
			break;
		}
	}
}

// Samurai
Samurai::Samurai(Game* game)
	:Player(game, PlayerId::Samurai, Samurai::STARTING_VITALITY, Samurai::MAX_VITALITY, Samurai::VITALITY_GAIN, Samurai::MAX_HP,
		Samurai::MAX_MOVES, "Samurai", ddutil::SAMURAI_COLOR, Art::getSamurai(ddutil::SAMURAI_COLOR), false)
{
	moves.push_back(new SamuraiMoves::Slice());
	moves.push_back(new SamuraiMoves::Slice());
	moves.push_back(new SamuraiMoves::Slice());
	moves.push_back(new SamuraiMoves::Deflect());
	moves.push_back(new SamuraiMoves::Deflect());
}

std::vector<Move*> Samurai::getRandomMoves(Strength str)
{
	std::vector<Move*> newMoves;
	const unsigned MOVES_TO_CHOOSE_FROM = movesToChooseFrom;

	if (str == Strength::Moderate)
	{
		for (int i : ddutil::uniqueRandom(1, 13, MOVES_TO_CHOOSE_FROM))
		{
			switch (i)
			{
			case 1:
				newMoves.push_back(new SamuraiMoves::PerilousStrike());
				break;
			case 2:
				newMoves.push_back(new SamuraiMoves::CalculatedSlice());
				break;
			case 3:
				newMoves.push_back(new SamuraiMoves::Sidestep());
				break;
			case 4:
				newMoves.push_back(new SamuraiMoves::Meditation());
				break;
			case 5:
				newMoves.push_back(new SamuraiMoves::Storm());
				break;
			case 6:
				newMoves.push_back(new SamuraiMoves::LightningRod());
				break;
			case 7:
				newMoves.push_back(new SamuraiMoves::Assault());
				break;
			case 8:
				newMoves.push_back(new SamuraiMoves::DualSlice());
				break;
			case 9:
				newMoves.push_back(new SamuraiMoves::BodySlam());
				break;
			case 10:
				newMoves.push_back(new SamuraiMoves::ThrowingKnives());
				break;
			case 11:
				newMoves.push_back(new SamuraiMoves::HelpingHand());
				break;
			case 12:
				newMoves.push_back(new SamuraiMoves::Tackle());
				break;
			case 13:
				newMoves.push_back(new SamuraiMoves::ToughenUp());
				break;
			}
		}
	}
	else if (str == Strength::Powerful)
	{
		for (int i : ddutil::uniqueRandom(1, 17, MOVES_TO_CHOOSE_FROM))
		{
			switch (i)
			{
			case 1:
				newMoves.push_back(new SamuraiMoves::DragonSlice());
				break;
			case 2:
				newMoves.push_back(new SamuraiMoves::CrushingBlow());
				break;
			case 3:
				newMoves.push_back(new SamuraiMoves::PerilousPlunge());
				break;
			case 4:
				newMoves.push_back(new SamuraiMoves::CriticalStrike());
				break;
			case 5:
				newMoves.push_back(new SamuraiMoves::Zen());
				break;
			case 6:
				newMoves.push_back(new SamuraiMoves::FinishingBlow());
				break;
			case 7:
				newMoves.push_back(new SamuraiMoves::ChargedAssault());
				break;
			case 8:
				newMoves.push_back(new SamuraiMoves::Typhoon());
				break;
			case 9:
				newMoves.push_back(new SamuraiMoves::Desperation());
				break;
			case 10:
				newMoves.push_back(new SamuraiMoves::EyeOfTheStorm());
				break;
			case 11:
				newMoves.push_back(new SamuraiMoves::Shuriken());
				break;
			case 12:
				newMoves.push_back(new SamuraiMoves::ShadowStep());
				break;
			case 13:
				newMoves.push_back(new SamuraiMoves::WarHorn());
				break;
			case 14:
				newMoves.push_back(new SamuraiMoves::Instinct());
				break;
			case 15:
				newMoves.push_back(new SamuraiMoves::Unhinge());
				break;
			case 16:
				newMoves.push_back(new SamuraiMoves::Revitalize());
				break;
			case 17:
				newMoves.push_back(new SamuraiMoves::FlameVeil());
				break;
			}
		}
	}
	else if (str == Strength::Mythical)
	{
		for (int i : ddutil::uniqueRandom(1, 15, MOVES_TO_CHOOSE_FROM))
		{
			switch (i)
			{
			case 1:
				newMoves.push_back(new SamuraiMoves::BlindingFury());
				break;
			case 2:
				newMoves.push_back(new SamuraiMoves::Tsunami());
				break;
			case 3:
				newMoves.push_back(new SamuraiMoves::LeavesFromTheVine());
				break;
			case 4:
				newMoves.push_back(new SamuraiMoves::Enlightenment());
				break;
			case 5:
				newMoves.push_back(new SamuraiMoves::Eviscerate());
				break;
			case 6:
				newMoves.push_back(new SamuraiMoves::DualingDragons());
				break;
			case 7:
				newMoves.push_back(new SamuraiMoves::TippedKunai());
				break;
			case 8:
				newMoves.push_back(new SamuraiMoves::Caltrops());
				break;
			case 9:
				newMoves.push_back(new SamuraiMoves::DragonForm());
				break;
			case 10:
				newMoves.push_back(new SamuraiMoves::DragonBreath());
				break;
			case 11:
				newMoves.push_back(new SamuraiMoves::Tornado());
				break;
			case 12:
				newMoves.push_back(new SamuraiMoves::ShinobiTactics());
				break;
			case 13:
				newMoves.push_back(new SamuraiMoves::HoldOut());
				break;
			case 14:
				newMoves.push_back(new SamuraiMoves::Decimate());
				break;
			case 15:
				newMoves.push_back(new SamuraiMoves::DragonsWill());;
				break;
			}
		}
	}
	return newMoves;
}

void Samurai::resetPicture()
{
	if (isCosmic())
	{
		this->changePicture(Art::getCosmicSamurai());
	}
	else
	{
		this->changePicture(Art::getSamurai(color));
	}
}

Creature* Samurai::makeCopy()
{
	return new Samurai(getGamePtr());
}
// Gunslinger

Gunslinger::Gunslinger(Game* game)
	:Player(game, PlayerId::Gunslinger, Gunslinger::STARTING_VITALITY, Gunslinger::MAX_VITALITY, Gunslinger::VITALITY_GAIN, Gunslinger::MAX_HP,
		Gunslinger::MAX_MOVES, "Gunslinger", ddutil::GUNSLINGER_COLOR, Art::getGunslinger(ddutil::GUNSLINGER_COLOR), false)
{
	
	maxBullets = Gunslinger::STARTING_MAX_BULLETS;
	reserveBullets = Gunslinger::STARTING_BULLETS;

	// Starting moves
	moves.push_back(new GunslingerMoves::Peashooter());
	moves.push_back(new GunslingerMoves::Peashooter());
	moves.push_back(new GunslingerMoves::Peashooter());
	moves.push_back(new GunslingerMoves::Peashooter());
	moves.push_back(new GunslingerMoves::Dodge());
	moves.push_back(new GunslingerMoves::MakeBullets());
}

std::vector<Move*> Gunslinger::getRandomMoves(Strength str)
{
	std::vector<Move*> newMoves;
	const unsigned MOVES_TO_CHOOSE_FROM = static_cast<unsigned>(movesToChooseFrom);

	if (str == Strength::Moderate)
	{
		for (int i : ddutil::uniqueRandom(1,13,MOVES_TO_CHOOSE_FROM))
		{
			switch (i)
			{
			case 1:
				newMoves.push_back(new GunslingerMoves::Revolver());
				break;
			case 2:
				newMoves.push_back(new GunslingerMoves::CardThrow());
				break;
			case 3:
				newMoves.push_back(new GunslingerMoves::ThrowDynamite());
				break;
			case 4:
				newMoves.push_back(new GunslingerMoves::CraftBullets());
				break;
			case 5:
			case 6:
			{
				int randomNum = ddutil::random(1, 4);
				if (randomNum == 1)
				{
					newMoves.push_back(new GunslingerMoves::DrawClub());
				}
				else if (randomNum == 2)
				{
					newMoves.push_back(new GunslingerMoves::DrawSpade());
				}
				else if (randomNum == 3)
				{
					newMoves.push_back(new GunslingerMoves::DrawDiamond());
				}
				else
				{
					newMoves.push_back(new GunslingerMoves::DrawHeart());
				}
				break;
			}
			case 7:
				newMoves.push_back(new GunslingerMoves::Punch());
				break;
			case 8:
				newMoves.push_back(new GunslingerMoves::Backflip());
				break;
			case 9:
				newMoves.push_back(new GunslingerMoves::Dropkick());
				break;
			case 10:
				newMoves.push_back(new GunslingerMoves::BowAndArrow());
				break;
			case 11:
				newMoves.push_back(new GunslingerMoves::TheMagician());
				break;
			case 12:
				newMoves.push_back(new GunslingerMoves::FastFeet());
				break;
			default:
				newMoves.push_back(new GunslingerMoves::BottleStrike());
				break;
			}
		}
	}
	else if (str == Strength::Powerful)
	{
		for (int i : ddutil::uniqueRandom(1, 17, MOVES_TO_CHOOSE_FROM))
		{
			switch (i)
			{
			case 1:
				newMoves.push_back(new GunslingerMoves::PumpShotgun());
				break;
			case 2:
				newMoves.push_back(new GunslingerMoves::ForgeBullets());
				break;
			case 3:
				newMoves.push_back(new GunslingerMoves::QuickDynamite());
				break;
			case 4:
				newMoves.push_back(new GunslingerMoves::DrinkWhiskey());
				break;
			case 5:
				newMoves.push_back(new GunslingerMoves::CardFlurry());
				break;
			case 6:
				newMoves.push_back(new GunslingerMoves::Uppercut());
				break;
			case 7:
				newMoves.push_back(new GunslingerMoves::Leap());
				break;
			case 8:
				newMoves.push_back(new GunslingerMoves::HeavyRevolver());
				break;
			case 9:
				newMoves.push_back(new GunslingerMoves::DoubleKick());
				break;
			case 10:
				newMoves.push_back(new GunslingerMoves::TheHighPriestess());
				break;
			case 11:
				newMoves.push_back(new GunslingerMoves::Judgement());
				break;
			case 12:
				newMoves.push_back(new GunslingerMoves::KnifeThrow());
				break;
			case 13:
				newMoves.push_back(new GunslingerMoves::ToxicToss());
				break;
			case 14:
				newMoves.push_back(new GunslingerMoves::TippedShot());
				break;
			case 15:
				newMoves.push_back(new GunslingerMoves::Dash());
				break;
			case 16:
				newMoves.push_back(new GunslingerMoves::SmokeAndMirrors());
				break;
			default:
				newMoves.push_back(new GunslingerMoves::ExplosiveShot());
				break;
			}
		}
	}
	else if (str == Strength::Mythical)
	{
		for (int i : ddutil::uniqueRandom(1, 14, MOVES_TO_CHOOSE_FROM))
		{
			switch (i)
			{
			case 1:
				newMoves.push_back(new GunslingerMoves::SharpsRifle());
				break;
			case 2:
				newMoves.push_back(new GunslingerMoves::ColtWalker());
				break;
			case 3:
				newMoves.push_back(new GunslingerMoves::SuckerPunch());
				break;
			case 4:
				newMoves.push_back(new GunslingerMoves::ConjureBullets());
				break;
			case 5:
				newMoves.push_back(new GunslingerMoves::BlackDynamite());
				break;
			case 6:
				newMoves.push_back(new GunslingerMoves::BrassKnuckles());
				break;
			case 7:
				newMoves.push_back(new GunslingerMoves::JesterForm());
				break;
			case 8:
				newMoves.push_back(new GunslingerMoves::SupressiveFire());
				break;
			case 9:
				newMoves.push_back(new GunslingerMoves::TheEmpress());
				break;
			case 10:
				newMoves.push_back(new GunslingerMoves::TheEmperor());
				break;
			case 11:
				newMoves.push_back(new GunslingerMoves::TheSun());
				break;
			case 12:
				newMoves.push_back(new GunslingerMoves::CrossSlash());
				break;
			default:
				int randomNum = ddutil::random(1, 4);
				if (randomNum == 1)
				{
					newMoves.push_back(new GunslingerMoves::FaceOfClubs());
				}
				else if (randomNum == 2)
				{
					newMoves.push_back(new GunslingerMoves::FaceOfSpades());
				}
				else if (randomNum == 3)
				{
					newMoves.push_back(new GunslingerMoves::FaceOfDiamonds());
				}
				else
				{
					newMoves.push_back(new GunslingerMoves::FaceOfHearts());
				}
				break;
			}
		}
	}
	return newMoves;
}

void Gunslinger::resetPicture()
{
	if (isCosmic())
	{
		this->changePicture(Art::getCosmicGunslinger());
	}
	else
	{
		this->changePicture(Art::getGunslinger(color));
	}
}

Creature* Gunslinger::makeCopy()
{
	return new Gunslinger(getGamePtr());
}

int Gunslinger::getReserveBullets()
{
	return reserveBullets;
}

bool Gunslinger::useBullet(int amount)
{
	if (amount <= reserveBullets)
	{
		reserveBullets -= amount;
		return true;
	}
	else
	{
		return false;
	}
}

void Gunslinger::createBullets(int amount)
{
	reserveBullets += amount;
	if (reserveBullets > maxBullets)
	{
		reserveBullets = maxBullets;
	}
}

ColorString Gunslinger::getStatLine()
{
	ColorString c = Player::getStatLine();
	c += ColorString("(" + std::to_string(reserveBullets) + "/" +std::to_string(maxBullets)+ " bullets) ", ddutil::TEXT_COLOR);
	return c;
}

void Gunslinger::setReserveBullets(int val)
{
	reserveBullets = val;
}

void Gunslinger::setMaxBullets(int val)
{
	maxBullets = val;
}

Savechunk Gunslinger::getUniqueSaveChunkInfo()
{
	Savechunk chunk;
	chunk.add(reserveBullets);
	chunk.add(maxBullets);
	return chunk;


}

// Sorcerer

Sorcerer::Sorcerer(Game* game)
	:Player(game, PlayerId::Sorceress, Sorcerer::STARTING_VITALITY, Sorcerer::MAX_VITALITY, Sorcerer::VITALITY_GAIN, Sorcerer::MAX_HP,
		Sorcerer::MAX_MOVES, "Sorceress", ddutil::SORCERER_COLOR, Art::getSorcerer(ddutil::SORCERER_COLOR), false)
{
	// Starting moves
	moves.push_back(new SorcererMoves::EnergyStrike());
	moves.push_back(new SorcererMoves::EnergyStrike());
	moves.push_back(new SorcererMoves::EnergyStrike());
	moves.push_back(new SorcererMoves::Heal());
	moves.push_back(new SorcererMoves::MagicBarrier());
	maxAura = STARTING_MAX_AURA;
	aura = maxAura;
}

std::vector<Move*> Sorcerer::getRandomMoves(Strength str)
{
	std::vector<Move*> newMoves;
	const unsigned MOVES_TO_CHOOSE_FROM = static_cast<unsigned>(movesToChooseFrom);

	if (str == Strength::Moderate)
	{
		for (int i : ddutil::uniqueRandom(1, 14, MOVES_TO_CHOOSE_FROM))
		{
			switch (i)
			{
			case 1:
				newMoves.push_back(new SorcererMoves::BlinkStrike());
				break;
			case 2:
				newMoves.push_back(new SorcererMoves::SummonIceImp(getGamePtr()));
				break;
			case 3:
				newMoves.push_back(new SorcererMoves::SummonFireImp(getGamePtr()));
				break;
			case 4:
				newMoves.push_back(new SorcererMoves::Recover());
				break;
			case 5:
				newMoves.push_back(new SorcererMoves::Fireball());
				break;
			case 6:
				newMoves.push_back(new SorcererMoves::IceOrb());
				break;
			case 7:
				newMoves.push_back(new SorcererMoves::EnergyBarrier());
				break;
			case 8:
				newMoves.push_back(new SorcererMoves::CleansingTouch());
				break;
			case 9:
				newMoves.push_back(new SorcererMoves::EnergySword());
				break;
			case 10:
				newMoves.push_back(new SorcererMoves::PlasmaStrike());
				break;
			case 11:
				newMoves.push_back(new SorcererMoves::Levitate());
				break;
			case 12:
				newMoves.push_back(new SorcererMoves::ElectricShock());
				break;
			case 13:
				newMoves.push_back(new SorcererMoves::BranchWhip());
				break;
			case 14:
				newMoves.push_back(new SorcererMoves::AuraStrike());	
				break;
			}
		}
	}
	else if (str == Strength::Powerful)
	{
		for (int i : ddutil::uniqueRandom(1, 15, MOVES_TO_CHOOSE_FROM))
		{
			switch (i)
			{
			case 1:
				newMoves.push_back(new SorcererMoves::FairySummon(getGamePtr()));
				break;
			case 2:
				newMoves.push_back(new SorcererMoves::SummonFireDragon(getGamePtr()));
				break;
			case 3:
				newMoves.push_back(new SorcererMoves::SummonIceDragon(getGamePtr()));
				break;
			case 4:
				newMoves.push_back(new SorcererMoves::Blizzard());
				break;
			case 5:
				newMoves.push_back(new SorcererMoves::StarStrike());
				break;
			case 6:
				newMoves.push_back(new SorcererMoves::PlasmaSword());
				break;
			case 7:
				newMoves.push_back(new SorcererMoves::Cataclysm());
				break;
			case 8:
				newMoves.push_back(new SorcererMoves::EtherealBarrier());
				break;
			case 9:
				newMoves.push_back(new SorcererMoves::Combust());
				break;
			case 10:
				newMoves.push_back(new SorcererMoves::Freeze());
				break;
			case 11:
				newMoves.push_back(new SorcererMoves::MirrorImage());
				break;
			case 12:
				newMoves.push_back(new SorcererMoves::LightningStrike());
				break;
			case 13:
				newMoves.push_back(new SorcererMoves::SelfHex());
				break;
			case 14:
				newMoves.push_back(new SorcererMoves::BindingGrasp());
				break;
			case 15:
				newMoves.push_back(new SorcererMoves::AuraBlast());
				break;
			}
		}
	}
	else if (str == Strength::Mythical)
	{
		for (int i : ddutil::uniqueRandom(1, 15, MOVES_TO_CHOOSE_FROM))
		{
			switch (i)
			{
			case 1:
				newMoves.push_back(new SorcererMoves::CleansingAura());
				break;
			case 2:
				newMoves.push_back(new SorcererMoves::CastInvulnerability());
				break;
			case 3:
				newMoves.push_back(new SorcererMoves::EnergyAura());
				break;
			case 4:
				newMoves.push_back(new SorcererMoves::Supernova());
				break;
			case 5:
				newMoves.push_back(new SorcererMoves::EtherealBlade());
				break;
			case 6:
				newMoves.push_back(new SorcererMoves::SpiritCall(getGamePtr()));
				break;
			case 7:
				newMoves.push_back(new SorcererMoves::ChainLightning());
				break;
			case 8:
				newMoves.push_back(new SorcererMoves::ElementalForm());
				break;
			case 9:
				newMoves.push_back(new SorcererMoves::ElementalBarrier());
				break;
			case 10:
				newMoves.push_back(new SorcererMoves::UltimateShock());
				break;
			case 11:
				newMoves.push_back(new SorcererMoves::XCast());
				break;
			case 12:
				newMoves.push_back(new SorcererMoves::TreeOfPower());
				break;
			case 13:
				newMoves.push_back(new SorcererMoves::AuraBomb());
				break;
			case 14:
				newMoves.push_back(new SorcererMoves::TreeOfLife());
				break;
			case 15:
				newMoves.push_back(new SorcererMoves::Resurrect());
				break;
			}
		}
	}
	return newMoves;
}


void Sorcerer::resetPicture()
{
	if (isCosmic())
	{
		this->changePicture(Art::getCosmicSorceress());
	}
	else
	{
		this->changePicture(Art::getSorcerer(color));
	}
	
}

Creature* Sorcerer::makeCopy()
{
	return new Sorcerer(getGamePtr());
}

void Sorcerer::resetTempStatAdjustments()
{
	Player::resetTempStatAdjustments();
	aura = maxAura;
	selfDamageThisFight = 0;
}

ColorString Sorcerer::getStatLine()
{
	ColorString c = Player::getStatLine();
	c += ColorString("(" + std::to_string(aura) + "/" +std::to_string(maxAura)+ " Aura) ", ddutil::AURA_COLOR);
	return c;
}

bool Sorcerer::useAura(int amount)
{
	if (aura >= amount)
	{
		aura -= amount;
		return true;
	}
	return false;
}

int Sorcerer::getAura() const
{
	return aura;
}


// Unlockable Players

// Minions

PlayerMinion::PlayerMinion(Game* game, int svit, int maxVit, int vitGain, int maxHp, int moveLim, std::string name, int color, Picture pic)
	:Player(game, PlayerId::Minion, svit, maxVit, vitGain, maxHp, moveLim, name, color, pic, true)
{
}

std::vector<Move*> PlayerMinion::getRandomMoves(Strength str)
{
	// does nothing because minions cannot learn new moves
	return std::vector<Move*>();
}


FireImp::FireImp(Game* game)
	:PlayerMinion(game, 2, 2, 2, 8, 1, "Fire Imp", ddutil::RED, Art::getFireImp())
{
	moves.push_back(new SorcererMoves::Fireball());
}

Creature* FireImp::makeCopy()
{
	FireImp* minion = new FireImp(getGamePtr());
	return minion;
}

IceImp::IceImp(Game* game)
	: PlayerMinion(game, 2, 2, 2, 8, 1, "Ice Imp", ddutil::LIGHTCYAN, Art::getIceImp())
{
	moves.push_back(new SorcererMoves::IceOrb());
}

Creature* IceImp::makeCopy()
{
	IceImp* minion = new IceImp(getGamePtr());
	return minion;
}

Fairy::Fairy(Game* game)
	:PlayerMinion(game, 2, 4, 2, 5, 2, "Fairy", ddutil::LIGHTMAGENTA, Art::getFairy())
{
	moves.push_back(new MinionMoves::FairyDust());
	moves.push_back(new MinionMoves::FairyDust());
}

Creature* Fairy::makeCopy()
{
	Fairy* minion = new Fairy(getGamePtr());
	return minion;
}

FireDragon::FireDragon(Game* game)
	:PlayerMinion(game, 2, 4, 2, 20, 1, "Fire Dragon", ddutil::RED, Art::getFireDragon())
{
	moves.push_back(new MinionMoves::FireBreath());
	moves.push_back(new MinionMoves::FireBreath());
}

Creature* FireDragon::makeCopy()
{
	FireDragon* minion = new FireDragon(getGamePtr());
	return minion;
}

IceDragon::IceDragon(Game* game)
	: PlayerMinion(game, 2, 4, 2, 20, 1, "Ice Dragon", ddutil::LIGHTCYAN, Art::getIceDragon())
{
	moves.push_back(new MinionMoves::IceBreath());
	moves.push_back(new MinionMoves::IceBreath());
}

Creature* IceDragon::makeCopy()
{
	IceDragon* minion = new IceDragon(getGamePtr());
	return minion;
}

SpiritKnight::SpiritKnight(Game* game)
	: PlayerMinion(game, 2, 6, 2, 30, 3, "Spirit Knight", ddutil::WHITE, Art::getSpiritKnight())
{
	moves.push_back(new MinionMoves::SpiritStrike());
	moves.push_back(new MinionMoves::SpiritStrike());
	moves.push_back(new MinionMoves::SpiritProjection());
}

Creature* SpiritKnight::makeCopy()
{
	SpiritKnight* minion = new SpiritKnight(getGamePtr());
	return minion;
}
