#include "enemy.h"
#include "move.h"
#include "art.h"
#include "enemy_moves.h"
#include "game.h"
#include "player.h"
#include "creature.h"
#include "utilities.h"
#include "colorstring.h"
#include "virtualwindow.h"
#include "menu.h"
#include "zone.h"
#include "sorcerer_moves.h"
#include "room.h"

#include <vector>
#include <string>

// Enemy Turn

EnemyTurn::EnemyTurn(ColorString theIntent, std::vector<Creature*> theTargets, Move* theMove)
{
	intent = theIntent;
	targets = theTargets;
	move = theMove;
}

void EnemyTurn::checkForDeadTargets()
{
	// if the player somehow died then remove them from the list
	auto it = targets.begin();
	while (it != targets.end())
	{
		if ((*it)->getHealth() <= 0)
		{
			it = targets.erase(it);
		}
		else
		{
			++it;
		}
	}
}

ColorString EnemyTurn::getIntent()
{
	return intent;
}

const std::vector<Creature*>& EnemyTurn::getTargets()
{
	return targets;
}

Move* EnemyTurn::getMove()
{
	return move;
}


// Enemy

Enemy::Enemy(Game* game, int maxHp, std::string name, int color, Picture pic, Mp3File theMusic, std::vector<ColorString> intro, char theChar)
	:Creature(maxHp, name, color, pic, game)
{
	music = theMusic;
	introText = intro;
	mapChar = theChar;
}

void Enemy::distortionUpdate(int dist)
{
	if (dist >= 4)
	{
		increaseMaxHealth(getMaxHealth(ddutil::DIST4_PERC_ENEMY_HP_INC));
	}
}

ColorString Enemy::getStatLine()
{
	return Creature::getStatLine();
}

const Mp3File& Enemy::getMusic() const
{
	return music;
}

const std::vector<ColorString>& Enemy::getIntroText() const
{
	return introText;
}

char Enemy::getMapChar()
{
	return mapChar;
}

void Enemy::deathScene()
{
	VirtualWindow* vwin = game->getVWin();

	game->clearCenterScreen();
	game->clearBottomDivider();

	Coordinate coord = Coordinate(0, ddutil::CENTER_TEXT_LINE - 2);

	vwin->printArtFromBottom(picture, coord, true);
	Sleep(150);
	playSound(WavFile("enemydeath1", false, true));
	Sleep(400);
	game->clearCenterScreen();
	Sleep(350);

	ColorString message = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" has been slain", ddutil::TEXT_COLOR);
	message += ColorString("!", ddutil::TEXT_COLOR);

	vwin->putcen(message, coord.y);
	coord.y++;
	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), coord, true);

	game->clearCenterScreen();
}

NormalEnemy::NormalEnemy(Game* game, int maxHp, std::string name, int color, Picture pic)
	:Enemy(game, maxHp, name, color, pic, Mp3File("normalbattle"),
		std::vector<ColorString>{ColorString("A ", ddutil::TEXT_COLOR) + ColorString(name, color) + ColorString(" attacks!", ddutil::TEXT_COLOR)},
		ddutil::MAP_NENEMY)
{
}

void NormalEnemy::deathScene()
{
	Enemy::deathScene();
	game->displayInfo();
	game->changeScore(ddutil::NORM_ENEMY_SCORE);

	std::vector<Artifact*> choices = Artifact::getRandomCommonArtifacts(game);
	std::vector<ColorString> menuOptions;
	menuOptions.push_back(ColorString("Skip", ddutil::TEXT_COLOR));
	menuOptions.push_back(ColorString("View Menu", ddutil::TEXT_COLOR));
	for (Artifact* a : choices)
	{
		menuOptions.push_back(a->getFullInformation());
	}
	while (true)
	{
		game->clearCenterScreen();
		game->clearBottomDivider();
		game->displayInfo();
		game->getVWin()->putcenSlowScroll(ColorString("The ", ddutil::ARTIFACT_COLOR) + getColorString() + 
			ColorString(" drops some artifacts. Choose one:", ddutil::ARTIFACT_COLOR), ddutil::EVENT_PICTURE_LINE-2);

		Menu menu(game->getVWin(), menuOptions, Coordinate(0, ddutil::EVENT_PICTURE_LINE), true);
		if (menu.getResponse() == 0)
		{
			break;
		}
		else if (menu.getResponse() == 1)
		{
			game->viewMenu();
		}
		else // selected an artifact
		{
			int index = menu.getResponse() - 2;
			Artifact* chosen = choices.at(index);
			choices.erase(choices.begin() + index);
			game->clearCenterScreen();
			game->artifactSelectionMenu(ddutil::EVENT_PICTURE_LINE, chosen);
			break;
		}
	}

	// delete the artifacts which are left, which will be all 3 if skipped, or just the 2 that weren't picked
	for (Artifact* a : choices)
	{
		delete a;
	}
	game->clearCenterScreen();
	game->displayInfo();
}

