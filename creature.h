#ifndef CREATURE_H
#define CREATURE_H

#include <vector>
#include <map>

#include "colorstring.h"
#include "utilities.h"
#include "art.h"
#include "status.h"

class Game;
class Move;

class Creature
{
public:
	Creature(int maxHp, std::string name, int color, Picture pic, Game* theGame);
	~Creature();

	int increaseHealth(int amount); // returns the change in health
	void increaseMaxHealth(int amount);
	void decreaseMaxHealth(int amount);
	ddutil::DamageReport reduceHealth(int amount, Creature* attacker, bool ignoreBlock = false); // reduces health by amount (taking into account buffer) and sets alive flag appropriately
	void applyBlock(int amount); // will block "amount" of damage taken the current turn
	void resetBlock(); // sets buffer to baseBlock
	int getBlock(); // returns amount of block (buffer)
	int getHealth();
	int getMaxHealth(int percentage); // input percentage paramater to get that percentage of max health
	int getColor();

	// Statuses
	void applyStatus(Status* status, int amount = 1);
	bool hasStatus(StatusID status); // checks whether or not the creature has the status in question
	void clearAllStatuses(); // removes all status effects this creature has without applying their effects
	void clearAllStatuses(StatusID id); // removes all of the statuses of this type
	std::vector<ColorString> decrementAllStatuses(); // decreases each status's counter by 1 and applies their end turn effect
	int getNumberOfStatuses(); // returns the number of statuses the creature has (burn (2) counts as 2, frostburn (1) counts as 1 etc...)
	int getNumberOfStatuses(StatusID id); // returns the number of statuses of this specific type
	void removeStatus(StatusID status); // removes all instances of the status passed through without applying any effects
	int getNumberOfStatusTypes();

	virtual Creature* makeCopy() = 0;
	
	void increaseDodgeChance(int percentage); // increases it by base% + x%
	void resetDodgeChance(); // reset dodge to base%
	int getDodgeChance();
	void increaseBaseDodgeChance(int percentage); // increases base dodge chance by the percentage passed through

	ColorString throwCard(Creature* other);

	std::vector<Move*> getMoves();

	Picture getPicture();
	void changePicture(Picture picture);
	virtual void resetPicture() {} // overridden by creatures which may change their picture

	ColorString getColorString();

	virtual ColorString getStatLine();

	ColorString getStatusString();

	Game* getGamePtr();

	void addAttackStatus(Status* stat, int amount);
	void applyAttackStatuses(Creature* other);

	void increaseBaseBlock(int amount);
	void increasePercentHealBoost(int amount);
	void decreasePercentHealBoost(int amount);

	void setImmunityToStatuses(bool immune); // set whether this creature can take damage from status effects

	virtual void deathScene() = 0;

	void appendCount(int count);
	void appendToName(std::string n);
protected:
	int health;
	int maxHealth;
	int percentHealBoost;
	bool immuneToStatuses;

	int buffer; // how much damage will be absorbed on the next hit (blocked)
	int baseBlock; // what "buffer" gets reset back to every turn
	std::string name;
	bool alive;
	std::vector<Move*> moves; // initialized in the constructor of the base level of player and enemies
	int color;
	Picture picture; // vector of colorstrings that holds the ascii art for this creature

	int dodgeChance; // integer representing the percentage chance to dodge the next damage that does not go through block
	int baseDodgeChance; // value that dodge gets reset to


	std::map<Status*, int> currentStatuses; // list that contains all the current status afflictions this creature has for the current turn

	std::vector<std::pair<Status*, int>> attackStatuses; // statuses that are applied on every attack 


	Game* game;
};

#endif