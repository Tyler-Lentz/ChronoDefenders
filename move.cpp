#include "move.h"
#include "utilities.h"
#include "colorstring.h"
#include "creature.h"
#include "soundfile.h"

#include <string>

Move::Move(std::string desc, std::string theName, int theCost, Strength howStrong, bool targetChoose, WavFile theSound)
{
	description = desc;
	cost = theCost;
	strength = howStrong;
	name = theName;
	chooseTarget = targetChoose;
	sound = theSound;
}

std::string Move::getDescription()
{
	return description;
}

ColorString Move::getColorString()
{
	int color = ddutil::TEXT_COLOR;
	switch (strength)
	{
	case Strength::Weak:
		color = ddutil::WEAK_COLOR;
		break;
	case Strength::Moderate:
		color = ddutil::MODERATE_COLOR;
		break;
	case Strength::Powerful:
		color = ddutil::POWERFUL_COLOR;
		break;
	case Strength::Mythical:
		color = ddutil::MYTHICAL_COLOR;
		break;
	}
	return ColorString(name, color);
}

ColorString Move::getFullInformation()
{
	return ColorString("(" + std::to_string(cost) + ") ", ddutil::VITALITY_COLOR) +
		getColorString() + ColorString(": ", ddutil::TEXT_COLOR) +
		ColorString(getDescription(), ddutil::TEXT_COLOR);
}

int Move::getCost()
{
	return cost;
}

bool Move::canChooseTarget()
{
	return chooseTarget;
}

void Move::playSoundEffect()
{
	sound.play();
}


StatusAttackMove::StatusAttackMove(int theDamage, Status* theStatus, int statusAmount, int cost, std::string name, Strength str, WavFile theSound)
	:Move("Deals " + std::to_string(theDamage) + " damage and makes the target " + theStatus->getName().getString() +
		(statusAmount > 1 ? (" " + std::to_string(statusAmount) + " times") : ("")),
		name, cost, str, true, theSound)
{
	damage = theDamage;
	status = theStatus;
	statusStack = statusAmount;
}

StatusAttackMove::~StatusAttackMove()
{
	if (status != nullptr)
	{
		delete status;
	}
}

ColorString StatusAttackMove::doAction(Creature* self, Creature* other)
{
	ddutil::DamageReport damRep = other->reduceHealth(damage, self);
	Status* newStat = status->makeCopy();
	ColorString statusName = newStat->getName();
	other->applyStatus(newStat, statusStack);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" deals ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep) +
		ColorString(" and makes the ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString(" ", ddutil::TEXT_COLOR) + statusName;
}

SimpleAttackMove::SimpleAttackMove(int dam, bool blockIgnore, int cost, std::string name, Strength str, WavFile theSound)
	:Move("Deals " + std::to_string(dam) + " damage" + (blockIgnore ? " through block" : ""), name, cost, str, true, theSound)
{
	damage = dam;
	ignoreBlock = blockIgnore;
}

ColorString SimpleAttackMove::doAction(Creature* self, Creature* other)
{
	ddutil::DamageReport damRep = other->reduceHealth(damage, self, ignoreBlock);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" deals ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep) +
		ColorString(" to the ", ddutil::TEXT_COLOR) + other->getColorString();
}

SimpleStatusMove::SimpleStatusMove(Status* stat, int statAmount, bool chooseTarget, int cost, std::string name, Strength str, WavFile theSound)
	:Move("Applies " + stat->getName().getString() + " to " + ( chooseTarget ? "the target" : "the user"),
		name, cost, str, chooseTarget, theSound)
{
	status = stat;
	statusAmount = statAmount;
}

SimpleStatusMove::~SimpleStatusMove()
{
	if (status != nullptr)
	{
		delete status;
	}
}

ColorString SimpleStatusMove::doAction(Creature* self, Creature* other)
{
	Status* newStatus = status->makeCopy();
	ColorString statusName = newStatus->getName();
	other->applyStatus(newStatus, statusAmount);
	if (self == other)
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
			ColorString(" becomes ", ddutil::TEXT_COLOR) + statusName;
	}
	else
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
			ColorString(" makes the ", ddutil::TEXT_COLOR) + other->getColorString() +
			ColorString(" ", ddutil::TEXT_COLOR) + statusName;
	}
	
}


