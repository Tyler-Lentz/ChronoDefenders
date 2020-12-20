#include "sorcerer_moves.h"
#include "player.h"
#include "creature.h"
#include "colorstring.h"
#include "game.h"
#include "move.h"

#include <string>

// Base


SorcererMoves::MakeBarrierMove::MakeBarrierMove(int barrierAmount, int cost, std::string name, Strength str)
	:Move("Gives " + std::to_string(barrierAmount) + " block to the target", name, cost, str, true, 
		WavFile("magicspell2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
	blockAmount = barrierAmount;
}

ColorString SorcererMoves::MakeBarrierMove::doAction(Creature* self, Creature* other)
{
	other->applyBlock(blockAmount);
	return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() +
		ColorString(" will block ", ddutil::TEXT_COLOR) + ddutil::genericBlockAlert(blockAmount);
}

SorcererMoves::SummonMove::SummonMove(Player* minion, int cost, std::string name, Strength str, WavFile theSound)
	:Move("Summons a " + minion->getColorString().getString() + " for the duration of battle", name, cost, str, false, theSound)
{
	summon = minion;
}

SorcererMoves::SummonMove::~SummonMove()
{
	if (summon != nullptr)
	{
		delete summon;
	}
}

ColorString SorcererMoves::SummonMove::doAction(Creature* self, Creature* other)
{
	Game* gamePtr = self->getGamePtr();

	Player* minion = dynamic_cast<Player*>(summon->makeCopy());

	if (minion == nullptr)
	{
		return ColorString("ERROR: this move is set up incorrectly", ddutil::TEXT_COLOR);
	}

	ColorString minionName = minion->getColorString();

	gamePtr->addPlayer(minion);

	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" summons a ", ddutil::TEXT_COLOR) + minionName;
}


// Weak

