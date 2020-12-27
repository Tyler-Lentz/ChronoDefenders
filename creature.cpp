#include <algorithm>

#include "move.h"
#include "creature.h"
#include "art.h"
#include "status.h"
#include "game.h"
#include "colorstring.h"
#include "player.h"

// damage report

ddutil::DamageReport::DamageReport(int dBlocked, int dTaken)
{
	damageBlocked = dBlocked;
	damageTaken = dTaken;
}

int ddutil::DamageReport::getDamageBlocked()
{
	return damageBlocked;
}

int ddutil::DamageReport::getDamageTaken()
{
	return damageTaken;
}

// Creature

Creature::Creature(int maxHp, std::string theName, int theColor, Picture pic, Game* theGame)
{
	health = maxHp;
	maxHealth = maxHp;
	buffer = 0;
	alive = true;
	name = theName;
	color = theColor;
	picture = pic;
	baseDodgeChance = 0;
	dodgeChance = baseDodgeChance;
	game = theGame;
	baseBlock = 0;
	percentHealBoost = 0;
}

Creature::~Creature()
{
	for (unsigned int i = 0; i < moves.size(); i++)
	{
		delete moves[i];
	}
	clearAllStatuses();
	for (unsigned int i = 0; i < attackStatuses.size(); i++)
	{
		delete attackStatuses[i].first;
	}
}

std::vector<Move*> Creature::getMoves()
{
	return moves;
}

Picture Creature::getPicture()
{
	return picture;
}

void Creature::changePicture(Picture picture)
{
	this->picture = picture;
}

ColorString Creature::getColorString()
{
	return ColorString(name, color);
}

ColorString Creature::getStatLine()
{
	ColorString divideChar = ColorString(" ", ddutil::WHITE);
	ColorString name = getColorString();
	ColorString healthBar = ddutil::getHealthBar(getHealth(), getMaxHealth(100), ddutil::PLAYER_HEALTHBAR_LENGTH);
	ColorString healthNum = ColorString(" " + std::to_string(getHealth()) + "/" + std::to_string(getMaxHealth(100)), ddutil::WHITE);
	ColorString blockNum = ColorString("(" + std::to_string(buffer) + ")", ddutil::BLOCK_COLOR);

	ColorString dodgeNum;
	if (dodgeChance > 0)
	{
		dodgeNum += ColorString("(" + std::to_string(dodgeChance) + "% DODGE)", ddutil::TEXT_COLOR);
	}
	return divideChar + name + divideChar + healthBar + healthNum + 
		divideChar + blockNum + divideChar + dodgeNum + divideChar;
}

ColorString Creature::getStatusString()
{
	if (currentStatuses.empty())
	{
		return ColorString("No current statuses", ddutil::TEXT_COLOR);
	}
	else
	{
		ColorString statusList;
		for (auto i : currentStatuses)
		{
			statusList += i.first->getName() + ColorString("(" + std::to_string(i.second) + ")", ddutil::TEXT_COLOR) + 
				ColorString(", ", ddutil::TEXT_COLOR);
		}
		ddutil::trimLength(statusList);
		return statusList;
	}
}

Game* Creature::getGamePtr()
{
	return game;
}

void Creature::addAttackStatus(Status* stat, int amount)
{
	attackStatuses.push_back(std::make_pair(stat, amount));
}

void Creature::applyAttackStatuses(Creature* other)
{
	for (auto s : attackStatuses)
	{
		other->applyStatus(s.first->makeCopy(), s.second);
	}
}

void Creature::increaseBaseBlock(int amount)
{
	baseBlock += amount;
}

void Creature::increasePercentHealBoost(int amount)
{
	percentHealBoost += amount;
}

void Creature::decreasePercentHealBoost(int amount)
{
	percentHealBoost -= amount;
	if (percentHealBoost < -100)
	{
		percentHealBoost = -100;
	}
}

void Creature::setImmunityToStatuses(bool immune)
{
	immuneToStatuses = immune;
}

void Creature::appendCount(int count)
{
	name += "(" + std::to_string(count) + ")";
}

void Creature::appendToName(std::string n)
{
	name = name + " " + n;
}

void Creature::setHealthPercent(int percentOfMaxHp)
{
	health = getMaxHealth(percentOfMaxHp);
}

void Creature::removeNewestMove()
{
	if (moves.empty())
	{
		return;
	}
	delete moves.back();
	moves.pop_back();
}

