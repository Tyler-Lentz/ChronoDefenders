#ifndef ENEMY_H
#define ENEMY_H

#include "creature.h"
#include "art.h"
#include "soundfile.h"

#include <vector>

class Move;
class Player;
class Game;

class EnemyTurn
{
public:
	EnemyTurn(ColorString theIntent, std::vector<Creature*> theTargets, Move* theMove);
	void checkForDeadTargets();
	ColorString getIntent();
	const std::vector<Creature*>& getTargets();
	Move* getMove();
private:
	ColorString intent;
	std::vector<Creature*> targets; // vector of whom it is attacking
	Move* move;
};

class Enemy : public Creature
{
public:
	Enemy(Game* game, int maxHp, std::string name, int color, Picture pic, Mp3File theMusic, std::vector<ColorString> intro, char mapChar);
	virtual EnemyTurn getTurn(std::vector<Creature*> players) = 0;

	virtual ColorString getStatLine();

	const Mp3File& getMusic() const;

	const std::vector<ColorString>& getIntroText() const;

	char getMapChar();

	virtual void deathScene() override;
private:
	std::vector<ColorString> introText;
	Mp3File music;
	char mapChar;
};

class NormalEnemy : public Enemy
{
public:
	NormalEnemy(Game* game, int maxHp, std::string name, int color, Picture pic);
	void deathScene() override;
};

class Ghost : public NormalEnemy
{
public:
	Ghost(Game* game);

	EnemyTurn getTurn(std::vector<Creature*> players) override;

	Creature* makeCopy() override;

	const static int HEALTH = 70;
	const static int BURN_AMOUNT = 4;
	const static int INVULNERABILITY_LENGTH = 2;
	const static int SINGLE_TARGET_DAMAGE = 12;
	const static int MULTI_TARGET_DAMAGE = 4;

private:
	int turnCounter;
};

class GhostVariant : public NormalEnemy
{
public:
	GhostVariant(Game* game);

	EnemyTurn getTurn(std::vector<Creature*> players) override;

	Creature* makeCopy() override;

	const static int HEALTH = 90;
	const static int FBURN_AMOUNT = 3;
	const static int SINGLE_TARGET_DAMAGE = 12;
	const static int HEAL_AMOUNT = 10;
private:
	int turnCounter;
};

class GiantSnail : public NormalEnemy
{
public:
	GiantSnail(Game* game);

	EnemyTurn getTurn(std::vector<Creature*> players) override;

	Creature* makeCopy() override;

	const static int HEALTH = 60;
	const static int SINGLE_TARGET_DAMAGE = 10;
	const static int MULTI_TARGET_DAMAGE = 6;
	const static int BLOCK = 20;
	const static int BLOCK_PER_ATTACK = 5;
private:
	int turnCounter;
};

class GiantSnailVariant : public NormalEnemy
{
public:
	GiantSnailVariant(Game* game);

	EnemyTurn getTurn(std::vector<Creature*> players) override;

	Creature* makeCopy() override;

	const static int HEALTH = 72;
	const static int HIGH_HP_DAMAGE = 10;
	const static int LOW_HP_DAMAGE = 14;
};