SorcererMoves::EnergyStrike::EnergyStrike()
	:SimpleAttackMove(DAMAGE, false, COST, "Energy Strike", Strength::Weak, WavFile("magicattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::MagicBarrier::MagicBarrier()
	:MakeBarrierMove(BLOCK_AMOUNT, COST, "Magic Barrier", Strength::Weak)
{}

SorcererMoves::Heal::Heal()
	:SimpleHealMove(HEAL_AMOUNT, true, 3, "Heal", Strength::Weak)
{
}

// Moderate

SorcererMoves::ElectricShock::ElectricShock()
	:StatusAttackMove(DAMAGE, new ZappedStatus(), ZAPPED_LENGTH, COST, "Electric Shock", Strength::Moderate, WavFile("electricattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::MirrorImage::MirrorImage()
	:SelfBlockMove(BLOCK, COST, "Mirror Image", Strength::Powerful, WavFile("magicspell2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::Levitate::Levitate()
	:SelfBlockMove(BLOCK, COST, "Levitate", Strength::Moderate, WavFile("magicspell2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::Fireball::Fireball()
	:StatusAttackMove(DAMAGE, new BurntStatus(), BURN_TIMES, COST, "Fireball", Strength::Moderate, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{}

SorcererMoves::IceOrb::IceOrb()
	: StatusAttackMove(DAMAGE, new FrostBurntStatus(), FBURN_TIMES, COST, "Ice Orb", Strength::Moderate, WavFile("magicattack2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{}

SorcererMoves::BlinkStrike::BlinkStrike()
	:SimpleAttackMove(DAMAGE, false, COST, "Blink Strike", Strength::Moderate, WavFile("attack2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::Recover::Recover()
	:SimpleHealMove(HEAL_AMOUNT, false, COST, "Recover", Strength::Moderate)
{
}

SorcererMoves::SummonFireImp::SummonFireImp(Game* game)
	:SummonMove(new FireImp(game), COST, "Summon Fire Imp", Strength::Moderate, WavFile("magicspell3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::SummonIceImp::SummonIceImp(Game* game)
	:SummonMove(new IceImp(game), COST, "Summon Ice Imp", Strength::Moderate, WavFile("magicspell3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::EnergyBarrier::EnergyBarrier()
	:MakeBarrierMove(BLOCK_AMOUNT, COST, "Energy Barrier", Strength::Moderate)
{}

SorcererMoves::CleansingTouch::CleansingTouch()
	:SimpleHealMove(HEAL_AMOUNT, true, COST, "Cleansing Touch", Strength::Moderate)
{
}

SorcererMoves::EnergySword::EnergySword()
	:SimpleAttackMove(DAMAGE, true, COST, "Energy Sword", Strength::Moderate, WavFile("energyattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::PlasmaStrike::PlasmaStrike()
	:SimpleAttackMove(DAMAGE, false, COST, "Plasma Strike", Strength::Moderate, WavFile("energyattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::FairySummon::FairySummon(Game* game)
	:SummonMove(new Fairy(game), COST, "Summon Fairy", Strength::Powerful, WavFile("magicspell3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::SummonFireDragon::SummonFireDragon(Game* game)
	:SummonMove(new FireDragon(game), COST, "Summon Fire Dragon", Strength::Powerful, WavFile("magicspell3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{

}

SorcererMoves::SummonIceDragon::SummonIceDragon(Game* game)
	:SummonMove(new IceDragon(game), COST, "Summon Ice Dragon", Strength::Powerful, WavFile("magicspell3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

// Powerful

SorcererMoves::Blizzard::Blizzard()
	:Move("Deals " + std::to_string(BASE_DAMAGE) + " damage (x"+std::to_string(MULTIPLIER)+" if enemy is frostburnt)", 
		"Blizzard", COST, Strength::Powerful, true, WavFile("freeze", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SorcererMoves::Blizzard::doAction(Creature* self, Creature* other)
{
	int damageAmount = BASE_DAMAGE;
	if (other->hasStatus(StatusID::Frostburnt))
	{
		damageAmount *= MULTIPLIER;
	}

	ddutil::DamageReport damage = other->reduceHealth(damageAmount, self);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" deals ", ddutil::TEXT_COLOR) + 
		ddutil::genericDamageAlert(damage) + ColorString(" to the ", ddutil::TEXT_COLOR) + other->getColorString();
}

SorcererMoves::Cataclysm::Cataclysm()
	:Move("Deals " + std::to_string(BASE_DAMAGE) + " damage (x" + std::to_string(MULTIPLIER) + " if enemy is burnt)", 
		"Cataclysm", COST, Strength::Powerful, true, WavFile("magicattack2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SorcererMoves::Cataclysm::doAction(Creature* self, Creature* other)
{
	int damageAmount = 10;
	if (other->hasStatus(StatusID::Burnt))
	{
		damageAmount *= 2;
	}

	ddutil::DamageReport damage = other->reduceHealth(damageAmount, self);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" deals ", ddutil::TEXT_COLOR) +
		ddutil::genericDamageAlert(damage) + ColorString(" to the ", ddutil::TEXT_COLOR) + other->getColorString();
}

SorcererMoves::PlasmaSword::PlasmaSword()
	:SimpleAttackMove(DAMAGE, true, COST, "Plasma Sword", Strength::Powerful, WavFile("energyattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::EtherealBarrier::EtherealBarrier()
	:MakeBarrierMove(BLOCK_AMOUNT, COST, "Ethereal Barrier", Strength::Powerful)
{
}

SorcererMoves::StarStrike::StarStrike()
	:SimpleAttackMove(DAMAGE, false, COST, "Star Strike", Strength::Powerful, WavFile("starattack", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::Combust::Combust()
	:Move("Deals "+std::to_string(DAM_PER_BURN)+" damage per burn on the target and removes all burn", "Combust",
		COST, Strength::Powerful, true, WavFile("explosion", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SorcererMoves::Combust::doAction(Creature* self, Creature* other)
{
	int damage = other->getNumberOfStatuses(StatusID::Burnt) * DAM_PER_BURN;
	other->clearAllStatuses(StatusID::Burnt);
	ddutil::DamageReport damRep = other->reduceHealth(damage, self, false);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" Combusts the ", ddutil::TEXT_COLOR) + other->getColorString() +
		ColorString(" for ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep);
}

SorcererMoves::Freeze::Freeze()
	:Move("Deals " + std::to_string(DAM_PER_FBURN) + " damage per frostburn on the target and removes all frostburn", "Freeze",
		COST, Strength::Powerful, true, WavFile("freeze", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SorcererMoves::Freeze::doAction(Creature* self, Creature* other)
{
	int damage = other->getNumberOfStatuses(StatusID::Frostburnt) * DAM_PER_FBURN;
	other->clearAllStatuses(StatusID::Frostburnt);
	ddutil::DamageReport damRep = other->reduceHealth(damage, self, false);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" Freezes the ", ddutil::TEXT_COLOR) + other->getColorString() +
		ColorString(" for ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep);
}

SorcererMoves::LightningStrike::LightningStrike()
	:StatusAttackMove(BASE_DAMAGE, new ZappedStatus(), ZAPPED_LENGTH, COST, "Lightning Strike", Strength::Powerful, WavFile("lightning", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}


// Mythical

SorcererMoves::CleansingAura::CleansingAura()
	:Move("Heals "+std::to_string(BASE_HEAL)+" health, plus "+std::to_string(BONUS_HEAL)+" health per status effect",
		"Cleansing Aura", COST, Strength::Mythical, true, WavFile("heal", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SorcererMoves::CleansingAura::doAction(Creature* self, Creature* other)
{
	int healAmount = BASE_HEAL + (BONUS_HEAL * other->getNumberOfStatuses());

	int actualHealAmount = other->increaseHealth(healAmount);

	return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString(" heals ", ddutil::TEXT_COLOR) + 
		ColorString(std::to_string(actualHealAmount) + " health", ddutil::HEAL_COLOR);
}

SorcererMoves::EnergyAura::EnergyAura()
	:Move("Gives 2 additional vitality per turn this battle", "Energy Aura", COST, Strength::Mythical, true, WavFile("magicspell3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SorcererMoves::EnergyAura::doAction(Creature* self, Creature* other)
{
	Player* player = dynamic_cast<Player*>(other);
	if (player == nullptr)
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() +
			ColorString(" cannot gain vitality", ddutil::TEXT_COLOR);
	}

	player->adjustVitalityGainTemp(VITALITY_GAIN);

	return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + 
		ColorString(" gains 2 additional vitality per turn", ddutil::TEXT_COLOR);
}

SorcererMoves::EtherealBlade::EtherealBlade()
	:SimpleAttackMove(DAMAGE, true, 4, "Ethereal Blade", Strength::Mythical, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::SpiritCall::SpiritCall(Game* game)
	:SummonMove(new SpiritKnight(game), COST, "Spirit Call", Strength::Mythical, WavFile("spiritcall", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::Supernova::Supernova()
	:Move("Deals "+std::to_string(DAMAGE)+" damage to EVERYONE except the caster",
		"Supernova", COST, Strength::Mythical, false, WavFile("supernova", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SorcererMoves::Supernova::doAction(Creature* self, Creature* other)
{
	Game* gamePtr = self->getGamePtr();

	ddutil::DamageReport enemyReport = other->reduceHealth(DAMAGE, self);
	
	// matching indices of these vectors correspond with each other
	std::vector<ddutil::DamageReport> playerReports;
	std::vector<ColorString> playerNames;
	for (Player* p : gamePtr->getPlayerParty())
	{
		if (p != self)
		{
			playerReports.push_back(p->reduceHealth(DAMAGE, self));
			playerNames.push_back(p->getColorString());
		}
	}

	ColorString result;
	result += other->getColorString() + ColorString(" ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(enemyReport) + ColorString(" ", ddutil::TEXT_COLOR);
	for (unsigned int i = 0; i < playerReports.size() && i < playerNames.size(); i++)
	{
		result += playerNames[i] + ColorString(" ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(playerReports[i]) + ColorString(" ", ddutil::TEXT_COLOR);
	}

	ddutil::trimLength(result);


	return result;
}

SorcererMoves::CastInvulnerability::CastInvulnerability()
	:Move("Makes the target invulnerable for the turn", "Cast Invulnerability", COST, Strength::Mythical, true, WavFile("ghostinvulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SorcererMoves::CastInvulnerability::doAction(Creature* self, Creature* other)
{
	other->applyStatus(new InvulnerableStatus(), 1);
	return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString(" will be invulnerable this turn", ddutil::TEXT_COLOR);
}

SorcererMoves::ChainLightning::ChainLightning()
	:Move(
		"Doubles the amount of Zapped on an enemy",
		"Chain Lightning", COST, Strength::Mythical, true, WavFile("electricattack2", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SorcererMoves::ChainLightning::doAction(Creature* self, Creature* other)
{
	int zappedAmount = other->getNumberOfStatuses(StatusID::Zapped);
	other->applyStatus(new ZappedStatus(), zappedAmount);

	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" doubles the ", ddutil::TEXT_COLOR) +
		other->getColorString() + ColorString("'s ", ddutil::TEXT_COLOR) +
		ColorString("Zapped ", ZappedStatus::COLOR) + ColorString("to ", ddutil::TEXT_COLOR) +
		ColorString(std::to_string(zappedAmount * 2), ZappedStatus::COLOR);
}

SorcererMoves::ElementalForm::ElementalForm()
	:Move("Makes every attack apply Zapped " + std::to_string(ElementalStatus::ZAP_AMOUNT) +  " for " + std::to_string(DURATION) + " turns",
		"Elemental Form", COST, Strength::Mythical,
		false, WavFile("lightning", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SorcererMoves::ElementalForm::doAction(Creature* self, Creature* other)
{
	ElementalStatus* newStatus = new ElementalStatus();
	ColorString statusName = newStatus->getName();
	self->applyStatus(newStatus, DURATION);
	self->changePicture(Art::getElementalSorceress());

	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" enters ", ddutil::TEXT_COLOR) + statusName +
		ColorString(" for " + std::to_string(DURATION) + " turns", ddutil::TEXT_COLOR);
}
