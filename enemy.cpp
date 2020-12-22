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
	game->changeScore(ddutil::NORM_ENEMY_SCORE);
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

	targets.push_back(ddutil::getLowestHealthPlayer(players));
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




HardEnemy::HardEnemy(Game* game, int maxHp, std::string name, int color, Picture pic)
	: Enemy(game, maxHp, name, color, pic, Mp3File("hardbattle"),
		std::vector<ColorString>{ColorString("A ", ddutil::TEXT_COLOR) + ColorString(name, color) + ColorString(" attacks!", ddutil::TEXT_COLOR)},
		ddutil::MAP_SENEMY)
{
}

void HardEnemy::deathScene()
{
	Enemy::deathScene();
	game->changeScore(ddutil::STRONG_ENEMY_SCORE);
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

	targets.push_back(ddutil::getHighestHealthPlayer(players));
	chosenMove = moves[0];
	intent = ddutil::genericDamageIntent(scalingAttack->getStrength(), getColorString(), "Strike", targets);

	

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* Brute::makeCopy()
{
	return new Brute(game);
}



BossEnemy::BossEnemy(Game* game, int maxHp, std::string name, int color, Picture pic, std::vector<ColorString> intro)
	: Enemy(game, maxHp, name, color, pic, Mp3File("bossbattle"), intro, ddutil::MAP_BOSS)
{
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

	game->clearCenterScreen();
}

TheMessenger::TheMessenger(Game* game)
	:BossEnemy(game, HEALTH, "Messenger", ddutil::YELLOW, Art::getBurningSkull(),
		std::vector<ColorString>{
			ColorString("\"You are no match for me, let alone ", ddutil::YELLOW) + ddutil::PATRIARCH_STRING + ColorString("!\"", ddutil::YELLOW)
		}
	)
{
	moves.push_back(new EnemyMoves::Burn(BURN_LENGTH));
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
			ColorString(" will ",ddutil::TEXT_COLOR) + ColorString("Burn", BurntStatus::COLOR) +
			ColorString(" everybody", ddutil::TEXT_COLOR);
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
	case 3: // Strike (again)
		targets.push_back(ddutil::getLowestHealthPlayer(players));
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
		targets.push_back(ddutil::getLowestHealthPlayer(players));
		intent = ddutil::genericDamageIntent(SINGLE_TARGET_DAMAGE, getColorString(), "Strike", targets);
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* MysteriousKnight::makeCopy()
{
	return new MysteriousKnight(game);
}

Spider::Spider(Game* game)
	:IntenseEnemy(game, HEALTH, "Giant Spider", Art::SPIDER_COLOR, Art::getSpider(), 
		std::vector<ColorString> {ColorString("The Spider's roar sends shivers down your spine", Art::SPIDER_COLOR)})
{
	turnCounter = 0;
	moves.push_back(new EnemyMoves::CreateShield(BASE_BLOCK, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new SimpleStatusMove(new StrangledStatus(), STRANGLED_LENGTH, true, 0, "Strangle", Strength::Powerful,
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

GiantLizard::GiantLizard(Game* game)
	:NormalEnemy(game, HEALTH, "Giant Lizard", Art::GIANT_LIZARD_COLOR, Art::getGiantLizard())
{
	firstTurn = true;
	moves.push_back(new EnemyMoves::GetStatusImmunity(WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(
		new StatusAttackMove(
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

TheCollector::TheCollector(Game* game)
	:BossEnemy(
		game, HEALTH, "Collector", Art::COLLECTOR_COLOR, Art::getCollector(),
		std::vector<ColorString> {
			ColorString("Resistance is futile", Art::COLLECTOR_COLOR),
			ColorString("",ddutil::TEXT_COLOR) + ddutil::PATRIARCH_STRING + ColorString(" will destroy this world!", Art::COLLECTOR_COLOR)
		}
	)
{
	turnCounter = 0;
	moves.push_back(new EnemyMoves::CreateShield(SHIELD_BLOCK, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(FIRE_DAMAGE, new BurntStatus(), BURN_LENGTH, 0, "", Strength::Powerful, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::StrikeAndBlock(BLOCKSTRIKE_DAMAGE, BLOCKSTRIKE_BLOCK, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new SimpleAttackMove(THROUGH_BLOCK_DAMAGE, true, 0, "", Strength::Powerful, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Block(BLOCK_AMOUNT, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
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
		targets.push_back(this);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" will ", ddutil::TEXT_COLOR) +
			ColorString("block " + std::to_string(BLOCK_AMOUNT) + " damage next turn", ddutil::BLOCK_COLOR);
		break;
	case 5:
		chosenMove = moves[5];
		targets.push_back(ddutil::getHighestHealthPlayer(players));
		intent = ddutil::genericDamageIntent(HYPER_BEAM_DAMAGE, getColorString(), "Blast", targets);
		break;
	}

	turnCounter++;
	if (turnCounter > 5)
	{
		turnCounter = 1; // reset to begining but dont redo more thorns
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* TheCollector::makeCopy()
{
	return new TheCollector(game);
}

AbyssBeast::AbyssBeast(Game* game)
	:HardEnemy(game, HEALTH, "Abyssal Beast", Art::ABYSS_BEAST_COLOR, Art::getAbyssBeast())
{
	moves.push_back(new EnemyMoves::TakeVitalityGain(VITALITY_CHANGE));
	moves.push_back(
		new SelfDamageAttackMove(ATTACK_DAMAGE, false, SELF_DAMAGE, 0, "", Strength::Weak, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)
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

Basilisk::Basilisk(Game* game)
	:HardEnemy(game, HEALTH, "Basilisk", Art::BASILISK_COLOR, Art::getBasilisk())
{
	turnCounter = 0;
	moves.push_back(
		new SimpleStatusMove(
			new HexedStatus(), HEXED_LENGTH,
			true, 0, "", Strength::Powerful,
			WavFile("ghostinvulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC)
		)
	);
	moves.push_back(
		new SimpleStatusMove(
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


IntenseHardEnemy::IntenseHardEnemy(Game* game, int maxHp, std::string name, int color, Picture pic, std::vector<ColorString> intro)
	:Enemy(game, maxHp, name, color, pic, Mp3File("intensehardbattle"), intro, ddutil::MAP_SENEMY)
{
}

void IntenseHardEnemy::deathScene()
{
	Enemy::deathScene();
	game->changeScore(ddutil::STRONG_ENEMY_SCORE);
}

IntenseEnemy::IntenseEnemy(Game* game, int maxHp, std::string name, int color, Picture pic, std::vector<ColorString> intro)
	: Enemy(game, maxHp, name, color, pic, Mp3File("intensebattle"), intro, ddutil::MAP_NENEMY)
{
}

void IntenseEnemy::deathScene()
{
	Enemy::deathScene();
	game->changeScore(ddutil::NORM_ENEMY_SCORE);
}

SentientMouth::SentientMouth(Game* game)
	: IntenseEnemy(game, HEALTH, "Sentient Mouth", Art::MOUTH_COLOR, Art::getFloatingMouth(), 
		std::vector<ColorString> {
			ColorString("\"[unintelligible screaming]\"", Art::Art::MOUTH_COLOR)
		}
	)
{
	turnCounter = 0;
	moves.push_back(new StatusAttackMove(HEX_DAMAGE, new HexedStatus(), HEX_LENGTH, 0, "", Strength::Mythical, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(LICK_DAMAGE, new HexedStatus(), LICK_HEX_LENGTH, 0, "", Strength::Mythical, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(FROSTBURN_DAMAGE, new FrostBurntStatus(), FROSTBURN_LENGTH, 0, "", Strength::Mythical, WavFile("freeze", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
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

LaughingMask::LaughingMask(Game* game)
	: IntenseEnemy(game, HEALTH, "Laughing Mask", Art::MASK_COLOR, Art::getLaughingMask(),
		std::vector<ColorString> {
			ColorString("*The mask laughs menancingly*", Art::MASK_COLOR)
		}
	)
{
	turnCounter = 0;
	moves.push_back(new SimpleStatusMove(new VulnerableStatus(), VULN_LENGTH, true,0,"",Strength::Mythical, WavFile("laugh", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(HEX_DAMAGE, new HexedStatus(), HEX_LENGTH, 0, "", Strength::Mythical, WavFile("magicspell1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
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

PossessedMace::PossessedMace(Game* game)
	: IntenseEnemy(game, HEALTH, "Possessed Mace", Art::MACE_COLOR, Art::getMace(),
		std::vector<ColorString> {
			ColorString("Suddenly, the Mace springs to life...", Art::MACE_COLOR)
		}
	)
{
	applyStatus(new ThornsStatus(), THORNS);

	moves.push_back(new StatusAttackMove(ST_DAMAGE, new BleedingStatus(), BLEED_AMOUNT, 0, "", Strength::Mythical, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(MT_DAMAGE, new BleedingStatus(), BLEED_AMOUNT, 0, "", Strength::Mythical, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
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
		new StatusAttackMove(HYPER_BEAM_DAM, new FrostBurntStatus(), FROSTBURN_LENGTH, 0, "", Strength::Mythical, WavFile("magicattack3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
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

AncientBird::AncientBird(Game* game)
	: IntenseHardEnemy(game, HEALTH, "Ancient Bird", Art::ANC_BIRD_COLOR, Art::getAncientBird(),
		std::vector<ColorString> {
			ColorString("The Ancient Bird roars loudly!", Art::ANC_BIRD_COLOR),
		})
{
	turnCounter = 0;
	moves.push_back(new SimpleStatusMove(new InvulnerableStatus(), INVULN_TURNS, false, 0, "", Strength::Mythical, WavFile("fly", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
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


TheProtector::TheProtector(Game* game)
	:Enemy(
		game, HEALTH, "Protector", Art::PROTECTOR_COLOR, Art::getTheProtector(), Mp3File("bossbattle"), 
		std::vector<ColorString> {
			ColorString("\"You will never reach ", Art::PROTECTOR_COLOR) + ddutil::PATRIARCH_STRING + ColorString("!\"", Art::PROTECTOR_COLOR)
		},
		ddutil::MAP_BOSS
	)
{
	turnCounter = 0;
	shieldCounter = 0;
	moves.push_back(new StatusAttackMove(ICE_BEAM_DAM, new FrostBurntStatus(), BURN_LENGTH, 0, "", Strength::Mythical, WavFile("freeze", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(HEX_DAMAGE, new HexedStatus(), HEX_LENGTH, 0, "", Strength::Mythical, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::CreateShield(SHIELD_INCREASE, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new BlockBreakingMove(BLOCK_BREAK_DAMAGE, BLOCK_BREAK_MULT, 0, "", Strength::Mythical, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
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
				ColorString(" for "+ std::to_string(HEX_LENGTH) + " turns", ddutil::TEXT_COLOR);
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

Creature* TheProtector::makeCopy()
{
	return new TheProtector(game);
}

void TheProtector::deathScene()
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
	vwin->putcenSlowScroll(ColorString("The Protector fades away....", ddutil::TEXT_COLOR), bottomLine);
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

	vwin->putcenSlowScroll(ColorString("Lies the most magnificent ",ddutil::TEXT_COLOR)+ColorString("Palace",ddutil::YELLOW)+ColorString(" you have ever seen", ddutil::TEXT_COLOR), bottomLine, true);
	Menu::oneOptionMenu(vwin, ColorString("...", ddutil::TEXT_COLOR), Coordinate(0, bottomLine + 1), true);
	vwin->clearLine(bottomLine); vwin->clearLine(bottomLine + 1);

	game->setSkipToNextChapter();

	vwin->clearScreen();
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
	turnCounter = 0;
	desperation = false;
	moves.push_back(new EnemyMoves::TakeVitalityGain(VIT_CHANGE));
	moves.push_back(new EnemyMoves::Strike(BEAM_DAMAGE, WavFile("magicattack3", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Block(BIG_BLOCK, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::BlockBreakStrike(BLOCK_BREAK_DAMAGE, BLOCK_BREAK_MULT, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new EnemyMoves::Strike(DESPERATION_BEAM_DAM, WavFile("desperationbeam", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
}

EnemyTurn TruePatriarch::getTurn(std::vector<Creature*> players)
{
	Move* chosenMove = nullptr;
	ColorString intent;
	std::vector<Creature*> targets;



	if (getHealth() <= getMaxHealth(DESPERATION_THRESHOLD) && !desperation)
	{
		desperation = true;
		applyStatus(new InvulnerableStatus(), INVULN_LENGTH);
		stopSound(SoundType::MP3);
		playSound(Mp3File("finalbossbattle2"));
	}

	if (!desperation)
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
	else // desperation
	{
		chosenMove = moves[4];
		targets.push_back(players[ddutil::random(0, players.size() - 1)]);
		intent = ColorString("The ", ddutil::TEXT_COLOR) + getColorString() +
			ColorString(" is wildy aiming its Energy Beam at an unidentifiable target for ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(DESPERATION_BEAM_DAM) + " damage", ddutil::DAMAGE_COLOR) +
			ColorString("!", ddutil::TEXT_COLOR);
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* TruePatriarch::makeCopy()
{
	return new TruePatriarch(game);
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

VampireBatVariant::VampireBatVariant(Game* game)
	:NormalEnemy(game, HEALTH, "Vampire Bat", Art::VAMP_BAT_ALT_COLOR, Art::getVampireBatAlt())
{
	turnCounter = 0;
	moves.push_back(new EnemyMoves::Strike(SWIPE_DAMAGE, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(SPIT_DAMAGE, new BurntStatus(), BURN_LENGTH, 0, "Spit", Strength::Moderate,
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
		targets.push_back(ddutil::getLowestHealthPlayer(players));
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

CaveBatVariant::CaveBatVariant(Game* game)
	:NormalEnemy(game, HEALTH, "Cave Bat", Art::CAVEBATALT_COLOR, Art::getCaveBatAlt())
{
	moves.push_back(new SimpleStatusMove(new PoisonedStatus(), POISON_SPRAY_AMOUNT, true, 0, "Poison Spray",
		Strength::Powerful, WavFile("electricattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(POISON_BITE_DAMAGE, new PoisonedStatus(), POISON_BITE_AMOUNT, 0, "Poison Bite",
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
			ColorString(" intends to bite ", ddutil::TEXT_COLOR) + targets.front()->getColorString() +
			ColorString(" for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(POISON_BITE_DAMAGE) + " damage ", ddutil::DAMAGE_COLOR) +
			ColorString(" and ", ddutil::TEXT_COLOR) + ColorString(std::to_string(POISON_BITE_AMOUNT) + " poison", PoisonedStatus::COLOR);
	}

	return EnemyTurn(intent, targets, chosenMove);
}

Creature* CaveBatVariant::makeCopy()
{
	return new CaveBatVariant(game);
}

AncientLizard::AncientLizard(Game* game)
	: HardEnemy(game, HEALTH, "Ancient Lizard", Art::ANC_LIZARD_COLOR, Art::getAncientLizard())
{
	turnCounter = 0;
	moves.push_back(new EnemyMoves::Strike(STOMP_DAMAGE, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)));
	moves.push_back(new StatusAttackMove(CHARGE_DAMAGE, new StunnedStatus(), STUNNED_LENGTH, 0, "Charge",
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
