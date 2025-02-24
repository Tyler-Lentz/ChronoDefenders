#include "status.h"
#include "utilities.h"
#include "colorstring.h"
#include "creature.h"
#include "player.h"

#include <Windows.h>
Status::Status(StatusID theID, ColorString theName, std::string description, bool uni, bool ablethrow, bool shouldTotalRemove, bool unchange)
{
	id = theID;
	name = theName;
	unique = uni;
	throwable = ablethrow;
	totalRemove = shouldTotalRemove;
	unchanging = unchange;
	this->description = description;
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

std::string Status::getDescription()
{
	return description;
}

ColorString Status::getFullInformation()
{
	return getName() + ColorString(": " + getDescription(), ddutil::TEXT_COLOR);
}

Status* Status::getStatusFromID(StatusID id)
{
	// TODO: if there is a reason a player should get some status that needs to be saved and it is a card status,
	// then will need to fix this saving to actually cary over information about card statuses
	// right now just using a placeholder value of 10 because it will never actually need to load in a card status
	switch (id)
	{
	case StatusID::Bleeding:
		return new BleedingStatus();
	case StatusID::Burnt:
		return new BurntStatus();
	case StatusID::Club:
		return new ClubStatus(10);
	case StatusID::Diamond:
		return new DiamondStatus(10);
	case StatusID::Dodging:
		return nullptr; // never actually used
	case StatusID::Dragon:
		return new DragonStatus();
	case StatusID::Dynamite:
		return new Dynamite();
	case StatusID::Elemental:
		return new ElementalStatus();
	case StatusID::Frostburnt:
		return new FrostBurntStatus();
	case StatusID::Heart:
		return new HeartStatus(10);
	case StatusID::Hexed:
		return new HexedStatus();
	case StatusID::Invulnerable:
		return new InvulnerableStatus();
	case StatusID::Jester:
		return new JesterStatus();
	case StatusID::OffBalance:
		return new OffBalanceStatus();
	case StatusID::Poisoned:
		return new PoisonedStatus();
	case StatusID::Scorched:
		return new ScorchedStatus();
	case StatusID::Spade:
		return new SpadeStatus(10);
	case StatusID::Storm:
		return new StormStatus();
	case StatusID::Strangled:
		return new StrangledStatus();
	case StatusID::Stunned:
		return new StunnedStatus();
	case StatusID::Thorns:
		return new ThornsStatus();
	case StatusID::Vulnerable:
		return new VulnerableStatus();
	case StatusID::Zapped:
		return new ZappedStatus();
	case StatusID::Zen:
		return new ZenStatus();
	case StatusID::Entombed:
		return new EntombedStatus();
	case StatusID::Marked:
		return new MarkedStatus();
	case StatusID::Beserked:
		return new BeserkedStatus();
	case StatusID::TheMagician:
		return new TheMagicianStatus();
	case StatusID::TheHighPriestess:
		return new TheHighPriestessStatus();
	case StatusID::Judgement:
		return new JudgementStatus();
	case StatusID::TheEmporer:
		return new TheEmperorStatus();
	case StatusID::TheEmpress:
		return new TheEmpressStatus();
	case StatusID::TheSun:
		return new TheSunStatus();
	default:
		return nullptr;
	}
}



// Normal Statuses

NormalStatus::NormalStatus(StatusID theID, ColorString name, std::string description)
	:Status(theID, name, description, false, false, false, false)
{
}


OffBalanceStatus::OffBalanceStatus()
	:NormalStatus(StatusID::OffBalance, ColorString("Off Balance", ddutil::TEXT_COLOR), "unused")
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
	:NormalStatus(StatusID::Burnt, ColorString("Burnt", COLOR), "Deals " + std::to_string(BURN_DAMAGE) + " damage per turn")
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
	:NormalStatus(StatusID::Frostburnt, ColorString("Frostburnt", COLOR),
		"Deals damage equal to " + std::to_string(DAMAGE_PROP) +"% of the target's HP every turn")
{
}

Status* FrostBurntStatus::makeCopy()
{
	return new FrostBurntStatus();
}

// reduces health by 6% of current health every turn
ColorString FrostBurntStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	double percentage = (DAMAGE_PROP / 100.0);
	int damage = static_cast<int>(percentage * target->getHealth());
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
	:NormalStatus(StatusID::Zen, ColorString("Zen", COLOR), 
		"Heals "+std::to_string(HP_GAIN)+"HP amount every turn")
{
}

Status* ZenStatus::makeCopy()
{
	return new ZenStatus();
}

ColorString ZenStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	int startHealAmount = HP_GAIN;
	Player* player = dynamic_cast<Player*>(target);
	int actualHealAmount = target->increaseHealth(startHealAmount);

	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() + ColorString(" heals ", ddutil::TEXT_COLOR) +
		ColorString(std::to_string(actualHealAmount) + " health ", ddutil::HEAL_COLOR) +
		ColorString("due to his ", ddutil::TEXT_COLOR) + getName();
}

