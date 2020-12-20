#ifndef PLAYER_H
#define PLAYER_H

#include "creature.h"
#include "art.h"
#include "move.h"
#include "artifact.h"

class Enemy;

class Player : public Creature
{
public:
	Player(Game* game, int svit, int maxVit, int vitGain, int maxHp, int moveLim, std::string name, int color, Picture pic, bool minion);
	~Player();

	virtual std::vector<Move*> getRandomMoves(Strength str) = 0; // get random moves to choose from to add to moveset
	void tradeExperience();

	void displayArtifacts(); // prints out all the player's artifacts in the top of the screen

	void addVitality(int amount); // adds "amount" to vitality. Checks to make sure it isnt over max vitality.
	bool subVitality(int amount); // subtracts "amount" from vitality. Returns false if unable to subtract that amount
	int stealVitality(int amount); // takes away the passed through vitality, cannot go below zero, returns amount stolen
	void resetVitality(); // sets vitality to starting vitality
	int getStartingVitality();
	int getVitality();
	int getMaxVitality();
	int getVitalityGain();
	int getExperience(); // returns how much experience the player has

	void adjustVitalityGainTemp(int amount); // add "amount" to vitalityGainAdjustment
	void resetTempStatAdjustments();

	int gainExperience(double amount); // returns amount of experience gained
	bool loseExperience(int amount); // returns true if amount <= experience, and subtracts the amount. Returns false and does nothing if amount > experience

	virtual ColorString getStatLine(); // return the name, healthbar, health in number, and vitality information in a formatted colorstring

	bool isMinion();

	bool hasArtifact(ArtifactID id);
	std::vector<ColorString> getArtifactList();
	void getArtifact(Artifact* artifact);
	int getNumofArtifacts();

	void increasePercentXPBoost(int amount); // increases the amount of XP gained by amount%
	void decreasePercentXPBoost(int amount); // increases the amount of XP gained by amount%

	void increaseStartingVitality(int amount); // increases vitality that the player enters the battle with
	void increaseVitalityPerTurn(int amount);

	void applyStartingStatuses(Enemy* enemy); // applies starting statuses to enemy and self
	void addSelfStartingStatus(Status* stat, int amount);
	void addEnemyStartingStatus(Status* stat, int amount);

	void increaseMovesetLimit(int amount);

	void deathScene() override;
protected:
	std::vector<Artifact*> artifacts;

	std::vector<std::pair<Status*, int>> selfStartingStatuses; // statuses that are applied to self at start of battle
	std::vector<std::pair<Status*, int>> enemyStartingStatuses; // statuses that are applied to enemy at start of battle

	int startingVitality; // how much vitality the player enters a battle with
	int vitality; // vitality is a measure of how many moves a player can use per turn. Each move costs vitality.
	int maxVitality; // the limit of how much vitality a player can accumulate
	int vitalityGain; // how much vitality the player gains after every turn
	int experience; // experience can be traded in for various rewards, like new moves
	int movesetLimit; // how many moves the player can have at once

	int percentXPBoost; // represents the percet amount of extra XP the player should gain from artifacts / other boosts

	int vitalityGainAdjustment; // value that is added to vitality gain and is used to keep track of temporary adjustments to vitality gain
								// that go away after the battle

	bool minion; // whether or not the object is a core member of the player party or a temporary minion that leaves after the battle
};

class Samurai : public Player
{
public:
	Samurai(Game* game);
	std::vector<Move*> getRandomMoves(Strength str) override;
	
	void resetPicture() override;

	Creature* makeCopy() override;

	static const int STARTING_VITALITY = 3;
	static const int MAX_VITALITY = 9;
	static const int VITALITY_GAIN = 3;
	static const int MAX_HP = 55;
	static const int MAX_MOVES = 5;
};

class Gunslinger : public Player
{
public:
	Gunslinger(Game* game);
	std::vector<Move*> getRandomMoves(Strength str) override;

	void resetPicture() override;
	Creature* makeCopy() override;

	int getReserveBullets();
	bool useBullet(int amount); // returns true if you have enough bullets for the attack, and uses the bullets. Does nothing if false.
	void createBullets(int amount);

	static const int STARTING_VITALITY = 3;
	static const int MAX_VITALITY = 8;
	static const int VITALITY_GAIN = 2;
	static const int MAX_HP = 40;
	static const int MAX_MOVES = 6;
	static const int STARTING_MAX_BULLETS = 9;
	static const int STARTING_BULLETS = 9;

	ColorString getStatLine(); // overrides the basic function to append the amount of bullets in reserve
private:
	int reserveBullets;
	int maxBullets;
};

class Sorcerer : public Player
{
public:
	Sorcerer(Game* game);
	std::vector<Move*> getRandomMoves(Strength str) override;

	void resetPicture() override;
	Creature* makeCopy() override;

	static const int STARTING_VITALITY = 10;
	static const int MAX_VITALITY = 20;
	static const int VITALITY_GAIN = 3;
	static const int MAX_HP = 45;
	static const int MAX_MOVES = 5;
};

class PlayerMinion : public Player
{
public:
	PlayerMinion(Game* game, int svit, int maxVit, int vitGain, int maxHp, int moveLim, std::string name, int color, Picture pic);

	std::vector<Move*> getRandomMoves(Strength str) override;

};

class FireImp : public PlayerMinion
{
public:
	FireImp(Game* game);
	Creature* makeCopy() override;
private:
	int count;
};

class IceImp : public PlayerMinion
{
public:
	IceImp(Game* game);
	Creature* makeCopy() override;
private:
	int count;
};

class Fairy : public PlayerMinion
{
public:
	Fairy(Game* game);
	Creature* makeCopy() override;
private:
	int count;
};

class FireDragon : public PlayerMinion
{
public:
	FireDragon(Game* game);
	Creature* makeCopy() override;
private:
	int count;
};

class IceDragon : public PlayerMinion
{
public:
	IceDragon(Game* game);
	Creature* makeCopy() override;
private:
	int count;
};


class SpiritKnight : public PlayerMinion
{
public:
	SpiritKnight(Game* game);
	Creature* makeCopy() override;
private:
	int count;
};

#endif