int Creature::increaseHealth(int amount)
{
	int overheal = 0;
	int startingHealth = health;
	health += static_cast<int>(static_cast<double>(amount) * (1 + (percentHealBoost / 100.0)));
	if (health > maxHealth)
	{
		health = maxHealth;
	}
	int endingHealth = health;

	

	return endingHealth - startingHealth;
}

void Creature::increaseMaxHealth(int amount)
{
	maxHealth += amount;
	health += amount; // health increases by the same amount
}

void Creature::decreaseMaxHealth(int amount)
{
	maxHealth -= amount;
	if (health > maxHealth)
	{
		health = maxHealth;
	}
	if (health <= 0)
	{
		alive = false;
	}
}

ddutil::DamageReport Creature::reduceHealth(int amount, Creature* attacker, bool ignoreBlock)
{
	int startingHealth = health;
	int startingBuffer = buffer;

	int damageBlocked = 0;

	if (attacker != nullptr && hasStatus(StatusID::Thorns))
	{
		for (auto s : currentStatuses)
		{
			if (s.first->getID() == StatusID::Thorns)
			{
				attacker->reduceHealth(s.second, nullptr, false);
				playSound(WavFile("thorns", ddutil::SF_LOOP, ddutil::SF_ASYNC));
			}
		}
	}

	if (hasStatus(StatusID::Invulnerable))
	{
		return ddutil::DamageReport(0, 0);
	}

	if (attacker == nullptr && immuneToStatuses)
	{
		return ddutil::DamageReport(0, 0);
	}

	if (!ignoreBlock && ddutil::random(1, 100) <= getDodgeChance())
	{
		resetDodgeChance();
		playSound(WavFile("Jump", ddutil::SF_LOOP, ddutil::SF_ASYNC));
		return ddutil::DamageReport(0, 0);
	}

	if (hasStatus(StatusID::Vulnerable))
	{
		double mult = 1 + (VulnerableStatus::PERCENT_DAM_INC / 100.0);
		amount = static_cast<int>(static_cast<double>(amount) * mult);
	}
	if (attacker != nullptr && attacker->hasStatus(StatusID::Dragon)) 
	{
		double mult = 1 + (DragonStatus::PERCENT_DAM_INC / 100.0);
		amount = static_cast<int>(static_cast<double>(amount) * mult);
	}
	if (attacker != nullptr && attacker->hasStatus(StatusID::Elemental))
	{
		applyStatus(new ZappedStatus(), ElementalStatus::ZAP_AMOUNT);
		applyStatus(new BurntStatus(), ElementalStatus::BURN_AMOUNT);
		applyStatus(new FrostBurntStatus(), ElementalStatus::FBURN_AMOUNT);
	}

	if (attacker != nullptr) // not a status effect or non-creature damage source
	{
		attacker->applyAttackStatuses(this);
	}
	

	if (ignoreBlock)
	{
		health -= amount;
	}
	else
	{
		buffer -= amount;

		if (buffer <= 0)
		{
			damageBlocked = startingBuffer;

			health += buffer; // will be a negative number representing how much damage was actually taken
		}
		else
		{
			damageBlocked = startingBuffer - buffer;
		}
	}
	

	if (health <= 0)
	{
		health = 0;
		alive = false;
	}

	if (buffer < 0)
	{
		buffer = 0;
	}

	if (health <= getMaxHealth(33))
	{
		playSound(WavFile("lowhealth", false, true));
	}

	return ddutil::DamageReport(damageBlocked, startingHealth - health);
}

void Creature::applyBlock(int amount)
{
	buffer += amount;
}

void Creature::applyStatus(Status* status, int amount)
{
	StatusID newID = status->getID();
	
	// check to see if the creature already has this status
	bool alreadyHas = false;
	if (!status->isUnique()) // if it isnt unique then we need to check if there is already a status it should stack with
	{
		for (auto it = currentStatuses.begin(); it != currentStatuses.end(); ++it)
		{
			if (it->first->getID() == newID)
			{
				alreadyHas = true;
				it->second += amount; // increment the counter of how many applications this status has
				delete status; // since we wont be storing the new status passed through, needs to be deleted
				break;
			}
		}
	}

	if (!alreadyHas) // no copy of this status exists yet, so store the status passed through and set counter at "amount"
	{
		currentStatuses.emplace(status, amount);
	}
}

