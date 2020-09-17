#include "status.h"
#include "utilities.h"
#include "colorstring.h"
#include "creature.h"
#include "player.h"
Status::Status(StatusID theID, ColorString theName, bool uni, bool ablethrow, bool shouldTotalRemove, bool unchange)
{
	id = theID;
	name = theName;
	unique = uni;
	throwable = ablethrow;
	totalRemove = shouldTotalRemove;
	unchanging = unchange;
}

bool Status::isThrowable()
{
	return throwable;
}

bool Status::isUnique()
{
	return unique;
}

bool Status::shouldTotalRemove()
{
	return totalRemove;
}

bool Status::isUnchanging()
{
	return unchanging;
}

StatusID Status::getID()
{
	return id;
}

ColorString Status::getName()
{
	return name;
}



// Normal Statuses

NormalStatus::NormalStatus(StatusID theID, ColorString name)
	:Status(theID, name, false, false, false, false)
{
}


OffBalanceStatus::OffBalanceStatus()
	:NormalStatus(StatusID::OffBalance, ColorString("Off Balance", ddutil::TEXT_COLOR))
{
}

Status* OffBalanceStatus::makeCopy()
{
	return new OffBalanceStatus();
}

ColorString OffBalanceStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	// does nothing
	return ColorString();
}

// Burnt

BurntStatus::BurntStatus()
	:NormalStatus(StatusID::Burnt, ColorString("Burnt", COLOR))
{
}

Status* BurntStatus::makeCopy()
{
	return new BurntStatus();
}

// reduces health by 2 every turn
ColorString BurntStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	ddutil::DamageReport dam= target->reduceHealth(BURN_DAMAGE, nullptr, true);

	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
		ColorString(" loses " + std::to_string(dam.getDamageTaken()) + " health due to being ", ddutil::TEXT_COLOR) +
		getName();
}

// FrostBurnt 

FrostBurntStatus::FrostBurntStatus()
	:NormalStatus(StatusID::Frostburnt, ColorString("Frostburnt", COLOR))
{
}

Status* FrostBurntStatus::makeCopy()
{
	return new FrostBurntStatus();
}

// reduces health by 6% of current health every turn
ColorString FrostBurntStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	int damage = static_cast<int>(0.06 * target->getHealth());
	if (damage < 1)
	{
		damage = 1;
	}

	ddutil::DamageReport dam = target->reduceHealth(damage, nullptr, true);

	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
		ColorString(" loses " + std::to_string(dam.getDamageTaken()) + " health due to being ", ddutil::TEXT_COLOR) +
		getName();
}

ZenStatus::ZenStatus()
	:NormalStatus(StatusID::Zen, ColorString("Zen", COLOR))
{
}

Status* ZenStatus::makeCopy()
{
	return new ZenStatus();
}

ColorString ZenStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	int actualHealAmount = target->increaseHealth(stackAmount);

	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() + ColorString(" heals ", ddutil::TEXT_COLOR) +
		ColorString(std::to_string(actualHealAmount) + " health ", ddutil::HEAL_COLOR) +
		ColorString("due to his ", ddutil::TEXT_COLOR) + getName();
}

StormStatus::StormStatus()
	:NormalStatus(StatusID::Storm, ColorString("Storm", COLOR))
{
}

Status* StormStatus::makeCopy()
{
	return new StormStatus();
}

ColorString StormStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	int damage = target->getBlock();

	ddutil::DamageReport damRep = target->reduceHealth(damage, nullptr, true);
	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
		ColorString(" loses " + std::to_string(damRep.getDamageTaken()) + " health due to the ", ddutil::TEXT_COLOR) +
		getName();
}


InvulnerableStatus::InvulnerableStatus()
	:NormalStatus(StatusID::Invulnerable, ColorString("Invulnerable", COLOR))
{
}

Status* InvulnerableStatus::makeCopy()
{
	return new InvulnerableStatus();
}

ColorString InvulnerableStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	// does nothing at the end of turn, is used as a marker in reduceHealth
	return ColorString();
}


VulnerableStatus::VulnerableStatus()
	:NormalStatus(StatusID::Vulnerable, ColorString("Vulnerable", COLOR))
{
}

Status* VulnerableStatus::makeCopy()
{
	return new VulnerableStatus();
}

ColorString VulnerableStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	return ColorString(); // does nothing. acts as a marker for reduceHealth, makes target take double damage
}

PoisonedStatus::PoisonedStatus()
	:NormalStatus(StatusID::Poisoned, ColorString("Poisoned", COLOR))
{
}

Status* PoisonedStatus::makeCopy()
{
	return new PoisonedStatus();
}

ColorString PoisonedStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	ddutil::DamageReport damRep = target->reduceHealth(stackAmount, nullptr, true);
	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
		ColorString(" loses ", ddutil::TEXT_COLOR) + ColorString(std::to_string(damRep.getDamageTaken()) + " health", ddutil::DAMAGE_COLOR) +
		ColorString(" from the ", ddutil::TEXT_COLOR) + ColorString("Poison", COLOR);
}

// Card Statuses

CardStatus::CardStatus(StatusID theID, ColorString name, int num)
	:Status(theID, name, true, true, false, false)
{
	number = num;
}