class VampireBat : public NormalEnemy
{
public:
	VampireBat(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 66;
	const static int STRIKE_DAMAGE = 11;
	const static int LIFESTEAL_AMOUNT = 9;
private:
	int turnCounter;
};

class VampireBatVariant : public NormalEnemy
{
public:
	VampireBatVariant(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 72;
	const static int SWIPE_DAMAGE = 8;// (multi attack on everyone)
	const static int SPIT_DAMAGE = 10;
	const static int FBURN_LENGTH = 3;
	const static int LIFESTEAL_BITE_DAMAGE = 8;
	const static int LIFESTEAL_AMOUNT = 8;
private:
	int turnCounter;
};

class CaveBat : public NormalEnemy
{
public:
	CaveBat(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 70;
	const static int DIVEBOMB_THRESHOLD = 30; // hp at which will do suicide attack
	const static int WING_ATTACK_DAMAGE = 10;
	const static int DIVEBOMB_DAMAGE = 22; // dies after doing this move
};

class CaveBatVariant : public NormalEnemy
{
public:
	CaveBatVariant(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 80;
	const static int POISON_SPRAY_AMOUNT = 2;
	const static int POISON_BITE_AMOUNT = 4;
	const static int POISON_BITE_DAMAGE = 4;

};

class Minion : public NormalEnemy
{
public:
	Minion(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 150;
	const static int DUAL_STRIKE_DAMAGE = 9;
	const static int BLOCK_BREAK_DAMAGE = 11;
	const static int BLOCK_MULTIPLIER = 2;
	const static int BASE_BLOCK = 13;
private:
	int turnCounter;
};

class MinionAlt : public NormalEnemy
{
public:
	MinionAlt(Game* game);
	EnemyTurn getTurn(std::vector <Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 180;
	const static int DUAL_STRIKE_DAMAGE = 10;
	const static int BLOCK_BREAK_DAMAGE = 12;
	const static int BLOCK_MULTIPLIER = 2;
};

class GiantLizard : public NormalEnemy
{
public:
	GiantLizard(Game* game);
	EnemyTurn getTurn(std::vector <Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 168;
	const static int FLAME_DAMAGE = 11;
	const static int BURN_LENGTH = 1;
	const static int KICK_DAMAGE = 20;
private:
	bool firstTurn;
};

class AltGiantLizard : public NormalEnemy
{
public:
	AltGiantLizard(Game* game);
	EnemyTurn getTurn(std::vector <Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 168;
	const static int KICK_DAMAGE = 16;
	const static int THORNS_DAMAGE = 3;

private:
	bool firstTurn;
	int target; // integer representing the index of the player to attack, is incremented every turn and resets to 0 when too high
};

class FireBat : public NormalEnemy
{
public:
	FireBat(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 150;
	const static int WHIRLWIND_DAMAGE = 10; // multi attack
	const static int WHIRLWIND_BURN_LENGTH = 4;
	const static int SCORCH_LENGTH = 1; // attack that applies scorched on an enemy
	const static int STRIKE_DAMAGE = 10; // single attack/block
	const static int STRIKE_BLOCK = 20;
	const static int BITE_DAMAGE = 6; // double attack
	const static int BITE_TIMES = 2;

private:
	int turnCounter;
};

class FireBatVariant : public NormalEnemy
{
public:
	FireBatVariant(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 150;
	const static int BASE_BLOCK = 8; // One time gets base block enveloping in shield of fire
	const static int BURN_LENGTH = 6; // Single target high burn attack
	const static int BURN_DAMAGE = 10;
	const static int SCORCHED_LENGTH = 1; // Attack that just applies scorched
private:
	int turnCounter;
	Creature* previousPerson;
};

class FirePlatypus : public NormalEnemy
{
public:
	FirePlatypus(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 180;
	const static int STRIKE_DAMAGE = 10; // attack that also gives block
	const static int STRIKE_BLOCK = 25;
	const static int FLAMETHROWER_DAMAGE = 10; // multitarget flame attack
	const static int FLAMETHROWER_BURN_LENGTH = 3;
	const static int BODY_SLAM_DAMAGE = 9; // Multitarget stun attack
	const static int STUN_LENGTH = 2;
	const static int HEAL_AMOUNT = 20; // heal move
private:
	int turnCounter;
};

class HardEnemy : public Enemy
{
public:
	HardEnemy(Game* game, int maxHp, std::string name, int color, Picture pic);
	void deathScene() override;
};

class Minotaur : public HardEnemy
{
public:
	Minotaur(Game* game);

	EnemyTurn getTurn(std::vector<Creature*> players) override;

	Creature* makeCopy() override;

	const static int STRIKE_DAMAGE = 15;
	const static int MULTI_TARGET_DAMAGE = 9;
	const static int BLOCK_PER_ATTACK = 5;
	const static int HEALTH = 100;
};

class Brute : public HardEnemy
{
public:
	Brute(Game* game);

	EnemyTurn getTurn(std::vector<Creature*> players) override;

	Creature* makeCopy() override;

	const static int HEALTH = 130;
	const static int BASE_DAMAGE = 5;
	const static int DAMAGE_INCREASE = 3;

};

class AncientLizard : public HardEnemy 
{
public:
	AncientLizard(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 133;
	const static int STOMP_DAMAGE = 10;
	const static int CHARGE_DAMAGE = 10;
	const static int STUNNED_LENGTH = 3;
	const static int HEAL_AMOUNT = 20;
private:
	int turnCounter;
};

class AbyssBeast : public HardEnemy
{
public:
	AbyssBeast(Game* game);

	EnemyTurn getTurn(std::vector<Creature*> players) override;

	Creature* makeCopy() override;

	const static int HEALTH = 260;
	const static int ATTACK_DAMAGE = 22;
	const static int SELF_DAMAGE = 6;
	const static int VITALITY_CHANGE = -1;
private:
	bool firstTurn;
};

class Basilisk : public HardEnemy
{
public:
	Basilisk(Game* game);