StormStatus::StormStatus()
	:NormalStatus(StatusID::Storm, ColorString("Storm", COLOR), "Deals damage equal to the target's block every turn")
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
	:NormalStatus(StatusID::Invulnerable, ColorString("Invulnerable", COLOR), "The target cannot take damage while active")
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
	:NormalStatus(StatusID::Vulnerable, ColorString("Vulnerable", COLOR), 
		"Makes the target take " + std::to_string(PERCENT_DAM_INC) +"% more damage")
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
	:NormalStatus(StatusID::Poisoned, ColorString("Poisoned", COLOR),
		"Deals damage equal to the status's stack amount every turn")
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


ZappedStatus::ZappedStatus()
	:NormalStatus(StatusID::Zapped, ColorString("Zapped", COLOR),
		"Deals damage equal to half its total number of status stacks every turn")
{
}

Status* ZappedStatus::makeCopy()
{
	return new ZappedStatus();
}

ColorString ZappedStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	int damage = target->getNumberOfStatuses() / 2;
	ddutil::DamageReport damRep = target->reduceHealth(damage, nullptr, true);

	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
		ColorString(" loses " + std::to_string(damRep.getDamageTaken()) + " health due to being ", ddutil::TEXT_COLOR) +
		getName();
}

// Card Statuses

CardStatus::CardStatus(StatusID theID, ColorString name, std::string description, int num)
	:Status(theID, name, description, true, true, false, false)
{
	number = num;
}

CardStatus* CardStatus::getRandomCard()
{
	int num = ddutil::random(2, 14);
	switch (ddutil::random(1, 4))
	{
	case 1:
		return new DiamondStatus(num);
	case 2:
		return new HeartStatus(num);
	case 3:
		return new ClubStatus(num);
	default:
		return new SpadeStatus(num);
	}
}

SpadeStatus::SpadeStatus(int num)
	:CardStatus(StatusID::Spade, ColorString(ddutil::cardNumToString(num) + " of Spades", ddutil::BLACK_CARD_COLOR),
		"Deals damage equal to the card's value", num)
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
	:CardStatus(StatusID::Heart, ColorString(ddutil::cardNumToString(num) + " of Hearts", ddutil::RED_CARD_COLOR),
		"Heals HP equal to the card's value", num)
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
	:CardStatus(StatusID::Club, ColorString(ddutil::cardNumToString(num) + " of Clubs", ddutil::BLACK_CARD_COLOR),
		"Deals damage equal to the card's value", num)
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
	:CardStatus(StatusID::Diamond, ColorString(ddutil::cardNumToString(num) + " of Diamonds", ddutil::RED_CARD_COLOR),
		"Gives XP equal to the card's value", num)
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

TheMagicianStatus::TheMagicianStatus()
	:CardStatus(
		StatusID::TheMagician,
		ColorString("The Magician", ddutil::LIGHTMAGENTA),
		"Gives " + std::to_string(BULLET_MIN) + "-"+std::to_string(BULLET_MAX) + " bullets to the holder",
		0 // unused
	)
{
}