Ghost::Ghost(Game* game)
	: NormalEnemy(game, HEALTH, "Ghost", ddutil::WHITE, Art::getGhost())
{
	moves.push_back(new EnemyMoves::Burn(BURN_AMOUNT));
	moves.push_back(new EnemyMoves::Invulnerability(INVULNERABILITY_LENGTH));
	moves.push_back(new EnemyMoves::Strike(SINGLE_TARGET_DAMAGE, WavFile("energyattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Strike(MULTI_TARGET_DAMAGE, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	turnCounter = 0;
}

EnemyTurn Ghost::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	turnCounter++;
	if (turnCounter > 4)
	{
		turnCounter = 1;
	}

	switch (turnCounter)
	{
	case 1:
		chosenMove = moves[0];
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("Burn ", BurntStatus::COLOR) + ColorString("everybody for " + std::to_string(BURN_AMOUNT) + " turns", ddutil::TEXT_COLOR);
		targets = players;
		break;
	case 2:
		chosenMove = moves[1];

		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("become ", ddutil::TEXT_COLOR) + ColorString("Invulnerable ", InvulnerableStatus::COLOR) +
			ColorString("for " + std::to_string(INVULNERABILITY_LENGTH) + " turns", ddutil::TEXT_COLOR);
		
		targets.push_back(this);
		break;
	case 3: {
		chosenMove = moves[2];

		Creature* theTarget = ddutil::getLowestHealthPlayer(players);

		targets.push_back(theTarget);

		intent = ddutil::genericDamageIntent(SINGLE_TARGET_DAMAGE, getColorString(), "attack", targets);

		break;
	}
	default: // will be when turn counter is 4, or if there is a bug which makes turnCounter not 1,2,3 or 4
		chosenMove = moves[3];
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("attack everybody for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(MULTI_TARGET_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	}
	return EnemyTurn(intent, targets, chosenMove);
}

Creature* Ghost::makeCopy()
{
	return new Ghost(game);
}

int Ghost::getRoomId()
{
	return RoomId::GhostEnemy;
}

GhostVariant::GhostVariant(Game* game) 
	: NormalEnemy(game, HEALTH, "Ghost", ddutil::BLUE, Art::getBlueGhost())
{
	moves.push_back(new EnemyMoves::Frostburn(FBURN_AMOUNT));
	moves.push_back(new EnemyMoves::Strike(SINGLE_TARGET_DAMAGE, WavFile("energyattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Heal(HEAL_AMOUNT));
	turnCounter = 0;
}

EnemyTurn GhostVariant::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	turnCounter++;
	if (turnCounter > 3)
	{
		turnCounter = 1;
	}

	switch (turnCounter)
	{
	case 1:
		chosenMove = moves[0];
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("Frostburn ", FrostBurntStatus::COLOR) + ColorString("everybody for " + std::to_string(FBURN_AMOUNT) + " turns", ddutil::TEXT_COLOR);
		targets = players;
		break;
	case 2:
		chosenMove = moves[1];
		targets.push_back(game->getPlayerParty()[ddutil::random(0, game->getPlayerParty().size() - 1)]);
		intent = ddutil::genericDamageIntent(SINGLE_TARGET_DAMAGE, getColorString(), "Phase Strike", targets);
		break;
	default: // 3
		chosenMove = moves[2];
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("heal " + std::to_string(HEAL_AMOUNT) + " HP", ddutil::HEAL_COLOR);
		targets.push_back(this);

		break;
	}
	
	return EnemyTurn(intent, targets, chosenMove);
}

Creature* GhostVariant::makeCopy()
{
	return new GhostVariant(game);
}

int GhostVariant::getRoomId()
{
	return RoomId::GhostVariantEnemy;
}


GiantSnail::GiantSnail(Game* game)
	:NormalEnemy(game, HEALTH, "Giant Snail", Art::GIANT_SNAIL_COLOR, Art::getGiantSnail())
{
	moves.push_back(new EnemyMoves::StrikeAndBlock(MULTI_TARGET_DAMAGE, BLOCK_PER_ATTACK, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Block(BLOCK, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Strike(SINGLE_TARGET_DAMAGE, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	turnCounter = 0;
}

EnemyTurn GiantSnail::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	int turnNumber = ddutil::random(0, 2);
	chosenMove = moves[turnNumber];
	switch (turnNumber)
	{
	case 0: // multi target attacka nnd block
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to Strike everybody for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(MULTI_TARGET_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) + ColorString(" and block ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(BLOCK_PER_ATTACK * targets.size()) + " damage ", ddutil::BLOCK_COLOR) +
			ColorString("next turn", ddutil::TEXT_COLOR);
		break;
	case 1: // straight block
		targets.push_back(this);
		intent = ddutil::genericBlockIntent(BLOCK, getColorString());
		break;
	case 2: // single target attack no block
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ddutil::genericDamageIntent(SINGLE_TARGET_DAMAGE, getColorString(), "Strike", targets);
		break;
	}
	return EnemyTurn(intent, targets, chosenMove);
}

Creature* GiantSnail::makeCopy()
{
	return new GiantSnail(game);
}

int GiantSnail::getRoomId()
{
	return RoomId::GiantSnailEnemy;
}

GiantSnailVariant::GiantSnailVariant(Game* game)
	:NormalEnemy(game, HEALTH, "Giant Snail", ddutil::YELLOW, Art::getYellowGiantSnail())
{
	moves.push_back(new EnemyMoves::Strike(LOW_HP_DAMAGE, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Strike(HIGH_HP_DAMAGE, WavFile("attack2", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn GiantSnailVariant::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
	if (health < getMaxHealth(50))
	{
		chosenMove = moves[0];
		intent = ddutil::genericDamageIntent(LOW_HP_DAMAGE, getColorString(), "Slam into", targets);
	}
	else
	{
		chosenMove = moves[1];
		intent = ddutil::genericDamageIntent(HIGH_HP_DAMAGE, getColorString(), "Strike", targets);
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* GiantSnailVariant::makeCopy()
{
	return new GiantSnailVariant(game);
}

int GiantSnailVariant::getRoomId()
{
	return RoomId::GiantSnailVariantEnemy;
}




HardEnemy::HardEnemy(Game* game, int maxHp, std::string name, int color, Picture pic)
	: Enemy(game, maxHp, name, color, pic, Mp3File("hardbattle"),
		std::vector<ColorString>{ColorString("A ", ddutil::TEXT_COLOR) + ColorString(name, color) + ColorString(" attacks!", ddutil::TEXT_COLOR)},
		ddutil::MAP_SENEMY)
{
}

void HardEnemy::deathScene()
{
	Enemy::deathScene();
	game->displayInfo();
	game->changeScore(ddutil::STRONG_ENEMY_SCORE);

	VirtualWindow* vwin = game->getVWin();
	Artifact* artifact = game->getActiveZone()->getRandomArtifact();
	int line = ddutil::CONSOLEY / 2;
	vwin->putcen(ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" drops a ", ddutil::TEXT_COLOR) +
		artifact->getName(), line - 2);
	game->artifactSelectionMenu(line, artifact);
	game->clearCenterScreen();
}

Minotaur::Minotaur(Game* game)
	: HardEnemy(game, HEALTH, "Minotaur", ddutil::BROWN, Art::getMinotaur())
{
	moves.push_back(new EnemyMoves::Strike(STRIKE_DAMAGE, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::StrikeAndBlock(MULTI_TARGET_DAMAGE, BLOCK_PER_ATTACK, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

// The minotaur randomly selects between an 8 damage strike and 8 damage block
EnemyTurn Minotaur::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;


	if (ddutil::random(1, 2) == 1 || game->getPlayerParty().size() == 1) // single target strike
	{
		chosenMove = moves[0];

		// randomly select one person
		int targetNumber = ddutil::random(0, players.size() - 1);
		targets.push_back(players[targetNumber]);

		intent = ddutil::genericDamageIntent(STRIKE_DAMAGE, getColorString(), "strike", targets);

	}
	else // block and multi target strike
	{
		chosenMove = moves[1];

		targets = players;

		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to Strike everybody for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(MULTI_TARGET_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) + ColorString(" and block ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(BLOCK_PER_ATTACK * targets.size()) + " damage ", ddutil::BLOCK_COLOR) +
			ColorString("next turn", ddutil::TEXT_COLOR);
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* Minotaur::makeCopy()
{
	return new Minotaur(getGamePtr());
}

int Minotaur::getRoomId()
{
	return RoomId::MinotaurEnemy;
}

Brute::Brute(Game* game)
	:HardEnemy(game, HEALTH, "Brute", ddutil::DARKGRAY, Art::getBrute())
{
	moves.push_back(new EnemyMoves::Strike(BASE_DAMAGE, WavFile("attack3", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn Brute::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	auto scalingAttack = dynamic_cast<EnemyMoves::Strike*>(moves[0]);

	if (scalingAttack != nullptr)
	{
		scalingAttack->increaseStrength(DAMAGE_INCREASE);
	}

	targets.push_back(players[ddutil::random(0, players.size() - 1)]);
	chosenMove = moves[0];
	intent = ddutil::genericDamageIntent(scalingAttack->getStrength(), getColorString(), "Strike", targets);

	

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* Brute::makeCopy()
{
	return new Brute(game);
}

int Brute::getRoomId()
{
	return RoomId::BruteEnemy;
}

GiantHead::GiantHead(Game* game)
	:HardEnemy(game, HEALTH, "Giant Head", Art::GIANT_HEAD_COLOR, Art::getGiantHead())
{
	attackTurn = true;
	moves.push_back(new EnemyMoves::Strike(STUN_DAMAGE, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn GiantHead::getTurn(std::vector<Creature*> players)
{

	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	auto scalingAttack = dynamic_cast<EnemyMoves::Strike*>(moves[0]);
	if (attackTurn)
	{
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		chosenMove = moves[0];
		intent = ddutil::genericDamageIntent(scalingAttack->getStrength(), getColorString(), "Maul", targets);
	}
	else
	{
		if (scalingAttack != nullptr)
		{
			scalingAttack->increaseStrength(STRENGTH_INCREASE);
		}
		chosenMove = nullptr;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is recharging this turn", ddutil::TEXT_COLOR);
	}	

	attackTurn = !attackTurn;

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* GiantHead::makeCopy()
{
	return new GiantHead(game);
}

int GiantHead::getRoomId()
{
	return RoomId::GiantHeadEnemy;
}


BossEnemy::BossEnemy(Game* game, int maxHp, std::string name, int color, Picture pic, std::vector<ColorString> intro)
	: Enemy(game, maxHp, name, color, pic, Mp3File("bossbattle"), intro, ddutil::MAP_BOSS)
{
	healthbarLength = ddutil::BOSS_HEALTHBAR_LENGTH;
}

void BossEnemy::deathScene()
{
	game->changeScore(ddutil::BOSS_ENEMY_SCORE);

	VirtualWindow* vwin = game->getVWin();

	game->clearCenterScreen();
	game->clearBottomDivider();

	Coordinate coord = Coordinate(0, ddutil::CENTER_TEXT_LINE - 2);

	vwin->printArtFromBottom(picture, coord, true);
	playSound(WavFile("bossdeath1", false, true));
	for (int i = 0; i < 10; i++)
	{
		vwin->printArtFromBottom(picture, coord, true);
		Sleep(100);
		game->clearCenterScreen();
		Sleep(100);
	}
	game->clearCenterScreen();
	playSound(WavFile("explosion", false, false));
	Sleep(500);
	playSound(WavFile("defeatboss", false, false));

	ColorString message = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" has been slain", ddutil::TEXT_COLOR);
	message += ColorString("!", ddutil::TEXT_COLOR);

	vwin->putcen(message, coord.y);
	coord.y++;
	Menu::oneOptionMenu(vwin, ColorString("Continue", ddutil::TEXT_COLOR), coord, true);

	game->clearCenterScreen();

	game->getActiveZone()->chooseBossArtifact();

	
	if (game->getDeadPlayers().size() > 0)
	{
		vwin->clearScreen();
		
		vwin->printArt(Art::getTheWatcher(), Coordinate(0, 4), true); 
		int line = (ddutil::CONSOLEY * 3) / 4;

		std::vector<ColorString> text = {
			ColorString("\"It appears that some in your party have died\"", ddutil::MAGENTA),
			ColorString("\"My powers cannot reach where you are currently\"", ddutil::MAGENTA),
			ColorString("\"However, if you find your way to a Revival Altar, I will be able to help\"", ddutil::MAGENTA),
			ColorString("\"There, my powers will be able to reach you.\"", ddutil::MAGENTA),
			ColorString("\"Don't give up!\"", ddutil::MAGENTA)
		};
		for (ColorString c : text)
		{
			vwin->putcenSlowScroll(c, line);
			Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, line + 1), true);
			vwin->clearLine(line);
			vwin->clearLine(line + 1);
		}
	}


	game->clearCenterScreen();
}

TheMessenger::TheMessenger(Game* game)
	:BossEnemy(game, HEALTH, "Messenger", ddutil::YELLOW, Art::getBurningSkull(),
		std::vector<ColorString>{
			ColorString("\"You are no match for me, let alone ", ddutil::YELLOW) + ddutil::PATRIARCH_STRING + ColorString("!\"", ddutil::YELLOW)
		}
	)
{
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, BURN_DAMAGE, new BurntStatus(), BURN_LENGTH, 0, "", Strength::Powerful, 
		WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Vulnerable(VULNERABLE_LENGTH));
	moves.push_back(new EnemyMoves::Strike(SINGLE_TARGET_DAMAGE, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Strike(SINGLE_TARGET_DAMAGE, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Invulnerability(INVULNERABLE_LENGTH));

	turnCounter = 0;
}

EnemyTurn TheMessenger::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;


	chosenMove = moves[turnCounter];
	switch (turnCounter)
	{
	case 0: // burn
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" will ", ddutil::TEXT_COLOR) + ColorString("Burn", BurntStatus::COLOR) +
			ColorString(" everybody for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(BURN_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 1: // apply vulnerable
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" will make everybody ", ddutil::TEXT_COLOR) +
			ColorString("Vulnerable", VulnerableStatus::COLOR);
		break;
	case 2: // Strike
		targets.push_back(ddutil::getLowestHealthPlayer(players));
		intent = ddutil::genericDamageIntent(SINGLE_TARGET_DAMAGE, getColorString(), "Attack", targets);
		break;
	case 3: // Strike (again) but attack random
		targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
		intent = ddutil::genericDamageIntent(SINGLE_TARGET_DAMAGE, getColorString(), "Attack", targets);
		break;
	case 4: // Invulnerability
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("become ", ddutil::TEXT_COLOR) + ColorString("Invulnerable ", InvulnerableStatus::COLOR) +
			ColorString("for " + std::to_string(INVULNERABLE_LENGTH) + " turns", ddutil::TEXT_COLOR);
		break;
	}

	turnCounter++;
	if (turnCounter > 4)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* TheMessenger::makeCopy()
{
	return new TheMessenger(game);
}

int TheMessenger::getRoomId()
{
	return RoomId::TheMessengerEnemy;
}

TheWanderer::TheWanderer(Game* game)
	:BossEnemy(
		game,
		HEALTH,
		"Wanderer",
		Art::WANDERER_COLOR,
		Art::getTheWanderer(),
		std::vector<ColorString> {
			ColorString("I MAY NOT BE ABLE TO SEE YOU, BUT I SURE CAN SMELL YOU!!", Art::WANDERER_COLOR)
		}
	)
{
	turnCounter = 0;
	moves.push_back(new StatusAttackMove(
		MoveId::EnemyMoveId, START_STRANGLE_DAMAGE, new StrangledStatus(), START_STRANGLE_LENGTH, 0, "Strangle", Strength::Powerful, WavFile("attack6", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	));
	moves.push_back(new EnemyMoves::Block(BIG_BLOCK, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::HealStrike(LIFESTEAL_AOE_DAMAGE, LIFESTEAL_AOE_DAMAGE, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::StrikeAndBlock(BLOCK_STRIKE_DAMAGE, BLOCK_STRIKE_BLOCK, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new SimpleStatusMove(
		MoveId::EnemyMoveId, new StrangledStatus(), LATE_STRANGLE_LENGTH, true, 0, "Strangle", Strength::Moderate, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn TheWanderer::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;


	chosenMove = moves[turnCounter];
	switch (turnCounter)
	{
	case 0: // initial stun, never comes back to this 
		targets.push_back(ddutil::getHighestHealthPlayer(players));
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" will ", ddutil::TEXT_COLOR) + ColorString("Strangle", StrangledStatus::COLOR) +
			ColorString(" the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() + ColorString(" for ", ddutil::TEXT_COLOR) + 
			ColorString(std::to_string(START_STRANGLE_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 1: // big block
		targets.push_back(this);
		intent = ddutil::genericBlockIntent(BIG_BLOCK, getColorString());
		break;
	case 2: // lifesteal AOE
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to", ddutil::TEXT_COLOR) +
			ColorString(" suck " + std::to_string(LIFESTEAL_AOE_DAMAGE) + " HP ", ddutil::DAMAGE_COLOR) +
			ColorString(" from everybody", ddutil::TEXT_COLOR);
		break;
	case 3: // Strike and block
		targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to attack The ", ddutil::TEXT_COLOR) +
			targets.front()->getColorString() + ColorString(" for " + std::to_string(BLOCK_STRIKE_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) +
			ColorString("and ", ddutil::TEXT_COLOR) + ColorString("block " + std::to_string(BLOCK_STRIKE_BLOCK) + " damage", ddutil::BLOCK_COLOR);
		break;
	case 4: // Strangle a person
		targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("Strangle ", StrangledStatus::COLOR) + ColorString("The ", ddutil::TEXT_COLOR) +
			targets.front()->getColorString();
		break;
	}

	turnCounter++;
	if (turnCounter > 4)
	{
		turnCounter = 1;// dont repeat 0
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* TheWanderer::makeCopy()
{
	return new TheWanderer(game);
}

int TheWanderer::getRoomId()
{
	return RoomId::TheWandererEnemy;
}

TheMuncher::TheMuncher(Game* game)
	:BossEnemy(
		game,
		HEALTH,
		"Muncher",
		Art::MUNCHER_COLOR,
		Art::getTheMuncher(),
		std::vector<ColorString> { ColorString("You'll make a fine dinner tonight--if I don't eat you now!", Art::MUNCHER_COLOR)}
	)
{
	turnCounter = 0;
	rampage = false;
	moves.push_back(new EnemyMoves::AddBleedToAttacks(BLEED_AMOUNT));
	moves.push_back(new MultiAttackMove(MoveId::EnemyMoveId, BITE_DAMAGE, BITE_TIMES, 0, "", Strength::Powerful, WavFile("dualattack", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Strike(MULTI_ATTACK_DAMAGE, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Heal(HEAL));
	moves.push_back(new EnemyMoves::AddBleedToAttacks(BLEED_INC));
	moves.push_back(new EnemyMoves::StrikeAndBlock(MULTI_BLOCK_STRIKE_DAMAGE, MULTI_BLOCK_STRIKE_BLOCK, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn TheMuncher::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	if (!rampage && getHealth() < RAMPAGE_HEALTH)
	{
		rampage = true;
		turnCounter = 0;
	}

	if(rampage)
	{
		if (turnCounter > 2)
		{
			turnCounter = 0;
		}

		switch (turnCounter)
		{
		case 0:
			chosenMove = moves[4];
			targets.push_back(this);
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" is sharpening its teeth", ddutil::TEXT_COLOR);
			break;
		case 1:
			chosenMove = moves[1];
			targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to Bite the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
				ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(BITE_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) + 
				ColorString(std::to_string(BITE_TIMES) + " times", ddutil::TEXT_COLOR);
			break;
		case 2:
			chosenMove = moves[5];
			targets = players;
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to attack everybody for ", ddutil::TEXT_COLOR) +
				ColorString(std::to_string(MULTI_BLOCK_STRIKE_DAMAGE) + " damage", ddutil::DAMAGE_COLOR) +
				ColorString(" and ", ddutil::TEXT_COLOR) +
				ColorString("block " + std::to_string(MULTI_BLOCK_STRIKE_BLOCK*players.size()) + " damage", ddutil::BLOCK_COLOR);
			break;
		}
	}
	else
	{

		if (turnCounter > 3)
		{
			turnCounter = 1;
		}

		switch (turnCounter)
		{
		case 0:
			chosenMove = moves[0];
			targets.push_back(this);
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" is sharpening its teeth", ddutil::TEXT_COLOR);
			break;
		case 1:
			chosenMove = moves[1];
			targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to Bite the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
				ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(BITE_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) + 
				ColorString(std::to_string(BITE_TIMES) + " times", ddutil::TEXT_COLOR);
			break;
		case 2:
			chosenMove = moves[2];
			targets = players;
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to attack everybody for ", ddutil::TEXT_COLOR) +
				ColorString(std::to_string(MULTI_ATTACK_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
			break;
		case 3:
			chosenMove = moves[3];
			targets.push_back(this);
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to ", ddutil::TEXT_COLOR) +
				ColorString("heal " + std::to_string(HEAL) + " HP", ddutil::HEAL_COLOR);
			break;
		}
	}

	turnCounter++;


	return EnemyTurn(intent, targets, chosenMove);
}

Creature* TheMuncher::makeCopy()
{
	return new TheMuncher(game);
}

int TheMuncher::getRoomId()
{
	return RoomId::TheMuncherEnemy;
}


MysteriousKnight::MysteriousKnight(Game* game)
	:IntenseEnemy(game, HEALTH, "Mysterious Knight", Art::MYSTERIOUS_KNIGHT_COLOR, Art::getMysteriousKnight(),
		std::vector<ColorString>{
			ColorString("\"You should have bargained...\"", Art::MYSTERIOUS_KNIGHT_COLOR)
		}
	)
{
	moves.push_back(new EnemyMoves::Sacrifice(VULNERABLE_LENGTH));
	moves.push_back(new EnemyMoves::Strike(SINGLE_TARGET_DAMAGE, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	firstTurn = true;
}

EnemyTurn MysteriousKnight::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	if (firstTurn) // apply vulnerable to everybody on first turn
	{
		chosenMove = moves[0];
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is performing a blood sacrifice...", ddutil::TEXT_COLOR);

		firstTurn = false;
	}
	else // damage people for the rest of hte battle
	{
		chosenMove = moves[1];
		targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
		intent = ddutil::genericDamageIntent(SINGLE_TARGET_DAMAGE, getColorString(), "Strike", targets);
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* MysteriousKnight::makeCopy()
{
	return new MysteriousKnight(game);
}

int MysteriousKnight::getRoomId()
{
	return RoomId::MysteriousKnightEventFight;
}

Spider::Spider(Game* game)
	:IntenseEnemy(game, HEALTH, "Giant Spider", Art::SPIDER_COLOR, Art::getSpider(), 
		std::vector<ColorString> {ColorString("The Spider's roar sends shivers down your spine", Art::SPIDER_COLOR)})
{
	turnCounter = 0;
	moves.push_back(new EnemyMoves::CreateShield(BASE_BLOCK, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new SimpleStatusMove(MoveId::EnemyMoveId, new StrangledStatus(), STRANGLED_LENGTH, true, 0, "Strangle", Strength::Powerful,
		WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Strike(STRIKE_DAMAGE, WavFile("attack2", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn Spider::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	switch (turnCounter)
	{
	case 0:
		chosenMove = moves[0];
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to create a shield of webs", ddutil::TEXT_COLOR);
		break;
	case 1:
	{
		chosenMove = moves[1];
		Creature* target = ddutil::getHighestHealthPlayer(players);
		targets.push_back(target);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to strangle the ", ddutil::TEXT_COLOR) +
			target->getColorString();
		break;
	}
	case 2:
	case 3:
	{
		chosenMove = moves[2];
		Creature* target = players[ddutil::random(0, players.size() - 1)];
		targets.push_back(target);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to strike the ", ddutil::TEXT_COLOR) +
			target->getColorString() + ColorString(" for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(STRIKE_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	}

	}
	turnCounter++;
	if (turnCounter > 3)
	{
		turnCounter = 1;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* Spider::makeCopy()
{
	return new Spider(game);
}

int Spider::getRoomId()
{
	return RoomId::SpiderEventFight;
}

Minion::Minion(Game* game)
	:NormalEnemy(game, HEALTH, "Minion", Art::MINION_COLOR, Art::getMinion())
{
	moves.push_back(new EnemyMoves::CreateShield(BASE_BLOCK, WavFile("magicspell2", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::DualStrike(DUAL_STRIKE_DAMAGE, WavFile("dualattack", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::BlockBreakStrike(BLOCK_BREAK_DAMAGE, BLOCK_MULTIPLIER, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	turnCounter = 0;
}

EnemyTurn Minion::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	turnCounter++;
	if (turnCounter == 1)
	{
		chosenMove = moves[0];
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is creating a barrier", ddutil::TEXT_COLOR);
	}
	else
	{
		if (ddutil::random(1, 2) == 1) // dual strike a random person
		{
			chosenMove = moves[1];
			targets.push_back(players[ddutil::random(0, players.size() - 1)]);
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to attack The ", ddutil::TEXT_COLOR) +
				targets.front()->getColorString() +
				ColorString(" twice for ", ddutil::TEXT_COLOR) +
				ColorString(std::to_string(DUAL_STRIKE_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		}
		else // block break the lowest health person
		{
			chosenMove = moves[2];
			targets.push_back(ddutil::getLowestHealthPlayer(players));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to body slam into The ", ddutil::TEXT_COLOR) +
				targets.front()->getColorString() +
				ColorString(" for ", ddutil::TEXT_COLOR) +
				ColorString(std::to_string(BLOCK_BREAK_DAMAGE) + " damage", ddutil::DAMAGE_COLOR) +
				ColorString(" (x2 if breaking block)", ddutil::DAMAGE_COLOR);
		}
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* Minion::makeCopy()
{
	return new Minion(game);
}

int Minion::getRoomId()
{
	return RoomId::MinionEnemy;
}

MinionAlt::MinionAlt(Game* game)
	:NormalEnemy(game, HEALTH, "Minion", Art::ALT_MINION_COLOR, Art::getAltMinion())
{
	moves.push_back(new EnemyMoves::DualStrike(DUAL_STRIKE_DAMAGE, WavFile("dualattack", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::BlockBreakStrike(BLOCK_BREAK_DAMAGE, BLOCK_MULTIPLIER, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn MinionAlt::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	if (ddutil::random(1, 2) == 1) // dual strike a random person
	{
		chosenMove = moves[0];
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to attack The ", ddutil::TEXT_COLOR) +
			targets.front()->getColorString() +
			ColorString(" twice for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(DUAL_STRIKE_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
	}
	else // block break the lowest health person
	{
		chosenMove = moves[1];
		targets.push_back(ddutil::getLowestHealthPlayer(players));
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to Body Slam into The ", ddutil::TEXT_COLOR) +
			targets[0]->getColorString() +
			ColorString(" for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(BLOCK_BREAK_DAMAGE) + " damage", ddutil::DAMAGE_COLOR) +
			ColorString(" (x2 if hits block)", ddutil::DAMAGE_COLOR);
	}
	return EnemyTurn(intent, targets, chosenMove);
}

Creature* MinionAlt::makeCopy()
{
	return new MinionAlt(game);
}

int MinionAlt::getRoomId()
{
	return RoomId::MinionVariantEnemy;
}


GiantLizard::GiantLizard(Game* game)
	:NormalEnemy(game, HEALTH, "Giant Lizard", Art::GIANT_LIZARD_COLOR, Art::getGiantLizard())
{
	firstTurn = true;
	moves.push_back(new EnemyMoves::GetStatusImmunity(WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(
		new StatusAttackMove(MoveId::EnemyMoveId, 
			FLAME_DAMAGE, new BurntStatus(), BURN_LENGTH, 0,
			"Flamethrower", Strength::Powerful, 
			WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC)
		)
	);
	moves.push_back(new EnemyMoves::Strike(KICK_DAMAGE, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn GiantLizard::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	if (firstTurn)
	{
		chosenMove = moves[0];
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is curling up into a ball", ddutil::TEXT_COLOR);
		firstTurn = false;
	}
	else
	{
		// check if somebody could be kicked to death
		// if so kick them
		// if not burn everybody
		Creature* lowestHealth = ddutil::getLowestHealthPlayer(players);

		if (lowestHealth->getHealth() <= KICK_DAMAGE)
		{
			targets.push_back(lowestHealth);
			chosenMove = moves[2];
			intent = ddutil::genericDamageIntent(KICK_DAMAGE, getColorString(), "Kick", targets);
		}
		else
		{
			targets = players;
			chosenMove = moves[1];
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to ", ddutil::TEXT_COLOR) +
				ColorString("Burn", BurntStatus::COLOR) +
				ColorString(" everybody for ", ddutil::TEXT_COLOR) +
				ColorString(std::to_string(FLAME_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		}
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* GiantLizard::makeCopy()
{
	return new GiantLizard(game);
}

int GiantLizard::getRoomId()
{
	return RoomId::GiantLizardEnemy;
}

AltGiantLizard::AltGiantLizard(Game* game)
	:NormalEnemy(game, HEALTH, "Giant Lizard", Art::ALT_GIANT_LIZARD_COLOR, Art::getAltGiantLizard())
{
	firstTurn = true;
	target = 0;
	moves.push_back(new EnemyMoves::CreateThorns(THORNS_DAMAGE));
	moves.push_back(new EnemyMoves::Strike(KICK_DAMAGE, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn AltGiantLizard::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	if (firstTurn)
	{
		chosenMove = moves[0];
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is curling up into a ball", ddutil::TEXT_COLOR);
		firstTurn = false;
	}
	else
	{
		targets.push_back(players[target]);
		chosenMove = moves[1];
		intent = ddutil::genericDamageIntent(KICK_DAMAGE, getColorString(), "Kick", targets);
		
		target++;
		if (target >= static_cast<int>(players.size()))
		{
			target = 0;
		}
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* AltGiantLizard::makeCopy()
{
	return new AltGiantLizard(game);
}

int AltGiantLizard::getRoomId()
{
	return RoomId::GiantLizardVariantEnemy;
}

TheCollector::TheCollector(Game* game)
	:BossEnemy(
		game, HEALTH, "Destructor", Art::COLLECTOR_COLOR, Art::getCollector(),
		std::vector<ColorString> {
			ColorString("\"Resistance is futile", Art::COLLECTOR_COLOR),
			ColorString("",ddutil::TEXT_COLOR) + ddutil::PATRIARCH_STRING + ColorString(" will destroy this world!\"", Art::COLLECTOR_COLOR)
		}
	)
{
	turnCounter = 0;
	moves.push_back(new EnemyMoves::CreateShield(SHIELD_BLOCK, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, FIRE_DAMAGE, new BurntStatus(), BURN_LENGTH, 0, "", Strength::Powerful, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::StrikeAndBlock(BLOCKSTRIKE_DAMAGE, BLOCKSTRIKE_BLOCK, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new SimpleAttackMove(MoveId::EnemyMoveId, THROUGH_BLOCK_DAMAGE, true, 0, "", Strength::Powerful, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Strike(HYPER_BEAM_DAMAGE, WavFile("magicattack3", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn TheCollector::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	switch (turnCounter)
	{
	case 0:
		chosenMove = moves[0];
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is creating a ", ddutil::TEXT_COLOR) +
			ColorString("Shield", ddutil::BLOCK_COLOR);
		break;
	case 1:
		chosenMove = moves[1];
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" will ", ddutil::TEXT_COLOR) + ColorString("Burn", BurntStatus::COLOR) +
			ColorString(" everybody for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(FIRE_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 2:
		chosenMove = moves[2];
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to Strike everybody for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(BLOCKSTRIKE_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) + ColorString(" and block ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(BLOCKSTRIKE_BLOCK * targets.size()) + " damage ", ddutil::BLOCK_COLOR) +
			ColorString("next turn", ddutil::TEXT_COLOR);
		break;
	case 3:
		chosenMove = moves[3];
		targets.push_back(ddutil::getLowestHealthPlayer(players));
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" will Strike The ", ddutil::TEXT_COLOR) +
			targets[0]->getColorString() + ColorString(" through block for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(THROUGH_BLOCK_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 4:
		chosenMove = moves[4];
		targets.push_back(ddutil::getHighestHealthPlayer(players));
		intent = ddutil::genericDamageIntent(HYPER_BEAM_DAMAGE, getColorString(), "Blast", targets);
		break;
	}

	turnCounter++;
	if (turnCounter > 4)
	{
		turnCounter = 1; // reset to begining but dont redo more thorns
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* TheCollector::makeCopy()
{
	return new TheCollector(game);
}

int TheCollector::getRoomId()
{
	return RoomId::TheCollectorEnemy;
}

TheArchitect::TheArchitect(Game* game)
	:BossEnemy(
		game,
		HEALTH,
		"Architect",
		Art::ARCHITECT_COLOR,
		Art::getTheArchitect(),
		std::vector<ColorString> {ColorString("\"You will not destroy all that I have worked so hard to construct!\"", Art::ARCHITECT_COLOR)}
	)
{
	turnCounter = 0;
	halfHealth = false;
	moves.push_back(new EnemyMoves::AddScorchToAttacks(SCORCH_PER_ATTACK));
	moves.push_back(new StatusAttackMove(
		MoveId::EnemyMoveId, FLAMETHROWER_DAMAGE, new BurntStatus(), FLAMETHROWER_BURN, 0, "", Strength::Moderate, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC))
	);
	moves.push_back(new MultiAttackMove(MoveId::EnemyMoveId, DOUBLE_TAP_DAMAGE, DOUBLE_TAP_TIMES, 0, "", Strength::Moderate, WavFile("dualattack", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::CreateShield(INCREASE_SHIELD, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	// half health moves
	moves.push_back(new SimpleStatusMove(
		MoveId::EnemyMoveId, new EntombedStatus(), ENTOMBED_AMOUNT, true, 0, "", Strength::Mythical, WavFile("lightning", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	));
	moves.push_back(new EnemyMoves::Heal(HEAL));
}

EnemyTurn TheArchitect::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	if (!halfHealth && getHealth() < HP_THRESHOLD)
	{
		halfHealth = true;
		turnCounter = 0;
	}

	if (halfHealth)
	{
		switch (turnCounter)
		{
		case 0:
			chosenMove = moves[4];
			targets = players;
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" is ", ddutil::TEXT_COLOR) +
				ColorString("Entombing", EntombedStatus::COLOR) + ColorString(" everybody!", ddutil::TEXT_COLOR);
			break;
		case 1:
			chosenMove = moves[5];
			targets.push_back(this);
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" will heal itself for ", ddutil::TEXT_COLOR) +
				ColorString(std::to_string(HEAL) + " HP", ddutil::HEAL_COLOR);
			break;
		}

		turnCounter = 1; // stay on the heal move
	}
	else
	{
		switch (turnCounter)
		{
		case 0:
			chosenMove = moves[0];
			targets.push_back(this);
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" is enveloping itself in flames!", ddutil::TEXT_COLOR);
			break;
		case 1:
			chosenMove = moves[1];
			targets = players;
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" targets everyone with a ", ddutil::TEXT_COLOR) +
				ColorString("Flame Blast", BurntStatus::COLOR) + ColorString(", dealing " + std::to_string(FLAMETHROWER_DAMAGE) + " damage", ddutil::DAMAGE_COLOR) +
				ColorString(" and ", ddutil::TEXT_COLOR) + ColorString("applying " + std::to_string(FLAMETHROWER_BURN) + " Burnt", BurntStatus::COLOR);
			break;
		case 2:
			chosenMove = moves[2];
			targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to tap the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
				ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(DOUBLE_TAP_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) + 
				ColorString(std::to_string(DOUBLE_TAP_TIMES) + " times", ddutil::TEXT_COLOR);
			break;
		case 3:
			chosenMove = moves[3];
			targets.push_back(this);
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" is creating a shield!", ddutil::BLOCK_COLOR);
			break;
		case 4:
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" is recharging...", ddutil::TEXT_COLOR);
			break;
		}
		
		turnCounter++;
		if (turnCounter > 4)
		{
			turnCounter = 1;
		}
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* TheArchitect::makeCopy()
{
	return new TheArchitect(game);
}

int TheArchitect::getRoomId()
{
	return RoomId::TheArchitectEnemy;
}

TheHarbinger::TheHarbinger(Game* game)
	:BossEnemy(
		game,
		HEALTH,
		"Harbinger",
		Art::HARBINGER_COLOR,
		Art::getTheHarbinger(),
		std::vector<ColorString> {ColorString("\"Your doom approaches...\"", Art::HARBINGER_COLOR)}
	)
{
	turnCounter = 0;
	moves.push_back(new SimpleStatusMove(
		MoveId::EnemyMoveId, new StormStatus(), STORM_LENGTH, true, 0, "", Strength::Mythical, WavFile("blackhole", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	));
	moves.push_back(new StatusAttackMove(
		MoveId::EnemyMoveId, LIGHTNING_DAMAGE, new ZappedStatus(), LIGHTNING_ZAPPED, 0, "", Strength::Mythical, WavFile("lightning", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	));
	moves.push_back(new StatusAttackMove(
		MoveId::EnemyMoveId, MULTI_STRIKE_DAMAGE, new ZappedStatus(), MULTI_STRIKE_ZAPPED, 0, "", Strength::Mythical, WavFile("electricattack2", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	));
	moves.push_back(new SimpleStatusMove(
		MoveId::EnemyMoveId, new BeserkedStatus(), BESERK_LENGTH, false, 0, "", Strength::Moderate, WavFile("warhorn", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	));
}

EnemyTurn TheHarbinger::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	chosenMove = moves[turnCounter];
	switch (turnCounter)
	{
	case 0:
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" is creating a ", ddutil::TEXT_COLOR) +
			ColorString("Storm!", StormStatus::COLOR);
		break;
	case 1:
		targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
		intent = ddutil::genericDamageIntent(LIGHTNING_DAMAGE, getColorString(), "Smite", targets) + ColorString(", applying ", ddutil::TEXT_COLOR) +
			ColorString("Zapped (" +std::to_string(LIGHTNING_ZAPPED) + ")", ZappedStatus::COLOR);
		break;
	case 2:
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("Zap", ZappedStatus::COLOR) + ColorString(" everybody for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(MULTI_STRIKE_DAMAGE) + " damage ", ddutil::TEXT_COLOR) + ColorString("and ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(MULTI_STRIKE_ZAPPED) + " Zapped", ZappedStatus::COLOR);
		break;
	case 3:
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" is going ", ddutil::TEXT_COLOR) +
			ColorString("Beserk!", BeserkedStatus::COLOR);
		break;
	}

	turnCounter++;
	if (turnCounter > 3)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* TheHarbinger::makeCopy()
{
	return new TheHarbinger(game);
}

int TheHarbinger::getRoomId()
{
	return RoomId::TheHarbingerEnemy;
}

AbyssBeast::AbyssBeast(Game* game)
	:HardEnemy(game, HEALTH, "Abyssal Beast", Art::ABYSS_BEAST_COLOR, Art::getAbyssBeast())
{
	moves.push_back(new EnemyMoves::TakeVitalityGain(VITALITY_CHANGE));
	moves.push_back(
		new SelfDamageAttackMove(MoveId::EnemyMoveId, ATTACK_DAMAGE, false, SELF_DAMAGE, 0, "", Strength::Weak, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)
		)
	);

	firstTurn = true;
}

EnemyTurn AbyssBeast::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	if (firstTurn)
	{
		firstTurn = false;
		chosenMove = moves[0];
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is stealing everybody's", ddutil::TEXT_COLOR) +
			ColorString(" Vitality Gain", ddutil::VITALITY_COLOR);
		targets = players;
	}
	else
	{
		chosenMove = moves[1];
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ddutil::genericDamageIntent(ATTACK_DAMAGE, getColorString(), "Maul", targets);
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* AbyssBeast::makeCopy()
{
	return new AbyssBeast(game);
}

int AbyssBeast::getRoomId()
{
	return RoomId::AbyssBeastEnemy;
}

Basilisk::Basilisk(Game* game)
	:HardEnemy(game, HEALTH, "Basilisk", Art::BASILISK_COLOR, Art::getBasilisk())
{
	turnCounter = 0;
	moves.push_back(
		new SimpleStatusMove(MoveId::EnemyMoveId,  
			new HexedStatus(), HEXED_LENGTH,
			true, 0, "", Strength::Powerful,
			WavFile("ghostinvulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC)
		)
	);
	moves.push_back(
		new SimpleStatusMove(MoveId::EnemyMoveId, 
			new PoisonedStatus(), POISON_AMOUNT,
			true, 0, "", Strength::Powerful,
			WavFile("energyattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)
		)
	);
	moves.push_back(new EnemyMoves::Strike(STRANGLE_DAMAGE, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn Basilisk::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	chosenMove = moves[turnCounter];
	switch (turnCounter)
	{
	case 0:
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ColorString("The ", ddutil::TEXT_COLOR) +
			getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("Hex", HexedStatus::COLOR) +
			ColorString(" The ", ddutil::TEXT_COLOR) +
			targets[0]->getColorString();
		break;
	case 1:
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) +
			getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("Poison", PoisonedStatus::COLOR) +
			ColorString(" Everybody ", ddutil::TEXT_COLOR);
		break;
	case 2:
		targets.push_back(ddutil::getLowestHealthPlayer(players));
		intent = ddutil::genericDamageIntent(STRANGLE_DAMAGE, getColorString(), "Strangle", targets);
		break;
	}

	turnCounter++;
	if (turnCounter > 2)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* Basilisk::makeCopy()
{
	return new Basilisk(game);
}

int Basilisk::getRoomId()
{
	return RoomId::BasiliskEnemy;
}


IntenseHardEnemy::IntenseHardEnemy(Game* game, int maxHp, std::string name, int color, Picture pic, std::vector<ColorString> intro)
	:Enemy(game, maxHp, name, color, pic, Mp3File("intensehardbattle"), intro, ddutil::MAP_SENEMY)
{
}

void IntenseHardEnemy::deathScene()
{
	Enemy::deathScene();
	game->displayInfo();
	game->changeScore(ddutil::STRONG_ENEMY_SCORE);

	VirtualWindow* vwin = game->getVWin();
	Artifact* artifact = game->getActiveZone()->getRandomArtifact();
	int line = ddutil::CONSOLEY / 2;
	vwin->putcen(ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" drops a ", ddutil::TEXT_COLOR) +
		artifact->getName(), line - 2);
	game->artifactSelectionMenu(line, artifact);
	game->clearCenterScreen();
}

IntenseEnemy::IntenseEnemy(Game* game, int maxHp, std::string name, int color, Picture pic, std::vector<ColorString> intro)
	: Enemy(game, maxHp, name, color, pic, Mp3File("intensebattle"), intro, ddutil::MAP_NENEMY)
{
}

void IntenseEnemy::deathScene()
{
	Enemy::deathScene();
	game->displayInfo();
	game->changeScore(ddutil::NORM_ENEMY_SCORE);

	std::vector<Artifact*> choices = Artifact::getRandomCommonArtifacts(game);
	std::vector<ColorString> menuOptions;
	menuOptions.push_back(ColorString("Skip", ddutil::TEXT_COLOR));
	menuOptions.push_back(ColorString("View Menu", ddutil::TEXT_COLOR));
	for (Artifact* a : choices)
	{
		menuOptions.push_back(a->getFullInformation());
	}
	while (true)
	{
		game->clearCenterScreen();
		game->clearBottomDivider();
		game->displayInfo();
		game->getVWin()->putcenSlowScroll(ColorString("The ", ddutil::ARTIFACT_COLOR) + getColorString() + 
			ColorString(" drops some artifacts. Choose one:", ddutil::ARTIFACT_COLOR), ddutil::EVENT_PICTURE_LINE-2);

		Menu menu(game->getVWin(), menuOptions, Coordinate(0, ddutil::EVENT_PICTURE_LINE), true);
		if (menu.getResponse() == 0)
		{
			break;
		}
		else if (menu.getResponse() == 1)
		{
			game->viewMenu();
		}
		else // selected an artifact
		{
			int index = menu.getResponse() - 2;
			Artifact* chosen = choices.at(index);
			choices.erase(choices.begin() + index);
			game->clearCenterScreen();
			game->artifactSelectionMenu(ddutil::EVENT_PICTURE_LINE, chosen);
			break;
		}
	}

	// delete the artifacts which are left, which will be all 3 if skipped, or just the 2 that weren't picked
	for (Artifact* a : choices)
	{
		delete a;
	}
	game->clearCenterScreen();
	game->displayInfo();
}

SentientMouth::SentientMouth(Game* game)
	: IntenseEnemy(game, HEALTH, "Sentient Mouth", Art::MOUTH_COLOR, Art::getFloatingMouth(), 
		std::vector<ColorString> {
			ColorString("\"[unintelligible screaming]\"", Art::Art::MOUTH_COLOR)
		}
	)
{
	turnCounter = 0;
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, HEX_DAMAGE, new HexedStatus(), HEX_LENGTH, 0, "", Strength::Mythical, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new SimpleAttackMove(MoveId::EnemyMoveId, LICK_DAMAGE,false, 0, "", Strength::Mythical, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, FROSTBURN_DAMAGE, new FrostBurntStatus(), FROSTBURN_LENGTH, 0, "", Strength::Mythical, WavFile("freeze", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn SentientMouth::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	chosenMove = moves[turnCounter];
	switch (turnCounter)
	{
	case 0:
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) +
			getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("Hex", HexedStatus::COLOR) + ColorString(" everybody for " + std::to_string(HEX_LENGTH) + " turns and ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(HEX_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 1:
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ddutil::genericDamageIntent(LICK_DAMAGE, getColorString(), "Lick", targets);
		break;
	case 2:
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) +
			getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("Frostburn", FrostBurntStatus::COLOR) + ColorString(" everybody for " + std::to_string(FROSTBURN_LENGTH) + " turns and ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(FROSTBURN_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	}

	turnCounter++;
	if (turnCounter > 2)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* SentientMouth::makeCopy()
{
	return new SentientMouth(game);
}

int SentientMouth::getRoomId()
{
	return RoomId::SentientMouthEnemy;
}

LaughingMask::LaughingMask(Game* game)
	: IntenseEnemy(game, HEALTH, "Laughing Mask", Art::MASK_COLOR, Art::getLaughingMask(),
		std::vector<ColorString> {
			ColorString("*The mask laughs menancingly*", Art::MASK_COLOR)
		}
	)
{
	turnCounter = 0;
	moves.push_back(new SimpleStatusMove(MoveId::EnemyMoveId, new VulnerableStatus(), VULN_LENGTH, true,0,"",Strength::Mythical, WavFile("laugh", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, HEX_DAMAGE, new HexedStatus(), HEX_LENGTH, 0, "", Strength::Mythical, WavFile("magicspell1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::StrikeAndBlock(HEAD_BUTT_DAM, HEAD_BUTT_BLOCK, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn LaughingMask::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	chosenMove = moves[turnCounter];
	switch (turnCounter)
	{
	case 0:
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) +
			getColorString() + ColorString(" is just laughing!", ddutil::TEXT_COLOR);
		break;
	case 1:
		targets.push_back(ddutil::getHighestHealthPlayer(players));
		intent = ColorString("The ", ddutil::TEXT_COLOR) +
			getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("Hex", HexedStatus::COLOR) + ColorString(" The ", ddutil::TEXT_COLOR) +
			targets[0]->getColorString() + ColorString(" for "+ std::to_string(HEX_LENGTH) + " turns and ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(HEX_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 2:
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ColorString("The ", ddutil::TEXT_COLOR) +
			getColorString() + ColorString(" intends to Headbutt The ", ddutil::TEXT_COLOR) + targets[0]->getColorString() +
			ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(HEAD_BUTT_DAM) + " damage ", ddutil::DAMAGE_COLOR) +
			ColorString(" and ", ddutil::TEXT_COLOR) + ColorString("block " + std::to_string(HEAD_BUTT_BLOCK) + " damage", ddutil::BLOCK_COLOR);
		break;
	}

	turnCounter++;
	if (turnCounter > 2)
	{
		turnCounter = 2;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* LaughingMask::makeCopy()
{
	return new LaughingMask(game);
}

int LaughingMask::getRoomId()
{
	return RoomId::LaughingMaskEnemy;
}

DevilishMask::DevilishMask(Game* game)
	: IntenseEnemy(game, HEALTH, "Devilish Mask", Art::DMASK_COLOR, Art::getDevilishMask(),
		std::vector<ColorString> {
			ColorString("\"Fear me.\"", Art::DMASK_COLOR)
		}
	)
{
	turnCounter = 0;
	moves.push_back(new EnemyMoves::Burn(BURN_LENGTH));
	moves.push_back(new SorcererMoves::Combust());
}

EnemyTurn DevilishMask::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	chosenMove = moves[turnCounter];
	switch (turnCounter)
	{
	case 0:
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) +
			getColorString() + ColorString(" intends to ", ddutil::TEXT_COLOR) +
			ColorString("Burn", BurntStatus::COLOR) + ColorString(" everybody", ddutil::TEXT_COLOR);
		break;
	case 1:
		Creature * highestBurnPlayer = players[0];
		for (auto p : players)
		{
			if (p->getNumberOfStatuses(StatusID::Burnt) > highestBurnPlayer->getNumberOfStatuses(StatusID::Burnt))
			{
				highestBurnPlayer = p;
			}
		}

		targets.push_back(highestBurnPlayer);
		intent = ColorString("The ", ddutil::TEXT_COLOR) +
			getColorString() + ColorString(" intends to deal ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(SorcererMoves::Combust::DAM_PER_BURN) + " damage per ", ddutil::DAMAGE_COLOR) +
			ColorString("Burn", BurntStatus::COLOR) + ColorString(" to The ", ddutil::TEXT_COLOR) +
			highestBurnPlayer->getColorString();
		break;
	}

	turnCounter++;
	if (turnCounter > 1)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* DevilishMask::makeCopy()
{
	return new DevilishMask(game);
}

int DevilishMask::getRoomId()
{
	return RoomId::DevilishMaskEnemy;
}

PossessedMace::PossessedMace(Game* game)
	: IntenseEnemy(game, HEALTH, "Possessed Mace", Art::MACE_COLOR, Art::getMace(),
		std::vector<ColorString> {
			ColorString("Suddenly, the Mace springs to life...", Art::MACE_COLOR)
		}
	)
{
	applyStatus(new ThornsStatus(), THORNS);

	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, ST_DAMAGE, new BleedingStatus(), BLEED_AMOUNT, 0, "", Strength::Mythical, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, MT_DAMAGE, new BleedingStatus(), BLEED_AMOUNT, 0, "", Strength::Mythical, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn PossessedMace::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	switch (ddutil::random(0, 1))
	{
	case 0:
		chosenMove = moves[0];
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ColorString("The ", ddutil::TEXT_COLOR) +
			getColorString() + ColorString(" intends to Strike The ", ddutil::TEXT_COLOR) +
			targets[0]->getColorString() + ColorString(" for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(ST_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 1:
		chosenMove = moves[1];
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) +
			getColorString() + ColorString(" intends to Strike everybody for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(MT_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* PossessedMace::makeCopy()
{
	return new PossessedMace(game);
}

int PossessedMace::getRoomId()
{
	return RoomId::PossessedMaceEnemy;
}

HyperBeast::HyperBeast(Game* game)
	: IntenseHardEnemy(game, HEALTH, "Hyper Beast", Art::HYPER_BEAST_COLOR, Art::getHyperBeast(),
		std::vector<ColorString> {
	ColorString("Your skin starts to crawl...", FrostBurntStatus::COLOR)
}
)
{
	turnCounter = 0;
	alreadyStoleVitality = false;
	moves.push_back(
		new StatusAttackMove(MoveId::EnemyMoveId, HYPER_BEAM_DAM, new FrostBurntStatus(), FROSTBURN_LENGTH, 0, "", Strength::Mythical, WavFile("magicattack3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
	);
	moves.push_back(new EnemyMoves::TakeVitalityGain(VIT_CHANGE));
	moves.push_back(new EnemyMoves::Heal(HEAL_AMOUNT));
}

EnemyTurn HyperBeast::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	
	switch (turnCounter)
	{
	case 0:
		chosenMove = moves[0];
		targets.push_back(ddutil::getHighestHealthPlayer(players));
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is pointing its Hyper Beam at The ", ddutil::TEXT_COLOR) +
			targets[0]->getColorString() + ColorString(" (" + std::to_string(HYPER_BEAM_DAM) + " damage)", ddutil::DAMAGE_COLOR);
		break;
	case 1:
		if (alreadyStoleVitality) // heal 20 hp
		{
			chosenMove = moves[2];
			targets.push_back(this);
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to ", ddutil::TEXT_COLOR) +
				ColorString("heal " + std::to_string(HEAL_AMOUNT) + " HP", ddutil::HEAL_COLOR);
		}
		else // take 1 vitality per turn
		{
			chosenMove = moves[1];
			targets = players;
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" is stealing everybody's", ddutil::TEXT_COLOR) +
				ColorString(" Vitality Gain", ddutil::VITALITY_COLOR);
			alreadyStoleVitality = true;
		}
		break;
	case 2:
		chosenMove = moves[2]; // set a move but put no targets so nothing actually happens
		// targets is empty
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is charging up a Hyper Beam", ddutil::TEXT_COLOR);
		break;
	}

	turnCounter++;
	if (turnCounter > 2)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* HyperBeast::makeCopy()
{
	return new HyperBeast(game);
}

int HyperBeast::getRoomId()
{
	return RoomId::HyperBeastEnemy;
}

AncientBird::AncientBird(Game* game)
	: IntenseHardEnemy(game, HEALTH, "Ancient Bird", Art::ANC_BIRD_COLOR, Art::getAncientBird(),
		std::vector<ColorString> {
			ColorString("The Ancient Bird roars loudly!", Art::ANC_BIRD_COLOR),
		})
{
	turnCounter = 0;
	moves.push_back(new SimpleStatusMove(MoveId::EnemyMoveId, new InvulnerableStatus(), INVULN_TURNS, false, 0, "", Strength::Mythical, WavFile("fly", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::TakeVitality(VITALITY_STEAL));
	moves.push_back(new EnemyMoves::Strike(DIVEBOMB_DAMAGE, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::DualStrike(DUALSTRIKE_DAM, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn AncientBird::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = moves[turnCounter];
	ColorString intent;
	std::vector<Creature*> targets;

	switch (turnCounter)
	{
	case 0:
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is attempting to fly high up", ddutil::TEXT_COLOR);
		break;
	case 1:
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is reducing everybody's ", ddutil::TEXT_COLOR) +
			ColorString("Vitality", ddutil::VITALITY_COLOR) +
			ColorString(" by " + std::to_string(VITALITY_STEAL), ddutil::TEXT_COLOR);
		break;
	case 2:
		targets.push_back(ddutil::getLowestHealthPlayer(players));
		intent = ddutil::genericDamageIntent(DIVEBOMB_DAMAGE, getColorString(), "Divebomb", targets);
		break;
	case 3:
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to strike The ", ddutil::TEXT_COLOR) +
			targets[0]->getColorString() + ColorString(" for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(DUALSTRIKE_DAM) + " damage ", ddutil::DAMAGE_COLOR) +
			ColorString("twice", ddutil::TEXT_COLOR);
		break;
	}

	turnCounter++;
	if (turnCounter > 3)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* AncientBird::makeCopy()
{
	return new AncientBird(game);
}

int AncientBird::getRoomId()
{
	return RoomId::AncientBirdEnemy;
}

Chapter3Boss::Chapter3Boss(Game* game, int maxHp, std::string name, int color, Picture pic, std::vector<ColorString> intro)
	:BossEnemy(game, maxHp, name, color, pic, intro)
{
}

void Chapter3Boss::deathScene()
{
	game->changeScore(ddutil::BOSS_ENEMY_SCORE);

	VirtualWindow* vwin = game->getVWin();

	game->clearCenterScreen();
	game->clearBottomDivider();

	Coordinate coord = Coordinate(0, ddutil::CENTER_TEXT_LINE - 2);

	vwin->printArtFromBottom(picture, coord, true);
	playSound(WavFile("fadeaway", false, true));
	for (int i = 0; i < 10; i++)
	{
		vwin->printArtFromBottom(picture, coord, true);
		Sleep(225);
		game->clearCenterScreen();
		Sleep(225);
	}
	vwin->clearScreen();

	int bottomLine = ddutil::CENTER_TEXT_LINE;
	vwin->putcenSlowScroll(ColorString("The ", ddutil::TEXT_COLOR)+ getColorString() + ColorString(" fades away....", ddutil::TEXT_COLOR), bottomLine);
	Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, bottomLine + 1), true);
	vwin->clearLine(bottomLine); vwin->clearLine(bottomLine + 1);

	vwin->printArtFromBottom(Art::getPortal(), coord, true);
	vwin->putcenSlowScroll(ColorString("In its place a portal appears", ddutil::TEXT_COLOR), bottomLine);
	Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, bottomLine + 1), true);
	vwin->clearLine(bottomLine); vwin->clearLine(bottomLine + 1);


	vwin->putcenSlowScroll(ColorString("You enter the portal, ",ddutil::TEXT_COLOR)+ColorString("and everybody recovers to Max HP", ddutil::HEAL_COLOR), bottomLine);
	playSound(WavFile("enterportal", false, false));
	Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, bottomLine + 1), true);
	vwin->clearLine(bottomLine); vwin->clearLine(bottomLine + 1);
	for (Player* p : game->getPlayerParty())
	{
		p->increaseHealth(999);
	}
	vwin->clearScreen();

	vwin->putcenSlowScroll(ColorString("And on the other side...", ddutil::TEXT_COLOR), bottomLine);
	Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, bottomLine + 1), true);
	vwin->clearLine(bottomLine); vwin->clearLine(bottomLine + 1);

	vwin->clearScreen();
	vwin->printArtFromBottom(Art::getPalace(), coord, true);

	vwin->putcenSlowScroll(ColorString("Lies the Patriarch's Eternal Palace...", ddutil::TEXT_COLOR), bottomLine, true);
	Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, bottomLine + 1), true);
	vwin->clearLine(bottomLine); vwin->clearLine(bottomLine + 1);

	game->setSkipToNextChapter();

	vwin->clearScreen();
}

TheBetrayer::TheBetrayer(Game* game)
	:Chapter3Boss(
		game,
		HEALTH,
		"Betrayer", // make sure to change below too if this changes
		Art::BETRAYER_COLOR,
		Art::getTheBetrayer(Art::BETRAYER_NORM_COLOR),
		std::vector<ColorString> {
			ColorString("\"I will turn your strength against you...\"", Art::BETRAYER_COLOR),
			ColorString("The ", ddutil::TEXT_COLOR) + ColorString("Betrayer", Art::BETRAYER_COLOR) + ColorString(" will apply ", ddutil::TEXT_COLOR) +
				ColorString("Judgement", JudgementStatus::COLOR) + ColorString(" on every attack.", ddutil::TEXT_COLOR)
		}
	)
{
	turnCounter = -1;
	strength = 0;
	absorbingStrength = false;
	addAttackStatus(new JudgementStatus(), JUDGEMENT_PER_ATTACK);

	moves.push_back(new EnemyMoves::Strike(BASE_ABSORB_DAMAGE, WavFile("attack6", ddutil::SF_LOOP, ddutil::SF_ASYNC)));

	moves.push_back(new EnemyMoves::HealStrike(LIFE_STEAL_AMOUNT, LIFE_STEAL_AMOUNT, WavFile("attackheal", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::TakeVitalityGain(VITGAIN_STEAL));
	moves.push_back(new EnemyMoves::Heal(HEAL));
	moves.push_back(new MultiAttackMove(
		MoveId::EnemyMoveId, SLICE_DAMAGE, SLICE_TIMES, 0, "", Strength::Mythical, WavFile("dualenergyattack", ddutil::SF_LOOP, ddutil::SF_ASYNC))
	);
}

EnemyTurn TheBetrayer::getTurn(std::vector<Creature*> players)
{
	ColorString intent;
	std::vector<Creature*> targets;
	Move* chosenMove = nullptr;

	switch (turnCounter)
	{
	case -1:
		chosenMove = moves[2];
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is stealing everybody's", ddutil::TEXT_COLOR) +
			ColorString(" Vitality Gain", ddutil::VITALITY_COLOR);
		break;
	case 0:
		absorbingStrength = true;
		changePicture(Art::getTheBetrayer(Art::BETRAYER_STRGAIN_COLOR));
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" will gain ", ddutil::TEXT_COLOR) +
			ColorString("strength", ddutil::DAMAGE_COLOR) + ColorString(" for every HP lost this turn!", ddutil::TEXT_COLOR);
		break;
	case 1:
	{
		absorbingStrength = false;
		auto scalingAttack = dynamic_cast<EnemyMoves::Strike*>(moves[0]);
		scalingAttack->resetStrength(BASE_ABSORB_DAMAGE);
		if (scalingAttack != nullptr)
		{
			scalingAttack->increaseStrength(strength);
		}
		strength = 0;
		chosenMove = moves[0];
		targets.push_back(ddutil::getHighestHealthPlayer(players));
		intent = ddutil::genericDamageIntent(scalingAttack->getStrength(), getColorString(), "Eviscerate", targets);
	}
		break;
	case 2:
		changePicture(Art::getTheBetrayer(Art::BETRAYER_NORM_COLOR));
		int random = ddutil::random(1, 2);
		if (random == 1) // heal strike
		{
			chosenMove = moves[1];
			targets = players;
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to", ddutil::TEXT_COLOR) +
				ColorString(" steal " + std::to_string(LIFE_STEAL_AMOUNT) + " HP ", ddutil::DAMAGE_COLOR) +
				ColorString(" from everybody", ddutil::TEXT_COLOR);
		}
		else if (random == 2 && getHealth() < getMaxHealth(HEAL_PERCENT)) // heal
		{
			chosenMove = moves[3];
			targets.push_back(this);
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" will heal " + std::to_string(HEAL) + " HP", ddutil::HEAL_COLOR);
		}
		else // multi attack
		{
			chosenMove = moves[4];
			targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to Slice the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
				ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(SLICE_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) + 
				ColorString(std::to_string(SLICE_TIMES) + " times", ddutil::TEXT_COLOR);
		}
		break;
	}

	turnCounter++;
	if (turnCounter > 2)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

ColorString TheBetrayer::getStatLine()
{
	return Enemy::getStatLine() + ColorString("STR: " + std::to_string(strength), ddutil::DAMAGE_COLOR);
}

Creature* TheBetrayer::makeCopy()
{
	return new TheBetrayer(game);
}

int TheBetrayer::getRoomId()
{
	return RoomId::TheBetrayerEnemy;
}

void TheBetrayer::doMiscDamageEffects(int damage)
{
	if (absorbingStrength)
	{
		strength += damage;
	}
}
TheProtector::TheProtector(Game* game)
	:Chapter3Boss(
		game, HEALTH, "Protector", Art::PROTECTOR_COLOR, Art::getTheProtector(),
		std::vector<ColorString> {
			ColorString("\"CAN'T\"", Art::PROTECTOR_COLOR),
			ColorString("\"LET\"", Art::PROTECTOR_COLOR),
			ColorString("\"REACH\"", Art::PROTECTOR_COLOR),
			ColorString("\"MASTER\"", Art::PROTECTOR_COLOR),
		}
	)
{
	turnCounter = 0;
	shieldCounter = 0;
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, ICE_BEAM_DAM, new FrostBurntStatus(), BURN_LENGTH, 0, "", Strength::Mythical, WavFile("freeze", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, HEX_DAMAGE, new HexedStatus(), HEX_LENGTH, 0, "", Strength::Mythical, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::CreateShield(SHIELD_INCREASE, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new BlockBreakingMove(MoveId::EnemyMoveId, BLOCK_BREAK_DAMAGE, BLOCK_BREAK_MULT, 0, "", Strength::Mythical, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Heal(SMALL_HEAL_AMOUNT));
	moves.push_back(new EnemyMoves::Heal(BIG_HEAL_AMOUNT));
}

EnemyTurn TheProtector::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;


	if (getHealth() <= getMaxHealth(BIG_HEAL_THRESHOLD))
	{
		chosenMove = moves[5];
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" will ", ddutil::TEXT_COLOR) +
			ColorString("heal " + std::to_string(BIG_HEAL_AMOUNT) + " HP", ddutil::HEAL_COLOR);
	}
	else
	{
		switch (turnCounter)
		{
		case 0:
			chosenMove = moves[0];
			targets = players;
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" will ", ddutil::TEXT_COLOR) + ColorString("Frostburn", FrostBurntStatus::COLOR) +
				ColorString(" everybody for ", ddutil::TEXT_COLOR) +
				ColorString(std::to_string(ICE_BEAM_DAM) + " damage", ddutil::DAMAGE_COLOR);
			break;
		case 1:
			chosenMove = moves[1];
			targets.push_back(ddutil::getLowestHealthPlayer(players));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" will ", ddutil::TEXT_COLOR) + ColorString("Hex", HexedStatus::COLOR) +
				ColorString(" The ", ddutil::TEXT_COLOR) + targets[0]->getColorString() +
				ColorString(" for " + std::to_string(HEX_LENGTH) + " turns, dealing ", ddutil::TEXT_COLOR) +
				ColorString(std::to_string(HEX_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
			break;
		case 2:
			if (shieldCounter < 3)
			{
				chosenMove = moves[2];
				targets.push_back(this);
				intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
					ColorString(" is creating a barrier", ddutil::TEXT_COLOR);
				shieldCounter++;
			}
			else
			{
				chosenMove = moves[1];
				targets.push_back(ddutil::getLowestHealthPlayer(players));
				intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
					ColorString(" will ", ddutil::TEXT_COLOR) + ColorString("Hex", HexedStatus::COLOR) +
					ColorString(" The ", ddutil::TEXT_COLOR) + targets[0]->getColorString() +
					ColorString(" for " + std::to_string(HEX_LENGTH) + " turns", ddutil::TEXT_COLOR);
			}
			break;
		case 3:
			chosenMove = moves[3];
			targets.push_back(players[ddutil::random(0, players.size() - 1)]);
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to Strike The ", ddutil::TEXT_COLOR) +
				targets.front()->getColorString() +
				ColorString(" for ", ddutil::TEXT_COLOR) +
				ColorString(std::to_string(BLOCK_BREAK_DAMAGE) + " damage", ddutil::DAMAGE_COLOR) +
				ColorString(" (x" + std::to_string(BLOCK_BREAK_MULT) + " if hits block)", ddutil::DAMAGE_COLOR);
			break;
		case 4:
			chosenMove = moves[4];
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" will ", ddutil::TEXT_COLOR) +
				ColorString("heal " + std::to_string(SMALL_HEAL_AMOUNT) + " HP", ddutil::HEAL_COLOR);
			targets.push_back(this);
			break;
		}

		turnCounter++;
		if (turnCounter > 4)
		{
			turnCounter = 0;
		}
	}

	return EnemyTurn(intent, targets, chosenMove);
}

TheSalvager::TheSalvager(Game* game)
	:Chapter3Boss(
		game,
		HEALTH,
		"Salvager",
		Art::SALVAGER_COLOR,
		Art::getTheSalvager(),
		std::vector<ColorString> {ColorString("*The Salvager unleashes a monstrous roar*", Art::SALVAGER_COLOR)}
	)
{
	turnCounter = 0;
	snatchedPlayer = nullptr;
	moves.push_back(new EnemyMoves::Snatch(this));
	moves.push_back(new EnemyMoves::AddBleedToAttacks(BLEED_INC));
	moves.push_back(new EnemyMoves::Throw(this));
	moves.push_back(new SimpleStatusMove(
		MoveId::EnemyMoveId, new StrangledStatus(), STRANGLE_LENGTH, true, 0, "", Strength::Moderate, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC))
	);
	moves.push_back(new StatusAttackMove(
		MoveId::EnemyMoveId, VULNERABLE_STRIKE_DAMAGE, new VulnerableStatus(), VULNERABLE_STRIKE_LENGTH, 0, "", Strength::Moderate, WavFile("attack6", ddutil::SF_LOOP, ddutil::SF_ASYNC))
	);
	moves.push_back(new EnemyMoves::HealStrike(
		LIFESTEAL_STRIKE_DAMAGE, LIFESTEAL_STRIKE_DAMAGE, WavFile("attackheal", ddutil::SF_LOOP, ddutil::SF_ASYNC))
	);
	moves.push_back(new BlockBreakingMove(
		MoveId::EnemyMoveId, BLOCK_BREAK_DAMAGE, BLOCK_BREAK_MOD, 0, "", Strength::Moderate, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
	);
	moves.push_back(new EnemyMoves::TakeVitality(VIT_STEAL));
	moves.push_back(new EnemyMoves::Block(BIG_BLOCK, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

TheSalvager::~TheSalvager()
{
	if (snatchedPlayer != nullptr)
	{
		delete snatchedPlayer;
	}
}

EnemyTurn TheSalvager::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	switch (turnCounter)
	{
	case 0:
		if (players.size() > 1) // snatch a player
		{
			chosenMove = moves[0];
			targets.push_back(ddutil::getHighestHealthPlayer(players));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to snatch The ", ddutil::TEXT_COLOR) +
				targets.front()->getColorString() + ColorString(" to damage later!", ddutil::TEXT_COLOR);
		}
		else // strangle the one player
		{
			chosenMove = moves[3];
			targets.push_back(players.at(0));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to ", ddutil::TEXT_COLOR) + ColorString("Strangle", StrangledStatus::COLOR) +
				ColorString(" The ", ddutil::TEXT_COLOR) + targets.front()->getColorString();
		}
		break;
	case 1: // always add bleed to its attacks
		chosenMove = moves[1];
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is sharpening its talons...", ddutil::TEXT_COLOR);
		break;
	case 2:
		if (snatchedPlayer != nullptr) // throw the snatched player
		{
			chosenMove = moves[2];
			targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" will throw The ", ddutil::TEXT_COLOR) + snatchedPlayer->getColorString() +
				ColorString(" at The ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
				ColorString(", dealing " + std::to_string(THROW_DAMAGE) + " dmg", ddutil::DAMAGE_COLOR) +
				ColorString(" and ", ddutil::TEXT_COLOR) + ColorString("Stunning", StunnedStatus::COLOR) +
				ColorString(" each", ddutil::TEXT_COLOR);
		}
		else // vulnerable strike the remaining player
		{
			chosenMove = moves[4];
			targets.push_back(players.at(0));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" will Strike The ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
				ColorString(" for " + std::to_string(VULNERABLE_STRIKE_DAMAGE) + " damage", ddutil::DAMAGE_COLOR) +
				ColorString(" and " + std::to_string(VULNERABLE_STRIKE_LENGTH) + " Vulnerable", VulnerableStatus::COLOR);
		}
		break;
	case 3: // roll an attack
		if (ddutil::random(1, 2) == 1) // Lifesteal strike
		{
			chosenMove = moves[5];
			targets = players;
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" intends to", ddutil::TEXT_COLOR) +
				ColorString(" suck " + std::to_string(LIFESTEAL_STRIKE_DAMAGE) + " HP ", ddutil::DAMAGE_COLOR) +
				ColorString("from everybody", ddutil::TEXT_COLOR);
		}
		else
		{
			chosenMove = moves[6];
			targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to Strike The ", ddutil::TEXT_COLOR) +
				targets.front()->getColorString() +
				ColorString(" for ", ddutil::TEXT_COLOR) +
				ColorString(std::to_string(BLOCK_BREAK_DAMAGE) + " damage", ddutil::DAMAGE_COLOR) +
				ColorString(" (x"+std::to_string(BLOCK_BREAK_MOD)+" if hits block)", ddutil::DAMAGE_COLOR);
		}
		break;
	case 4: // roll vitality steal versus big block
		if (ddutil::random(1, 2) == 1)
		{
			chosenMove = moves[7]; // steal vitality
			targets = players;
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" is reducing everybody's ", ddutil::TEXT_COLOR) +
				ColorString("Vitality", ddutil::VITALITY_COLOR) +
				ColorString(" by " + std::to_string(VIT_STEAL), ddutil::TEXT_COLOR);
		}
		else
		{
			chosenMove = moves[8]; // beeg block
			targets.push_back(this);
			intent = ddutil::genericBlockIntent(BIG_BLOCK, getColorString());
		}
		break;
	}
	turnCounter++;
	if (turnCounter > 4)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

ColorString TheSalvager::getStatLine()
{
	if (snatchedPlayer == nullptr)
	{
		return Enemy::getStatLine();
	}
	else
	{
		return Enemy::getStatLine() + ColorString("(", ddutil::TEXT_COLOR) + snatchedPlayer->getColorString() +
			ColorString(")", ddutil::TEXT_COLOR);
	}
}

Creature* TheSalvager::makeCopy()
{
	return new TheSalvager(game);
}

int TheSalvager::getRoomId()
{
	return RoomId::TheSalvagerEnemy;
}

void TheSalvager::deathScene()
{
	if (snatchedPlayer != nullptr)
	{
		game->addPlayer(snatchedPlayer);
		snatchedPlayer = nullptr;
	}
	Chapter3Boss::deathScene();
}

ColorString TheSalvager::snatch(Creature* creature)
{
	Player* player = dynamic_cast<Player*>(creature);
	if (player == nullptr)
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" cannot snatch The ", ddutil::TEXT_COLOR) +
			creature->getColorString();
	}

	game->removePlayer(player);
	snatchedPlayer = player;
	return ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" snatches The ", ddutil::TEXT_COLOR) +
		player->getColorString();
}

ColorString TheSalvager::throwPlayer(Creature* targetToHit)
{
	ColorString targetName("Ground", ddutil::TEXT_COLOR); // in the case where the target has died since it was selected
	if (targetToHit != nullptr) // if everything is normal, do things like you would expect
	{
		targetToHit->reduceHealth(THROW_DAMAGE, this, false);
		targetToHit->applyStatus(new StunnedStatus(), THROW_STUN);
		targetName = targetToHit->getColorString();// override the nothing name because we actually were able to throw at somebody
	} // else, we just don't reduce health of another person, essentially the snatched player gets thrown at nothing

	// the snatched player will always be non nullptr
	snatchedPlayer->reduceHealth(THROW_DAMAGE, this, false);
	snatchedPlayer->applyStatus(new StunnedStatus(), THROW_STUN);
	if (snatchedPlayer->getHealth() <= 0)
	{
		snatchedPlayer->setHealth(1); // so that the thrown player can't die from being thrown, to be more fair
	}

	ColorString output = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
		ColorString(" throws The ", ddutil::TEXT_COLOR) + snatchedPlayer->getColorString() +
		ColorString(" at The ", ddutil::TEXT_COLOR) + targetName + ColorString(", dealing " + std::to_string(THROW_DAMAGE) + " damage", ddutil::DAMAGE_COLOR) +
		ColorString(" and applying ", ddutil::TEXT_COLOR) + ColorString("Stunned", StunnedStatus::COLOR);

	game->addPlayer(snatchedPlayer);
	snatchedPlayer = nullptr;

	return output;
}

Creature* TheProtector::makeCopy()
{
	return new TheProtector(game);
}

int TheProtector::getRoomId()
{
	return RoomId::TheProtectorEnemy;
}

TruePatriarch::TruePatriarch(Game* game)
	:Enemy(
		game, HEALTH, "Patriarch", ddutil::PATRIARCH_STRING[0].color, Art::getPatriarch(), Mp3File("finalbossbattle1"),
		std::vector<ColorString> {
			ColorString("\"I've been plotting this world's destruction for the last 10,000 years....\"", ddutil::PATRIARCH_STRING[0].color),
			ColorString("\"And no mere mortals are going to stop me!\"", ddutil::PATRIARCH_STRING[0].color)
		},
		ddutil::MAP_BOSS
	)
{
	healthbarLength = ddutil::BOSS_HEALTHBAR_LENGTH;
	turnCounter = 0;
	damageCap = TURN_DMG_CAP;
	desperation = false;
	moves.push_back(new EnemyMoves::TakeVitalityGain(VIT_CHANGE));
	moves.push_back(new EnemyMoves::Strike(BEAM_DAMAGE, WavFile("magicattack3", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Block(BIG_BLOCK, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::BlockBreakStrike(BLOCK_BREAK_DAMAGE, BLOCK_BREAK_MULT, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Strike(DESPERATION_BEAM_DAM, WavFile("desperationbeam", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	// distortion 5 moves
	moves.push_back(new EnemyMoves::StealMove());
	moves.push_back(new EnemyMoves::Strike(DIST5_BEAM_DAM, WavFile("magicattack3", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, HEX_DAMAGE, new HexedStatus(), HEX_LENGTH, 0, "", Strength::Mythical,
		WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Block(DIST5BLOCK, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new SimpleAttackMove(MoveId::EnemyMoveId, DIST5THRUBLOCK, true, 0, "", Strength::Mythical, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn TruePatriarch::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	// reset the damage cap
	damageCap = TURN_DMG_CAP;

	if (getHealth() <= getMaxHealth(DESPERATION_THRESHOLD) && !desperation)
	{
		desperation = true;
		applyStatus(new InvulnerableStatus(), INVULN_LENGTH);
		stopSound(SoundType::MP3);
		playSound(Mp3File("finalbossbattle2"));
	}

	if (!desperation)
	{
		if (game->getCurrentDistortion() == 5)
		{
			switch (turnCounter)
			{
			case 0:
				chosenMove = moves[5];
				targets = players;
				intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
					ColorString(" is opening its mouth?", ddutil::TEXT_COLOR);
				break;
			case 1:
			{
				auto scalingAttack = dynamic_cast<EnemyMoves::Strike*>(moves[6]);
				scalingAttack->increaseStrength(DIST5_BEAM_SCALE);
				chosenMove = moves[6];
				targets = players;
				intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
					ColorString(" will Blast everyone for ", ddutil::TEXT_COLOR) +
					ColorString(std::to_string(scalingAttack->getStrength()) + " damage!", ddutil::DAMAGE_COLOR);

				break;
			}
			case 2:
				if (ddutil::random(1, 2) == 1)
				{
					chosenMove = moves[3];
					targets.push_back(ddutil::getLowestHealthPlayer(players));
					intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
						ColorString(" intends to Strike its horns into The ", ddutil::TEXT_COLOR) +
						targets.front()->getColorString() +
						ColorString(" for ", ddutil::TEXT_COLOR) +
						ColorString(std::to_string(BLOCK_BREAK_DAMAGE) + " damage", ddutil::DAMAGE_COLOR) +
						ColorString(" (x"+std::to_string(BLOCK_BREAK_MULT)+" if hits block)", ddutil::DAMAGE_COLOR);
				}
				else
				{
					chosenMove = moves[9];
					targets.push_back(ddutil::getLowestHealthPlayer(players));
					intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
						ColorString(" intends to ", ddutil::TEXT_COLOR) + ColorString("Strike", ddutil::TEXT_COLOR) +
						ColorString(" the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
						ColorString(" through block for ", ddutil::TEXT_COLOR) +
						ColorString(std::to_string(DIST5THRUBLOCK) + " damage ", ddutil::DAMAGE_COLOR);
				}
				break;
			case 3:
				chosenMove = moves[8];
				targets.push_back(this);
				intent = ddutil::genericBlockIntent(DIST5BLOCK, getColorString());
				break;
			}
			turnCounter++;
			if (turnCounter > 3)
			{
				turnCounter = 0;
			}
		}
		else
		{
			switch (turnCounter)
			{
			case 0:
				chosenMove = moves[0];
				targets = players;
				intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
					ColorString(" is stealing everybody's", ddutil::TEXT_COLOR) +
					ColorString(" Vitality Gain", ddutil::VITALITY_COLOR);
				break;
			case 1:
				chosenMove = moves[1];
				targets = players;
				intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
					ColorString(" will Blast everybody for ", ddutil::TEXT_COLOR) +
					ColorString(std::to_string(BEAM_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
				break;
			case 2:
				chosenMove = moves[2];
				targets.push_back(this);
				intent = ddutil::genericBlockIntent(BIG_BLOCK, getColorString());
				break;
			case 3:
				chosenMove = moves[3];
				targets.push_back(ddutil::getLowestHealthPlayer(players));
				intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
					ColorString(" intends to Strike its horns into The ", ddutil::TEXT_COLOR) +
					targets.front()->getColorString() +
					ColorString(" for ", ddutil::TEXT_COLOR) +
					ColorString(std::to_string(BLOCK_BREAK_DAMAGE) + " damage", ddutil::DAMAGE_COLOR) +
					ColorString(" (x"+std::to_string(BLOCK_BREAK_MULT)+" if hits block)", ddutil::DAMAGE_COLOR);
				break;
			}

			turnCounter++;
			if (turnCounter > 3)
			{
				turnCounter = 0;
			}
		}



	}
	else // desperation
	{
		auto scalingAttack = dynamic_cast<EnemyMoves::Strike*>(moves[4]);
		if (game->getCurrentDistortion() == 5)
		{
			scalingAttack->increaseStrength(5);
		}
		chosenMove = moves[4];
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is wildy aiming its Energy Beam at an unidentifiable target for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(scalingAttack->getStrength()) + " damage", ddutil::DAMAGE_COLOR) +
			ColorString("!", ddutil::TEXT_COLOR);
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* TruePatriarch::makeCopy()
{
	return new TruePatriarch(game);
}

int TruePatriarch::getRoomId()
{
	return RoomId::TruePatriarchEnemy;
}

void TruePatriarch::deathScene()
{
	game->changeScore(ddutil::VICTORY_SCORE);

	VirtualWindow* vwin = game->getVWin();

	vwin->clearScreen();

	Coordinate coord = Coordinate(0, ddutil::CENTER_TEXT_LINE - 2);

	vwin->printArtFromBottom(picture, coord, true);
	playSound(WavFile("bossdeath2", false, true));
	for (int i = 0; i < 15; i++)
	{
		vwin->printArtFromBottom(picture, coord, true);
		Sleep(100);
		game->clearCenterScreen();
		Sleep(100);
	}
	game->clearCenterScreen();
	playSound(WavFile("explosion", false, false));
	Sleep(3000);

	game->endingScene();

	game->setGameWin();

}

void TruePatriarch::distortionUpdate(int dist)
{
	if (dist == 5)
	{	
		picture = Art::getDist5Patriarch();
		increaseMaxHealth(200);
		introText = std::vector<ColorString>{
			ColorString("Do you know what I'll do after I defeat you?", color),
			ColorString("......", color),
			ColorString("Why don't you stay and find out?", color)
		};
	}
}

void TruePatriarch::doMiscDamageEffects(int damage)
{
	damageCap -= damage;
	if (damageCap < 0)
	{
		health -= damageCap; // increase health by how much the damage cap is below zero
		damageCap = 0;
	}
}

ColorString TruePatriarch::getStatLine()
{
	return Enemy::getStatLine() + ColorString("CAP: " + std::to_string(damageCap), ddutil::DAMAGE_COLOR);
}

VampireBat::VampireBat(Game* game)
	:NormalEnemy(game, HEALTH, "Vampire Bat", Art::VAMP_BAT_COLOR, Art::getVampireBat())
{
	turnCounter = 0;
	moves.push_back(new EnemyMoves::Strike(STRIKE_DAMAGE, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::HealStrike(LIFESTEAL_AMOUNT, LIFESTEAL_AMOUNT,
		WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn VampireBat::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	chosenMove = moves[turnCounter];
	switch (turnCounter)
	{
	case 0: // non lifesteal strike
		targets.push_back(players[ddutil::random(0, players.size()-1)]);
		intent = ddutil::genericDamageIntent(STRIKE_DAMAGE, getColorString(), "Strike", targets);
		break;
	case 1: // lifesteal strike
		targets.push_back(ddutil::getHighestHealthPlayer(players));
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to suck " + std::to_string(LIFESTEAL_AMOUNT) + " health points out of the ", ddutil::TEXT_COLOR) +
			targets.front()->getColorString();
		break;
	}
	turnCounter++;
	if (turnCounter > 1)
	{
		turnCounter = 0;
	}
	return EnemyTurn(intent, targets, chosenMove);
}

Creature* VampireBat::makeCopy()
{
	return new VampireBat(game);
}

int VampireBat::getRoomId()
{
	return RoomId::VampireBatEnemy;
}

VampireBatVariant::VampireBatVariant(Game* game)
	:NormalEnemy(game, HEALTH, "Vampire Bat", Art::VAMP_BAT_ALT_COLOR, Art::getVampireBatAlt())
{
	turnCounter = 0;
	moves.push_back(new EnemyMoves::Strike(SWIPE_DAMAGE, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, SPIT_DAMAGE, new FrostBurntStatus(), FBURN_LENGTH, 0, "Spit", Strength::Moderate,
		WavFile("attack3", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::HealStrike(LIFESTEAL_BITE_DAMAGE, LIFESTEAL_AMOUNT, WavFile("attack5", ddutil::SF_LOOP,
		ddutil::SF_ASYNC)));
}

EnemyTurn VampireBatVariant::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	chosenMove = moves[turnCounter];
	switch (turnCounter)
	{
	case 0: // Swipe attack	
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to swipe everybody for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(SWIPE_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 1:
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to spit on the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
			ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(SPIT_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 2:
		targets.push_back(ddutil::getLowestHealthPlayer(players));
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to suck " + std::to_string(LIFESTEAL_AMOUNT) + " health points out of the ", ddutil::TEXT_COLOR) +
			targets.front()->getColorString();
		break;
	}
	turnCounter += ddutil::random(1, 2);
	if (turnCounter > 2)
	{
		turnCounter = 0;
	}
	return EnemyTurn(intent, targets, chosenMove);
}

Creature* VampireBatVariant::makeCopy()
{
	return new VampireBatVariant(game);
}

int VampireBatVariant::getRoomId()
{
	return RoomId::VampireBatVariantEnemy;
}

CaveBat::CaveBat(Game* game)
	:NormalEnemy(game, HEALTH, "Cave Bat", Art::CAVEBAT_COLOR, Art::getCaveBat())
{
	moves.push_back(new EnemyMoves::Strike(WING_ATTACK_DAMAGE, WavFile("attack3", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::SuicideStrike(DIVEBOMB_DAMAGE, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn CaveBat::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	if (getHealth() > DIVEBOMB_THRESHOLD)
	{
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		chosenMove = moves[0];
		intent = ddutil::genericDamageIntent(WING_ATTACK_DAMAGE, getColorString(), "Wing Strike", targets);
	}
	else
	{
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		chosenMove = moves[1];
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to divebomb the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
			ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(DIVEBOMB_DAMAGE) + " damage", ddutil::DAMAGE_COLOR) +
			ColorString(", killing itself", ddutil::TEXT_COLOR);
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* CaveBat::makeCopy()
{
	return new CaveBat(game);
}

int CaveBat::getRoomId()
{
	return RoomId::CaveBatEnemy;
}

CaveBatVariant::CaveBatVariant(Game* game)
	:NormalEnemy(game, HEALTH, "Cave Bat", Art::CAVEBATALT_COLOR, Art::getCaveBatAlt())
{
	moves.push_back(new SimpleStatusMove(MoveId::EnemyMoveId, new PoisonedStatus(), POISON_SPRAY_AMOUNT, true, 0, "Poison Spray",
		Strength::Powerful, WavFile("electricattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, POISON_BITE_DAMAGE, new PoisonedStatus(), POISON_BITE_AMOUNT, 0, "Poison Bite",
		Strength::Moderate, WavFile("attack3", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn CaveBatVariant::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	if (ddutil::random(1,2) == 1)
	{
		targets = players;
		chosenMove = moves[0];
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to spray ", ddutil::TEXT_COLOR) +
			ColorString("poison (" + std::to_string(POISON_SPRAY_AMOUNT) + ")", PoisonedStatus::COLOR) +
			ColorString(" on everybody", ddutil::TEXT_COLOR);
	}
	else
	{
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		chosenMove = moves[1];
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to bite the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
			ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(POISON_BITE_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) +
			ColorString(" and ", ddutil::TEXT_COLOR) + ColorString(std::to_string(POISON_BITE_AMOUNT) + " poison", PoisonedStatus::COLOR);
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* CaveBatVariant::makeCopy()
{
	return new CaveBatVariant(game);
}

int CaveBatVariant::getRoomId()
{
	return RoomId::CaveBatVariantEnemy;
}

AncientLizard::AncientLizard(Game* game)
	: HardEnemy(game, HEALTH, "Ancient Lizard", Art::ANC_LIZARD_COLOR, Art::getAncientLizard())
{
	turnCounter = 0;
	moves.push_back(new EnemyMoves::Strike(STOMP_DAMAGE, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, CHARGE_DAMAGE, new StunnedStatus(), STUNNED_LENGTH, 0, "Charge",
		Strength::Powerful, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Heal(HEAL_AMOUNT));
}

EnemyTurn AncientLizard::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	chosenMove = moves[turnCounter];
	switch (turnCounter)
	{
	case 0: // Stomp
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ddutil::genericDamageIntent(STOMP_DAMAGE, getColorString(), "Stomp on", targets);
		break;
	case 1: // Charge --> stun
		targets.push_back(ddutil::getLowestHealthPlayer(players));
		intent = ddutil::genericDamageIntent(CHARGE_DAMAGE, getColorString(), "Charge into and stun", targets);
		break;
	case 2: // self heal
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is shedding its skin", ddutil::HEAL_COLOR);
		break;
	}
	turnCounter++;
	if (turnCounter > 2)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* AncientLizard::makeCopy()
{
	return new AncientLizard(game);
}

int AncientLizard::getRoomId()
{
	return RoomId::AncientLizardEnemy;
}

FireBat::FireBat(Game* game)
	:NormalEnemy(game, HEALTH, "Fire Bat", Art::FIRE_BAT_COLOR, Art::getFireBat())
{
	turnCounter = 0;
	if (ddutil::random(1, 2))
	{
		turnCounter = 2;
	}
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, WHIRLWIND_DAMAGE, new BurntStatus(), WHIRLWIND_BURN_LENGTH,
		0, "Fire Whirlwind", Strength::Powerful, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new SimpleStatusMove(MoveId::EnemyMoveId, new ScorchedStatus(), SCORCH_LENGTH, false, 0, "Scorched",
		Strength::Powerful, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new AttackAndBlockMove(MoveId::EnemyMoveId, STRIKE_DAMAGE, STRIKE_BLOCK, false, 0, "Block-Strike",
		Strength::Powerful, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new MultiAttackMove(MoveId::EnemyMoveId, BITE_DAMAGE, BITE_TIMES, 0, "Dual Bite", Strength::Powerful,
		WavFile("dualattack", ddutil::SF_LOOP, ddutil::SF_ASYNC)));

}

EnemyTurn FireBat::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	switch (turnCounter)
	{
	case 0: // multiattack burn thing
		chosenMove = moves[0];
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to engulf everyone in a ", ddutil::TEXT_COLOR) +
			ColorString("Fire Maelstrom", BurntStatus::COLOR) + ColorString(" for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(WHIRLWIND_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 1: // apply scorched
		chosenMove = moves[1];
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" will ", ddutil::TEXT_COLOR) + ColorString("Scorch", ScorchedStatus::COLOR) +
			ColorString(" the ", ddutil::TEXT_COLOR) + targets.front()->getColorString();
		break;
	case 2: // either dual attack or block strike
		if (ddutil::random(1, 2) == 1)
		{
			chosenMove = moves[2];
			targets.push_back(ddutil::getHighestHealthPlayer(players));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to Strike the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
				ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(STRIKE_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) +
				ColorString(" and block ", ddutil::TEXT_COLOR) + ColorString(std::to_string(STRIKE_BLOCK) + " damage", ddutil::TEXT_COLOR);
		}
		else
		{
			chosenMove = moves[3];
			targets.push_back(ddutil::getLowestHealthPlayer(players));
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
				ColorString(" intends to Bite the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
				ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(BITE_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) + 
				ColorString(std::to_string(BITE_TIMES) + " times", ddutil::TEXT_COLOR);
		}
		break;
	}

	turnCounter++;
	if (turnCounter > 2)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* FireBat::makeCopy()
{
	return new FireBat(game);
}

int FireBat::getRoomId()
{
	return RoomId::FireBatEnemy;
}

FireBatVariant::FireBatVariant(Game* game)
	:NormalEnemy(game, HEALTH, "Fire Bat", Art::FIRE_BAT_VARIANT_COLOR, Art::getFireBatVariant())
{
	turnCounter = 0;
	previousPerson = nullptr;
	moves.push_back(new EnemyMoves::CreateShield(BASE_BLOCK, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, BURN_DAMAGE, new BurntStatus(), BURN_LENGTH, 0, "Flamethrower",
		Strength::Powerful, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new SimpleStatusMove(MoveId::EnemyMoveId, new ScorchedStatus(), SCORCHED_LENGTH, false, 0, "Scorch", Strength::Powerful,
		WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn FireBatVariant::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	switch (turnCounter)
	{
	case 0: // flame shield move
		chosenMove = moves[0];
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is enveloping itself in ", ddutil::TEXT_COLOR) +
			ColorString(" Flames", Art::FIRE_BAT_VARIANT_COLOR);
		break;
	case 1: // burn dood
		chosenMove = moves[1];
		previousPerson = ddutil::getHighestHealthPlayer(players);
		targets.push_back(previousPerson);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to ", ddutil::TEXT_COLOR) + ColorString("Burn", Art::FIRE_BAT_VARIANT_COLOR) +
			ColorString(" the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
			ColorString(" for ", ddutil::TEXT_COLOR) + 
			ColorString(std::to_string(BURN_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 2: // scorch dood
		chosenMove = moves[2];
		if (previousPerson == nullptr)
		{
			previousPerson = ddutil::getHighestHealthPlayer(players);
		}
		targets.push_back(previousPerson);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" will ", ddutil::TEXT_COLOR) + ColorString("Scorch", ScorchedStatus::COLOR) +
			ColorString(" the ", ddutil::TEXT_COLOR) + targets.front()->getColorString();
		break;
	}

	turnCounter++;
	if (turnCounter > 2)
	{
		turnCounter = 1;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* FireBatVariant::makeCopy()
{
return new FireBatVariant(game);
}

int FireBatVariant::getRoomId()
{
	return RoomId::FireBatVariantEnemy;
}

FirePlatypus::FirePlatypus(Game* game)
	:NormalEnemy(game, HEALTH, "Magma Platypus", Art::FIRE_PLATYPUS_COLOR, Art::getFirePlatypus())
{
	turnCounter = 0;
	moves.push_back(new AttackAndBlockMove(MoveId::EnemyMoveId, STRIKE_DAMAGE, STRIKE_BLOCK, false, 0,
		"Strike-Block", Strength::Powerful, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, FLAMETHROWER_DAMAGE, new BurntStatus(), FLAMETHROWER_BURN_LENGTH, 0,
		"Flamethrower", Strength::Powerful, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, BODY_SLAM_DAMAGE, new StunnedStatus(), STUN_LENGTH, 0,
		"Body Slam", Strength::Powerful, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Heal(HEAL_AMOUNT));
}

EnemyTurn FirePlatypus::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	switch (turnCounter)
	{
	case 0:
		chosenMove = moves[0];
		targets.push_back(ddutil::getLowestHealthPlayer(players));
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to Strike the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
			ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(STRIKE_DAMAGE) + " damage and ", ddutil::DAMAGE_COLOR) +
			ColorString(" block ", ddutil::BLOCK_COLOR) + ColorString(std::to_string(STRIKE_BLOCK) + " damage", ddutil::BLOCK_COLOR);
		break;
	case 1:
		chosenMove = moves[1];
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to shoot a ", ddutil::TEXT_COLOR) +
			ColorString("Flamethrower", BurntStatus::COLOR) + ColorString(" at everybody for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(FLAMETHROWER_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 2:
		chosenMove = moves[2];
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to Body Slam into everybody for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(BODY_SLAM_DAMAGE) + " damage", ddutil::TEXT_COLOR);
		break;
	case 3:
		chosenMove = moves[3];
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to ", ddutil::TEXT_COLOR) + ColorString("Heal", ddutil::HEAL_COLOR) +
			ColorString(" for " + std::to_string(HEAL_AMOUNT) + " health", ddutil::TEXT_COLOR);
		break;
	}
	turnCounter++;
	if (turnCounter > 3)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* FirePlatypus::makeCopy()
{
	return new FirePlatypus(game);
}

int FirePlatypus::getRoomId()
{
	return RoomId::FirePlatypusEnemy;
}

Sniffer::Sniffer(Game* game)
	:IntenseEnemy(game, HEALTH, "Sniffer", Art::SNIFFER_COLOR, Art::getSniffer(),
		std::vector<ColorString> {
			ColorString("The Sniffer howls loudly into the void", Art::SNIFFER_COLOR)
		}
	)
{
	turnCounter = 0;
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, RAVAGE_DAMAGE, new VulnerableStatus(), RAVAGE_VULNERABLE_LENGTH,
		0, "Ravage", Strength::Mythical, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, STRANGLE_DAMAGE, new StrangledStatus(), STRANGLE_LENGTH,
		0, "Strangle", Strength::Mythical, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::HealStrike(LIFE_STEAL_AMOUNT, LIFE_STEAL_AMOUNT,
		WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn Sniffer::getTurn(std::vector<Creature*> players)
{

	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;

	chosenMove = moves[turnCounter];
	switch (turnCounter)
	{
	case 0:
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ddutil::genericDamageIntent(RAVAGE_DAMAGE, getColorString(), "Ravage", targets);
		break;
	case 1:
		for (Creature* p : players)
		{
			if (p->hasStatus(StatusID::Vulnerable))
			{
				targets.push_back(p);
				break;
			}
		}
		if (targets.empty())// nobody is currently vulnerable so just attack a random person
		{
			targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		}
		intent = ddutil::genericDamageIntent(STRANGLE_DAMAGE, getColorString(), "Strangle", targets);
		break;
	case 2:
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to suck ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(LIFE_STEAL_AMOUNT), ddutil::DAMAGE_COLOR) +
			ColorString(" health ", ddutil::HEAL_COLOR) + ColorString("from everybody", ddutil::TEXT_COLOR);
		break;
	}
	// Keep generating random turn until it generates not the same turn two turns in a row
	int previous = turnCounter;
	do
	{
		turnCounter = ddutil::random(0, 2);
	} while (turnCounter == previous);

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* Sniffer::makeCopy()
{
	return new Sniffer(game);
}

int Sniffer::getRoomId()
{
	return RoomId::SnifferEnemy;
}

SnifferVariant::SnifferVariant(Game* game)
	:IntenseEnemy(game, HEALTH, "Sniffer", Art::SNIFFER_VARIANT_COLOR, Art::getSnifferVariant(),
		std::vector<ColorString> {
			ColorString("The Sniffer howls loudly into the void", Art::SNIFFER_VARIANT_COLOR)
		}
	)
{
	previousMove = -1;
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, FROST_BREATH_DAMAGE, new FrostBurntStatus(), FROST_BURN_LENGTH, 0,
		"Frost Breath", Strength::Powerful, WavFile("freeze", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new SelfBlockMove(MoveId::EnemyMoveId, BIG_BLOCK_AMOUNT, 0, "beeg block", Strength::Powerful, WavFile("gainblock",
		ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new AttackAndBlockMove(MoveId::EnemyMoveId, BLOCK_ATTACK_DAMAGE, BLOCK_ATTACK_BLOCK, false, 0, "attack-block",
		Strength::Powerful, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn SnifferVariant::getTurn(std::vector<Creature*> players)
{
	ColorString intent;
	std::vector<Creature*> targets;
	Move* chosenMove = nullptr;

	int moveNum = previousMove;
	do
	{
		// keep trying to pick a move until it doesn't pick the same move two times in a row
		moveNum = ddutil::random(0, 2);
	} while (moveNum == previousMove);
	previousMove = moveNum;

	chosenMove = moves[moveNum];
	switch (moveNum)
	{
	case 0:
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to ", ddutil::TEXT_COLOR) + ColorString("Freeze", FrostBurntStatus::COLOR) +
			ColorString(" everybody for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(FROST_BREATH_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 1:
		targets.push_back(this);
		intent = ddutil::genericBlockIntent(BIG_BLOCK_AMOUNT, getColorString());
		break;
	case 2:
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to attack the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
			ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(BLOCK_ATTACK_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) +
			ColorString("and", ddutil::TEXT_COLOR) + ColorString(" block " + std::to_string(BLOCK_ATTACK_BLOCK) + " damage", ddutil::BLOCK_COLOR);
		break;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* SnifferVariant::makeCopy()
{
	return new SnifferVariant(game);
}

int SnifferVariant::getRoomId()
{
	return RoomId::SnifferVariantEnemy;
}

CorruptedDisciple::CorruptedDisciple(Game* game)
	:IntenseHardEnemy(game, HEALTH, "Corrupted Disciple", Art::CORRUPTED_DISCIPLE_COLOR, Art::getCorruptedDisciple(),
		std::vector<ColorString> {
			ColorString("The Disciple mumbles a strange hymn...", Art::CORRUPTED_DISCIPLE_COLOR)
		}
	)
{
	turnCounter = 0;
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, HEX_DAMAGE, new HexedStatus(), HEXED_LENGTH,
		0, "Hex", Strength::Powerful, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MoveId::EnemyMoveId, LIGHTNING_STRIKE_DAMAGE, new ZappedStatus(), ZAPPED_AMOUNT,
		0, "Smite", Strength::Mythical, WavFile("lightning", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Heal(HEAL_AMOUNT));
}

EnemyTurn CorruptedDisciple::getTurn(std::vector<Creature*> players)
{
	ColorString intent;
	std::vector<Creature*> targets;
	Move* chosenMove = moves[turnCounter];

	switch (turnCounter)
	{
	case 0: // hex 
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to ", ddutil::TEXT_COLOR) + ColorString("Hex", HexedStatus::COLOR) +
			ColorString(" the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
			ColorString(" for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(HEX_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR);
		break;
	case 1: // lightning strike
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" intends to ", ddutil::TEXT_COLOR) + ColorString("Smite", ZappedStatus::COLOR) +
			ColorString(" the ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
			ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(LIGHTNING_STRIKE_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 2: // heal
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is reciting a strange prayer", ddutil::TEXT_COLOR);
		break;
	}
	turnCounter++;
	if (turnCounter > 2)
	{
		turnCounter = 0;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* CorruptedDisciple::makeCopy()
{
	return new CorruptedDisciple(game);
}

int CorruptedDisciple::getRoomId()
{
	return RoomId::CorruptedDiscipleEnemy;
}

DemonicDisciple::DemonicDisciple(Game* game)
	:Enemy(
		game,
		HEALTH,
		"Demonic Disciple",
		Art::DEMONIC_DISCIPLE_COLOR,
		Art::getDemonicDisciple(),
		Mp3File("penultimatebattle"),
		std::vector<ColorString> {
			ColorString("...", Art::DEMONIC_DISCIPLE_COLOR),
		},
		ddutil::MAP_NENEMY
	)
{
	currentStatuses.emplace(new InvulnerableStatus(), INVULN_LENGTH);
	turnCounter = 0;
	unHexedPlayer = nullptr;
	moves.push_back(new SimpleStatusMove(
		MoveId::EnemyMoveId, new HexedStatus(), HEXED_LENGTH, true, 0, "", Strength::Cosmic, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC))
	);
	moves.push_back(new EnemyMoves::Strike(BLAST_DAMAGE, WavFile("explosion", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Strike(MULTI_STRIKE_DAMAGE, WavFile("attack6", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::TakeVitality(VIT_STEAL));
	moves.push_back(new EnemyMoves::Strike(END_BLAST_DAMAGE, WavFile("magicattack3", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn DemonicDisciple::getTurn(std::vector<Creature*> players)
{
	std::vector<Creature*> targets;
	ColorString intent;
	Move* chosenMove = nullptr;

	chosenMove = moves[turnCounter];
	switch (turnCounter)
	{
	case 0:
	{
		// iterate through the players in a random order
		int timesThrough = 0;
		for (int i : ddutil::uniqueRandom(0, players.size() - 1, players.size()))
		{
			if (timesThrough == 0)
			{
				unHexedPlayer = players.at(i);
			}
			else
			{
				targets.push_back(players.at(i));
			}
			timesThrough++;
		}
		if (!targets.empty()) // normal case
		{
			// targets contain player to hex, unhexed player will be stored for next turn
			intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" will ", ddutil::TEXT_COLOR) +
				ColorString("Hex", HexedStatus::COLOR);
			for (unsigned i = 0; i < targets.size(); i++)
			{
				Creature* p = targets.at(i);
				intent += ColorString(" The ", ddutil::TEXT_COLOR) + p->getColorString();
				if (i != targets.size() - 1) // not last player
				{
					intent += ColorString(" and", ddutil::TEXT_COLOR);
				}
			}
			break;
		}
		// else = edge case
		chosenMove = moves[1]; // update chosen move to fit with the switch case down below
		// if the targets were empty, then fall through to the next switch case so that the solo player
		// gets attacked instead of hexed because it wouldn't be fair to hex the just one person
	}
		
	case 1:
		if (unHexedPlayer->getHealth() <= 0) // dead because the player killed them??
		{
			unHexedPlayer = players.front(); // just set the target to the front player in the alive player list
			// now they technically aren't an unhexed player, but whatever
		}
		targets.push_back(unHexedPlayer);
		intent = ddutil::genericDamageIntent(BLAST_DAMAGE, getColorString(), "Blast", targets);
		break;
	case 2:
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" will Strike everybody for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(MULTI_STRIKE_DAMAGE) + " damage", ddutil::DAMAGE_COLOR);
		break;
	case 3:
		targets = players;
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is reducing everybody's ", ddutil::TEXT_COLOR) +
			ColorString("Vitality", ddutil::VITALITY_COLOR) +
			ColorString(" by " + std::to_string(VIT_STEAL), ddutil::TEXT_COLOR);
		break;
	case 4:
		targets.push_back(players.at(ddutil::random(0, players.size() - 1)));
		intent = ddutil::genericDamageIntent(END_BLAST_DAMAGE, getColorString(), "Blast", targets);
		break;
	}
	turnCounter++;
	if (turnCounter > 4)
	{
		turnCounter = 4;
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* DemonicDisciple::makeCopy()
{
	return new DemonicDisciple(game);
}

int DemonicDisciple::getRoomId()
{
	return RoomId::DemonicDiscipleEnemy;
}