	EnemyTurn getTurn(std::vector<Creature*> players) override;

	Creature* makeCopy() override;

	const static int HEALTH = 220;
	const static int HEXED_LENGTH = 3;
	const static int POISON_AMOUNT = 5;
	const static int STRANGLE_DAMAGE = 19;

private:
	int turnCounter;
};

class GiantHead : public HardEnemy
{
public:
	GiantHead(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;
	const static int HEALTH = 300;
	const static int STUN_DAMAGE = 16;
	const static int STRENGTH_INCREASE = 4;
private:
	bool attackTurn;
};

// enemies with the intense music (chapter 3 and beyond + early event fights in chapter 1)
class IntenseEnemy : public Enemy
{
public:
	IntenseEnemy(Game* game, int maxHp, std::string name, int color, Picture pic, std::vector<ColorString> intro);
	void deathScene() override;
private:

};

class MysteriousKnight : public IntenseEnemy
{
public:
	MysteriousKnight(Game* game);

	EnemyTurn getTurn(std::vector<Creature*> players) override;

	Creature* makeCopy() override;

	const static int HEALTH = 240;
	const static int VULNERABLE_LENGTH = 99;
	const static int SINGLE_TARGET_DAMAGE = 20;
	const static int BLOCK = 3;
private:
	bool firstTurn;
};

class Spider : public IntenseEnemy
{
public:
	Spider(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 125;
	const static int STRANGLED_LENGTH = 2;
	const static int STRIKE_DAMAGE = 15;
	const static int BASE_BLOCK = 6;
private:
	int turnCounter;
};

class SentientMouth : public IntenseEnemy
{
public:
	SentientMouth(Game* game);
	EnemyTurn getTurn(std::vector <Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 250;
	const static int HEX_LENGTH = 2;
	const static int HEX_DAMAGE = 13;
	const static int LICK_DAMAGE = 18;
	const static int LICK_HEX_LENGTH = 1;
	const static int FROSTBURN_LENGTH = 2;
	const static int FROSTBURN_DAMAGE = 15;
private:
	int turnCounter;
};

class LaughingMask : public IntenseEnemy
{
public:
	LaughingMask(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 280;

	const static int VULN_LENGTH = 10;
	const static int HEX_LENGTH = 4; // one time long hex length
	const static int HEX_DAMAGE = 12;
	
	const static int HEAD_BUTT_DAM = 18; // single target headbutt
	const static int HEAD_BUTT_BLOCK = 10;
private:
	int turnCounter;
};

class DevilishMask : public IntenseEnemy
{
public:
	DevilishMask(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 300;

	const static int BURN_LENGTH = 4;
	// damage per burn is same as sorceress's combust move
private:
	int turnCounter;
};

class PossessedMace : public IntenseEnemy
{
public:
	PossessedMace(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 280;
	const static int THORNS = 4;

	const static int ST_DAMAGE = 18; // single target damage
	const static int BLEED_AMOUNT = 3;

	const static int MT_DAMAGE = 12; // multi target damage
};

class Sniffer : public IntenseEnemy
{
public:
	Sniffer(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 250;
	const static int RAVAGE_DAMAGE = 18; // Single target attack
	const static int RAVAGE_VULNERABLE_LENGTH = 3;
	const static int STRANGLE_DAMAGE = 14; // Single target attack
	const static int STRANGLE_LENGTH = 2;
	const static int LIFE_STEAL_AMOUNT = 10; // Multi target lifesteal attack
private:
	int turnCounter;
};

class SnifferVariant : public IntenseEnemy
{
public:
	SnifferVariant(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 280;
	const static int FROST_BREATH_DAMAGE = 12; // Multitarget frostburn attack
	const static int FROST_BURN_LENGTH = 2;
	const static int BIG_BLOCK_AMOUNT = 40; // blocks a lot in one turn
	const static int BLOCK_ATTACK_DAMAGE = 20; // blocks and attacks in one turn
	const static int BLOCK_ATTACK_BLOCK = 18;
private:
	int previousMove;
};

class IntenseHardEnemy : public Enemy
{
public:
	IntenseHardEnemy(Game* game, int maxHp, std::string name, int color, Picture pic, std::vector<ColorString> intro);
	void deathScene() override;
};

class CorruptedDisciple : public IntenseHardEnemy
{
public:
	CorruptedDisciple(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 375;
	const static int HEXED_LENGTH = 3; // single target hex attack
	const static int HEX_DAMAGE = 14;
	const static int LIGHTNING_STRIKE_DAMAGE = 25; // single target lightning strike
	const static int ZAPPED_AMOUNT = 7;
	const static int HEAL_AMOUNT = 30;
private:
	int turnCounter;
};

class HyperBeast : public IntenseHardEnemy
{

public:
	HyperBeast(Game* game);
	EnemyTurn getTurn(std::vector <Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 400;
	const static int HYPER_BEAM_DAM = 40;
	const static int VIT_CHANGE = -1;
	const static int FROSTBURN_LENGTH = 3;
	const static int HEAL_AMOUNT = 40;
private:
	int turnCounter;
	bool alreadyStoleVitality;
};

class AncientBird : public IntenseHardEnemy
{

public:
	AncientBird(Game* game);
	EnemyTurn getTurn(std::vector <Creature*> players) override;
	Creature* makeCopy() override;