SpadeStatus::SpadeStatus(int num)
	:CardStatus(StatusID::Spade, ColorString(ddutil::cardNumToString(num) + " of Spades", ddutil::BLACK_CARD_COLOR), num)
{
}

Status* SpadeStatus::makeCopy()
{
	return new SpadeStatus(number);
}

ColorString SpadeStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	ddutil::DamageReport damRep = target->reduceHealth(number, nullptr);

	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
		ColorString(" loses " + std::to_string(damRep.getDamageTaken()) + " health due to the ", ddutil::TEXT_COLOR) +
		getName();
}

HeartStatus::HeartStatus(int num)
	:CardStatus(StatusID::Heart, ColorString(ddutil::cardNumToString(num) + " of Hearts", ddutil::RED_CARD_COLOR), num)
{
}

Status* HeartStatus::makeCopy()
{
	return new HeartStatus(number);
}

ColorString HeartStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	int actualHealAmount = target->increaseHealth(number);

	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() + ColorString(" heals ", ddutil::TEXT_COLOR) + 
		ColorString(std::to_string(actualHealAmount) + " health ", ddutil::HEAL_COLOR) +
		ColorString("due to the ", ddutil::TEXT_COLOR) + getName();
}

ClubStatus::ClubStatus(int num)
	:CardStatus(StatusID::Club, ColorString(ddutil::cardNumToString(num) + " of Clubs", ddutil::BLACK_CARD_COLOR), num)
{
}

Status* ClubStatus::makeCopy()
{
	return new ClubStatus(number);
}

ColorString ClubStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	ddutil::DamageReport damRep = target->reduceHealth(number, nullptr);

	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
		ColorString(" loses " + std::to_string(damRep.getDamageTaken()) + " health due to the ", ddutil::TEXT_COLOR) +
		getName();
}

DiamondStatus::DiamondStatus(int num)
	:CardStatus(StatusID::Diamond, ColorString(ddutil::cardNumToString(num) + " of Diamonds", ddutil::RED_CARD_COLOR), num)
{
}

Status* DiamondStatus::makeCopy()
{
	return new SpadeStatus(number);
}

ColorString DiamondStatus::applyEndTurnEffect(Creature* target, int stackAmounts)
{
	Player* player = dynamic_cast<Player*>(target);
	if (player == nullptr) // a non player has the status
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() + ColorString(" has no effect on the ", ddutil::TEXT_COLOR) +
			target->getColorString();
	}
	else
	{
		player->gainExperience(number);
		return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
			ColorString(" gains " + std::to_string(number) + " experience due to the ", ddutil::TEXT_COLOR) +
			getName();
	}
}

UniqueStatus::UniqueStatus(StatusID theID, ColorString name)
	:Status(theID, name, true, false, false, false)
{
}

Dynamite::Dynamite()
	:UniqueStatus(StatusID::Dynamite, ColorString("Dynamite", COLOR))
{
}

Status* Dynamite::makeCopy()
{
	return new Dynamite();
}

ColorString Dynamite::applyEndTurnEffect(Creature* target, int stackAmount)
{

	if (stackAmount == 1)
	{
		ddutil::DamageReport damage = target->reduceHealth(DAMAGE, nullptr);
		return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() + ColorString(" takes ", ddutil::TEXT_COLOR) +
			ddutil::genericDamageAlert(damage) + ColorString(" from the ", ddutil::TEXT_COLOR) + getName() +
			ColorString(" explosion", ddutil::TEXT_COLOR);
	}
	else
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() + ColorString(" attached to The ", ddutil::TEXT_COLOR) +
			target->getColorString() + ColorString(" is one turn closer to detonating", ddutil::TEXT_COLOR);
	}
}

TotalRemoveStatus::TotalRemoveStatus(StatusID id, ColorString name)
	:Status(id, name, false, false, true, false)
{

}

BleedingStatus::BleedingStatus()
	:TotalRemoveStatus(StatusID::Bleeding, ColorString("Bleeding", COLOR))
{
}

Status* BleedingStatus::makeCopy()
{
	return new BleedingStatus();
}

ColorString BleedingStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	ddutil::DamageReport damRep = target->reduceHealth(stackAmount, nullptr, true);
	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
		ColorString(" loses ", ddutil::TEXT_COLOR) + ColorString(std::to_string(damRep.getDamageTaken()) + " health", ddutil::DAMAGE_COLOR) +
		ColorString(" from the ", ddutil::TEXT_COLOR) + ColorString("Bleed", COLOR);
}

UnchangingStatus::UnchangingStatus(StatusID id, ColorString name)
	:Status(id, name, false, false, false, true)
{
}

ThornsStatus::ThornsStatus()
	:UnchangingStatus(StatusID::Thorns, ColorString("Thorns", COLOR))
{
}

Status* ThornsStatus::makeCopy()
{
	return new ThornsStatus();
}

ColorString ThornsStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	// acts as flag for reduceHealth()
	return ColorString();
}

HexedStatus::HexedStatus()
	:NormalStatus(StatusID::Hexed, ColorString("Hexed", COLOR))
{
}

Status* HexedStatus::makeCopy()
{
	return new HexedStatus();
}

ColorString HexedStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	return ColorString();
}
