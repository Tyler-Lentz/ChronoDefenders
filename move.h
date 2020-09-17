#ifndef MOVE_H
#define MOVE_H

class Creature;
class Status;

#include "colorstring.h"
#include "soundfile.h"

#include <string>
#include <vector>

enum class Strength {
	Weak,
	Moderate,
	Powerful, 
	Mythical
};

class Move
{
public:
	Move(std::string desc, std::string theName, int cost, Strength howStrong, bool targetChoose, WavFile theSound);
	virtual ColorString doAction(Creature* self, Creature* other) = 0;
	std::string getDescription();

	ColorString getColorString();
	ColorString getFullInformation();

	int getCost();

	bool canChooseTarget();

	void playSoundEffect();
private:
	bool chooseTarget;
	std::string name;
	int cost; // how much vitality a move uses up
	Strength strength; // what category of move it belongs to
protected:
	std::string description;
	WavFile sound;
};

// Base class for a move which deals damage and inflicts a status
class StatusAttackMove : public Move
{
public:
	StatusAttackMove(int dam, Status* status, int statusAmount, int cost, std::string name, Strength str, WavFile theSound);
	~StatusAttackMove();
	ColorString doAction(Creature* self, Creature* other) override;
private:
	Status* status;
	int damage;
	int statusStack;
};

// Base class for a move which simply deals damage
class SimpleAttackMove : public Move
{
public:
	SimpleAttackMove(int dam, bool blockIgnore, int cost, std::string name, Strength str, WavFile theSound);

	ColorString doAction(Creature* self, Creature* other) override;
private:
	int damage;
	bool ignoreBlock;
};

// Base class for a move which just applies a status
class SimpleStatusMove : public Move
{
public:
	SimpleStatusMove(Status* stat, int statAmount, bool chooseTarget, int cost, std::string name, Strength str, WavFile theSound);
	~SimpleStatusMove();

	ColorString doAction(Creature* self, Creature* other);
private:
	Status* status;
	int statusAmount;
};

// Base class for a move which gives block to the user
class SelfBlockMove : public Move
{
public:
	SelfBlockMove(int blockAmount, int cost, std::string name, Strength str, WavFile theSound);

	ColorString doAction(Creature* self, Creature* other);
private:
	int block;
};

class AttackAndBlockMove : public SimpleAttackMove
{
public:
	AttackAndBlockMove(int moveDamage, int moveBlock, bool blockIgnore, int cost, std::string name, Strength str, WavFile theSound);

	ColorString doAction(Creature* self, Creature* other) override;

private:
	int block;
};

//base class for a move which applies healing
class SimpleHealMove : public Move
{
public:
	SimpleHealMove(int healAmount, bool canChooseTarget, int cost, std::string name, Strength str);

	ColorString doAction(Creature* self, Creature* other) override;
private:
	int healAmount;
};

class MultiAttackMove : public Move
{
public:
	MultiAttackMove(int damage, int times, int cost, std::string name, Strength str, WavFile theSound);
	ColorString doAction(Creature* self, Creature* other) override;
private:
	int damage; // how much damage each hit does
	int times; // how many times it hits the target
};

// move that has a multiplier for damage when the enemy has block
class BlockBreakingMove : public Move
{
public:
	BlockBreakingMove(int theBaseDamage, int theMultiplier, int cost, std::string name, Strength str, WavFile theSound);
	ColorString doAction(Creature* self, Creature* other) override;
private:
	int baseDamage;
	int multiplier; // how many times more damage it does if the enemy has block
};

class SelfDamageAttackMove : public SimpleAttackMove
{
public:
	SelfDamageAttackMove(int moveDamage, bool blockIgnore, int damagetaken, int cost, std::string name, Strength str, WavFile theSound);

	ColorString doAction(Creature* self, Creature* other) override;
private:
	int selfDamage;
};

#endif