	const static int HEALTH = 400;
	const static int VITALITY_STEAL = 6; // takes away vitality
	const static int DIVEBOMB_DAMAGE = 36;
	const static int INVULN_TURNS = 2;
	const static int DUALSTRIKE_DAM = 12;

private:
	int turnCounter;
};

class BossEnemy : public Enemy
{
public:
	BossEnemy(Game* game, int maxHp, std::string name, int color, Picture pic, std::vector<ColorString> intro);
	void deathScene() override;
};

class TheMessenger : public BossEnemy
{
public:
	TheMessenger(Game* game);

	EnemyTurn getTurn(std::vector<Creature*> players) override;

	Creature* makeCopy() override;

	const static int HEALTH = 230;
	const static int VULNERABLE_LENGTH = 3;
	const static int BURN_LENGTH = 3;
	const static int SINGLE_TARGET_DAMAGE = 16;
	const static int INVULNERABLE_LENGTH = 3;

private:
	int turnCounter;
};

class TheCollector : public BossEnemy
{
public:
	TheCollector(Game* game);

	EnemyTurn getTurn(std::vector<Creature*> players) override;

	Creature* makeCopy() override;

	const static int HEALTH = 400;
	const static int SHIELD_BLOCK = 8;
	const static int FIRE_DAMAGE = 15;
	const static int BURN_LENGTH = 3;
	const static int BLOCKSTRIKE_DAMAGE = 10;
	const static int BLOCKSTRIKE_BLOCK = 10;
	const static int THROUGH_BLOCK_DAMAGE = 9;
	const static int HYPER_BEAM_DAMAGE = 40;
	const static int BLOCK_AMOUNT = 50;

private:
	int turnCounter;
};

class TheProtector : public Enemy
{
public:
	TheProtector(Game* game);

	EnemyTurn getTurn(std::vector<Creature*> players) override;

	Creature* makeCopy() override;

	const static int HEALTH = 700;
	void deathScene() override;

	const static int ICE_BEAM_DAM = 22; // multitarget ice blast
	const static int BURN_LENGTH = 4;

	const static int HEX_DAMAGE = 15; // single target hex the player
	const static int HEX_LENGTH = 3;

	const static int SHIELD_INCREASE = 12;

	const static int BLOCK_BREAK_DAMAGE = 15;
	const static int BLOCK_BREAK_MULT = 2;

	const static int THROUGH_BLOCK_DAMAGE = 9;

	const static int SMALL_HEAL_AMOUNT = 50;

	const static int BIG_HEAL_THRESHOLD = 10; // percent of max hp where tries to heal
	const static int BIG_HEAL_AMOUNT = 100;
private:
	int turnCounter;
	int shieldCounter; // keeps track of how many times he has increased his shield
};

class TruePatriarch : public Enemy
{
public:
	TruePatriarch(Game* game);
	EnemyTurn getTurn(std::vector<Creature*> players) override;
	Creature* makeCopy() override;
	void deathScene() override;

	const static int HEALTH = 800;

	const static int VIT_CHANGE = -1;
	const static int BEAM_DAMAGE = 20; // multitarget beam
	const static int BIG_BLOCK = 66;
	
	const static int BLOCK_BREAK_DAMAGE = 20;
	const static int BLOCK_BREAK_MULT = 2;

	const static int DESPERATION_THRESHOLD = 20; // at less than 20% of max hp go to desperation mode
	
	const static int INVULN_LENGTH = 2;
	const static int DESPERATION_BEAM_DAM = 45; // solo target
private:
	int turnCounter;
	bool desperation;
};


#endif