Status* TheMagicianStatus::makeCopy()
{
	return new TheMagicianStatus();
}

ColorString TheMagicianStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	Gunslinger* gunslingerTarget = dynamic_cast<Gunslinger*>(target);
	if (gunslingerTarget != nullptr)
	{
		int bulletNum = ddutil::random(BULLET_MIN, BULLET_MAX);
		gunslingerTarget->createBullets(bulletNum);
		std::string bulletWord = " bullet";
		if (bulletNum > 1)
		{
			bulletWord += "s";
		}
		return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
			ColorString(" gets " + std::to_string(bulletNum) + bulletWord + " from ", ddutil::TEXT_COLOR) +
			getName();
	}
	else
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() + ColorString(" cannot use ", ddutil::TEXT_COLOR) +
			getName();
	}
}

TheHighPriestessStatus::TheHighPriestessStatus()
	:CardStatus(
		StatusID::TheHighPriestess,
		ColorString("The High Priestess", ddutil::YELLOW),
		"Gives " +std::to_string(VIT) + " vitality to the holder",
		0
	)
{
}

Status* TheHighPriestessStatus::makeCopy()
{
	return new TheHighPriestessStatus();
}

ColorString TheHighPriestessStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	Player* playerSelf = dynamic_cast<Player*>(target);
	if (playerSelf != nullptr)
	{
		playerSelf->addVitality(VIT);
		return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
			ColorString(" gains " + std::to_string(VIT) + " vitality ", ddutil::VITALITY_COLOR) +
			ColorString("from ", ddutil::TEXT_COLOR) + getName();
	}
	else
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() + ColorString(" cannot gain vitality", ddutil::TEXT_COLOR);
	}
}

JudgementStatus::JudgementStatus()
	:CardStatus(
		StatusID::Judgement,
		ColorString("Judgement", COLOR),
		"Deals damage equal to " +std::to_string(DMG_PERCENT)+"% of the target's max HP",
		0
	)
{
}

Status* JudgementStatus::makeCopy()
{
	return new JudgementStatus();
}

ColorString JudgementStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	int damage = target->getMaxHealth(DMG_PERCENT);
	auto damRep = target->reduceHealth(damage, nullptr, true);
	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() + ColorString(" takes ", ddutil::TEXT_COLOR) +
		ddutil::genericDamageAlert(damRep) + ColorString(" from ", ddutil::TEXT_COLOR) + getName();
}

UniqueStatus::UniqueStatus(StatusID theID, ColorString name, std::string description)
	:Status(theID, name, description, true, false, false, false)
{
}

Dynamite::Dynamite()
	:UniqueStatus(StatusID::Dynamite, ColorString("Dynamite", COLOR), 
		"Once the status stack reaches 0, the target takes " + std::to_string(DAMAGE) + " damage")
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
		playSound(WavFile("explosion", false, false));
		return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() + ColorString(" takes ", ddutil::TEXT_COLOR) +
			ddutil::genericDamageAlert(damage) + ColorString(" from the ", ddutil::TEXT_COLOR) + getName() +
			ColorString(" explosion", ddutil::TEXT_COLOR);
	}
	else
	{
		playSound(WavFile("bombfuse", true, true));
		Sleep(2000);
		stopSound(SoundType::WAV);
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() + ColorString(" attached to The ", ddutil::TEXT_COLOR) +
			target->getColorString() + ColorString(" is one turn closer to detonating", ddutil::TEXT_COLOR);
	}
}

TotalRemoveStatus::TotalRemoveStatus(StatusID id, ColorString name, std::string desc)
	:Status(id, name, desc, false, false, true, false)
{

}

BleedingStatus::BleedingStatus()
	:TotalRemoveStatus(StatusID::Bleeding, ColorString("Bleeding", COLOR),
		"Deals damage equal to the status's stack amount, then is removed completely")
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