void Creature::resetBlock()
{
	buffer = baseBlock;
}

int Creature::getBlock()
{
	return buffer;
}

bool Creature::hasStatus(StatusID statusID)
{
	// returns true if the creature has the passed through status affliction
	// returns false if otherwise
	bool hasStatus = false;
	for (auto i : currentStatuses)
	{
		if (i.first->getID() == statusID)
		{
			hasStatus = true;
			break;
		}
	}
	return hasStatus;
}

void Creature::clearAllStatuses()
{
	auto it = currentStatuses.begin();
	while (it != currentStatuses.end())
	{
		delete (*it).first;
		it = currentStatuses.erase(it);
	}
}

void Creature::clearAllStatuses(StatusID id)
{
	auto it = currentStatuses.begin();
	while (it != currentStatuses.end())
	{
		if ((*it).first->getID() == id)
		{
			delete (*it).first;
			it = currentStatuses.erase(it);
		}
		else
		{
			++it;
		}
	}
}

std::vector<ColorString> Creature::decrementAllStatuses()
{
	// stores the descriptions of the status effects so they can be printed to the screen later
	std::vector<ColorString> information; 

	auto it = currentStatuses.begin();
	while (it != currentStatuses.end())
	{
		information.push_back((*it).first->applyEndTurnEffect(this, (*it).second));
		if (!(*it).first->isUnchanging()) // if NOT unchanging, then try to reduce the status amount
		{
			if ((*it).second <= 1 || (*it).first->shouldTotalRemove())
			{
				delete (*it).first;
				it = currentStatuses.erase(it);
			}
			else
			{
				(*it).second--;
				++it;
			}
		}
		else
		{
			++it;
		}
	}

	return information;
}

int Creature::getNumberOfStatuses()
{
	int amount = 0;
	for (auto it = currentStatuses.begin(); it != currentStatuses.end(); ++it)
	{
		if ((*it).first->isUnique())
		{
			amount += 1;
		}
		else
		{
			amount += (*it).second;
		}
	}
	return amount;
}

int Creature::getNumberOfStatuses(StatusID id)
{
	int amount = 0;
	for (auto it = currentStatuses.begin(); it != currentStatuses.end(); ++it)
	{
		if ((*it).first->getID() == id)
		{
			if ((*it).first->isUnique())
			{
				amount += 1;
			}
			else
			{
				amount += (*it).second;
			}
		}
	}
	return amount;
}

void Creature::removeStatus(StatusID statusID)
{
	auto it = currentStatuses.begin();
	while (it != currentStatuses.end())
	{
		if ((*it).first->getID() == statusID)
		{
			delete (*it).first;
			it = currentStatuses.erase(it);
		}
		else
		{
			++it;
		}
	}
}

int Creature::getNumberOfStatusTypes()
{
	return currentStatuses.size();
}

void Creature::increaseDodgeChance(int percentage)
{
	dodgeChance += percentage;
	if (dodgeChance > 100)
	{
		dodgeChance = 100;
	}
}

void Creature::resetDodgeChance()
{
	dodgeChance = baseDodgeChance;
}

int Creature::getDodgeChance()
{
	return dodgeChance;
}

void Creature::increaseBaseDodgeChance(int percentage)
{
	baseDodgeChance += percentage;
}

// returns ColorString() if there are no cards to throw, otherwise returns string to print
ColorString Creature::throwCard(Creature* other)
{
	auto it = currentStatuses.begin();
	while (it != currentStatuses.end())
	{
		if ((*it).first->isThrowable())
		{
			other->applyStatus((*it).first->makeCopy());
			(*it).second--;
			ColorString cardName = (*it).first->getName();
			if ((*it).second <= 0)
			{
				delete (*it).first;
				it = currentStatuses.erase(it);
			}
			return ColorString("The ", ddutil::TEXT_COLOR) + getColorString() + ColorString(" throws their ", ddutil::TEXT_COLOR) +
				cardName + ColorString(" at The ", ddutil::TEXT_COLOR) + other->getColorString();
		}
		else
		{
			++it;
		}
	}
	return ColorString();
}

int Creature::getHealth()
{
	return health;
}

int Creature::getMaxHealth(int percentage = 100)
{
	return (int)(maxHealth * (double)(percentage / 100.0));
}

int Creature::getColor()
{
	return color;
}