SelfBlockMove::SelfBlockMove(int blockAmount, int cost, std::string name, Strength str, WavFile theSound)
	:Move("Blocks " + std::to_string(blockAmount) + " damage", name, cost, str, false, theSound)
{
	block = blockAmount;
}

ColorString SelfBlockMove::doAction(Creature* self, Creature* other)
{
	self->applyBlock(block);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" will block ", ddutil::TEXT_COLOR) + ddutil::genericBlockAlert(block);

}


AttackAndBlockMove::AttackAndBlockMove(int moveDamage, int moveBlock, bool blockIgnore, int cost, std::string name, Strength str, WavFile theSound)
	:SimpleAttackMove(moveDamage, blockIgnore, cost, name, str, theSound)
{
	block = moveBlock;
	description += " and blocks " + std::to_string(block) + " damage";
}


ColorString AttackAndBlockMove::doAction(Creature* self, Creature* other)
{
	ColorString baseStr = SimpleAttackMove::doAction(self, other);
	baseStr += ColorString(" and will block ", ddutil::TEXT_COLOR) + ddutil::genericBlockAlert(block);
	self->applyBlock(block);
	return baseStr;
}

SimpleHealMove::SimpleHealMove(int theHealAmount, bool canChooseTarget, int cost, std::string name, Strength str)
	:Move("Heals " + std::to_string(theHealAmount) + " health for the " + (canChooseTarget ? "target" : "caster"), name, cost, str, canChooseTarget,
		WavFile("heal", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
	healAmount = theHealAmount;
}

ColorString SimpleHealMove::doAction(Creature* self, Creature* other)
{
	Creature* target;
	if (canChooseTarget())
	{
		target = other;
	}
	else
	{
		target = self;
	}

	int actualHealAmount = target->increaseHealth(healAmount);

	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() + ColorString(" heals ", ddutil::TEXT_COLOR) + ColorString(std::to_string(actualHealAmount) + " health", ddutil::HEAL_COLOR);
}

MultiAttackMove::MultiAttackMove(int damagePerHit, int numHits, int cost, std::string name, Strength str, WavFile theSound)
	:Move("Deals " + std::to_string(damagePerHit) + " damage " + std::to_string(numHits) + " times", name, cost, str, true, theSound)
{
	damage = damagePerHit;
	times = numHits;
}

ColorString MultiAttackMove::doAction(Creature* self, Creature* other)
{
	ColorString info = ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" attacks the ", ddutil::TEXT_COLOR) + other->getColorString() +
		ColorString(" " + std::to_string(times) + " times: ", ddutil::TEXT_COLOR);
	for (int i = 0; i < times; i++)
	{
		info += ddutil::genericDamageAlert(other->reduceHealth(damage, self)) + ColorString("; ", ddutil::TEXT_COLOR);
	}
	ddutil::trimLength(info);
	return info;
}

BlockBreakingMove::BlockBreakingMove(int theBaseDamage, int theMultiplier, int cost, std::string name, Strength str, WavFile theSound)
	:Move("Deals " + std::to_string(theBaseDamage) + " damage (x" + std::to_string(theMultiplier) + " if the enemy has block)", name, cost, str, true, theSound)
{
	baseDamage = theBaseDamage;
	multiplier = theMultiplier;
}

ColorString BlockBreakingMove::doAction(Creature* self, Creature* other)
{
	int actualDamage = baseDamage;
	if (other->getBlock() > 0)
	{
		actualDamage *= multiplier;
	}

	SimpleAttackMove tempMove(actualDamage, false, 0, "TEMP", Strength::Moderate, sound);
	return tempMove.doAction(self, other);
}

SelfDamageAttackMove::SelfDamageAttackMove(int moveDamage, bool blockIgnore, int damageTaken, int cost, std::string name, Strength str, WavFile theSound)
	:SimpleAttackMove(moveDamage, blockIgnore, cost, name, str, theSound)
{
	selfDamage = damageTaken;
	description += " but inflicts " + std::to_string(selfDamage) + " self-damage";
}

ColorString SelfDamageAttackMove::doAction(Creature* self, Creature* other)
{
	ColorString baseStr = SimpleAttackMove::doAction(self, other);

	ddutil::DamageReport d = self->reduceHealth(selfDamage, nullptr);

	baseStr += ColorString(" and takes ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(d);
	return baseStr;
}