UnchangingStatus::UnchangingStatus(StatusID id, ColorString name, std::string desc)
	:Status(id, name, desc, false, false, false, true)
{
}

ThornsStatus::ThornsStatus()
	:UnchangingStatus(StatusID::Thorns, ColorString("Thorned", COLOR),
		"Every time the target is attacked, the attacker takes damage equal to the status stack")
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
	:NormalStatus(StatusID::Hexed, ColorString("Hexed", COLOR), 
		"The target cannot attack while active and loses " + std::to_string(VITALITY_LOSS) + " vitality each turn")
{
}

Status* HexedStatus::makeCopy()
{
	return new HexedStatus();
}

ColorString HexedStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	Player* player = dynamic_cast<Player*>(target);
	if (player != nullptr)
	{
		int vitLoss = player->stealVitality(HexedStatus::VITALITY_LOSS);
		return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
			ColorString(" loses ", ddutil::TEXT_COLOR) + ColorString(std::to_string(vitLoss) + " vitality ", ddutil::VITALITY_COLOR) +
			ColorString("due to being ", ddutil::TEXT_COLOR) + getName();
	}
	else
	{
		return ColorString();
	}
}

StrangledStatus::StrangledStatus()
	:NormalStatus(StatusID::Strangled, ColorString("Strangled", ddutil::WHITE),
		"The target cannot attack while active and loses " + std::to_string(STRANGLE_DAMAGE) + " health per turn")
{
}

Status* StrangledStatus::makeCopy()
{
	return new StrangledStatus();	
}

ColorString StrangledStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{	
	ddutil::DamageReport dam= target->reduceHealth(STRANGLE_DAMAGE, nullptr, true);

	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
		ColorString(" loses " + std::to_string(dam.getDamageTaken()) + " health due to being ", ddutil::TEXT_COLOR) +
		getName();
}

StunnedStatus::StunnedStatus()
	:NormalStatus(StatusID::Stunned, ColorString("Stunned", COLOR), "The target cannot attack while active")
{
}

Status* StunnedStatus::makeCopy()
{
	return new StunnedStatus();
}

ColorString StunnedStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	return ColorString();
}

DragonStatus::DragonStatus()
	:NormalStatus(StatusID::Dragon, ColorString("Dragon Form", COLOR), 
		"Makes every attack deal " + std::to_string(PERCENT_DAM_INC) + "% more damage ")
{
}

Status* DragonStatus::makeCopy()
{
	return new DragonStatus();
}

ColorString DragonStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	// reset the picture back to normal if the status is over
	if (stackAmount == 1)
	{
		target->resetPicture();
	}
	return ColorString();
}

JesterStatus::JesterStatus()
	:NormalStatus(StatusID::Jester, ColorString("Jester Form", COLOR),
		"The effects of card draw moves are doubled")
{
}

Status* JesterStatus::makeCopy()
{
	return new JesterStatus();
}

ColorString JesterStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	if (stackAmount == 1)
	{
		target->resetPicture();
	}

	return ColorString();
}

ElementalStatus::ElementalStatus()
	:NormalStatus(StatusID::Elemental, ColorString("Elemental Form", COLOR),
		"Makes every attack apply " + std::to_string(ZAP_AMOUNT) + " Zapped, " + std::to_string(BURN_AMOUNT) + " Burn," +
		" and " + std::to_string(FBURN_AMOUNT) + " Frostburn")
{
}

Status* ElementalStatus::makeCopy()
{
	return new ElementalStatus();
}

ColorString ElementalStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	if (stackAmount == 1)
	{
		target->resetPicture();
	}
	return ColorString();
}

ScorchedStatus::ScorchedStatus()
	:NormalStatus(StatusID::Scorched, ColorString("Scorched", COLOR),
		"Deals damage equal to the status stack times the number of burns the target has")
{
}

Status* ScorchedStatus::makeCopy()
{
	return new ScorchedStatus();
}

ColorString ScorchedStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	int damage = target->getNumberOfStatuses(StatusID::Burnt) * stackAmount;

	ddutil::DamageReport dam = target->reduceHealth(damage, nullptr, true);

	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
		ColorString(" loses " + std::to_string(dam.getDamageTaken()) + " health due to being ", ddutil::TEXT_COLOR) +
		getName();
}

EntombedStatus::EntombedStatus()
	:UnchangingStatus(
		StatusID::Entombed,
		ColorString("Entombed", COLOR),
		"Starts by dealing 1 damage. The damage doubles each turn."
	)
{
	damage = 1;
}

Status* EntombedStatus::makeCopy()
{
	return new EntombedStatus();
}

ColorString EntombedStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	ddutil::DamageReport damRep = target->reduceHealth(damage, nullptr, true);
	
	damage *= 2;
	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() +
		ColorString(" takes " + std::to_string(damRep.getDamageTaken()) + " damage", ddutil::DAMAGE_COLOR) +
		ColorString(" from being ", ddutil::TEXT_COLOR) + ColorString("Entombed", COLOR);
}

MarkedStatus::MarkedStatus()
	:UnchangingStatus(
		StatusID::Marked,
		ColorString("Marked", COLOR),
		"When a marked creature is attacked, the attacker gains block equal to the status stack"
	)
{
}

Status* MarkedStatus::makeCopy()
{
	return new MarkedStatus();
}

ColorString MarkedStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	return ColorString();
}

BeserkedStatus::BeserkedStatus()
	:NormalStatus(
		StatusID::Beserked,
		ColorString("Beserked", COLOR),
		"While active, increases damage by "+std::to_string(PERCENT_DAM_INC)+"%"
	)
{
}

Status* BeserkedStatus::makeCopy()
{
	return new BeserkedStatus();
}

TheEmpressStatus::TheEmpressStatus()
	:CardStatus(
		StatusID::TheEmpress,
		ColorString("The Empress", FrostBurntStatus::COLOR),
		"Applies " +std::to_string(FBURN_AMOUNT)+" Frostburnt",
		0
	)
{
}

Status* TheEmpressStatus::makeCopy()
{
	return new TheEmpressStatus();
}

ColorString TheEmpressStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	target->applyStatus(new FrostBurntStatus(), FBURN_AMOUNT);
	return getName() + ColorString(" applies ", ddutil::TEXT_COLOR) + ColorString("Frostburnt (" + std::to_string(FBURN_AMOUNT) + ") ", FrostBurntStatus::COLOR) +
		ColorString("to The ", ddutil::TEXT_COLOR) + target->getColorString();
}

TheEmperorStatus::TheEmperorStatus()
	:CardStatus(
		StatusID::TheEmporer,
		ColorString("The Emperor", BurntStatus::COLOR),
		"Applies " +std::to_string(BURN_AMOUNT)+" Burnt",
		0
	)
{
}

Status* TheEmperorStatus::makeCopy()
{
	return new TheEmperorStatus();
}

ColorString TheEmperorStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	target->applyStatus(new BurntStatus(), BURN_AMOUNT);
	return getName() + ColorString(" applies ", ddutil::TEXT_COLOR) + ColorString("Burnt (" + std::to_string(BURN_AMOUNT) + ") ", BurntStatus::COLOR) +
		ColorString("to The ", ddutil::TEXT_COLOR) + target->getColorString();
}

TheSunStatus::TheSunStatus()
	:CardStatus(
		StatusID::TheSun,
		ColorString("The Sun", ScorchedStatus::COLOR),
		"Applies " +std::to_string(SCORCHED_AMOUNT)+" Scorched",
		0
	)
{
}

Status* TheSunStatus::makeCopy()
{
	return new TheSunStatus();
}

ColorString TheSunStatus::applyEndTurnEffect(Creature* target, int stackAmount)
{
	target->applyStatus(new ScorchedStatus(), SCORCHED_AMOUNT);
	return getName() + ColorString(" Scorches ", ScorchedStatus::COLOR) + target->getColorString();
}
