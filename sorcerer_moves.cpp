#include "sorcerer_moves.h"
#include "player.h"
#include "creature.h"
#include "colorstring.h"
#include "game.h"
#include "move.h"

#include <string>

// Base


SorcererMoves::MakeBarrierMove::MakeBarrierMove(MoveId id, int barrierAmount, int cost, std::string name, Strength str)
	:Move(id, "Gives " + std::to_string(barrierAmount) + " block to the target", name, cost, str, true, 
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

SorcererMoves::SummonMove::SummonMove(MoveId id, Player* minion, int cost, std::string name, Strength str, WavFile theSound)
	:Move(id, "Summons a " + minion->getColorString().getString() + " for one battle, and uses "+std::to_string(AURA)+" Aura", name, cost, str, false, theSound)
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
	Sorcerer* playerSelf = dynamic_cast<Sorcerer*>(self);
	if (playerSelf != nullptr && !playerSelf->useAura(AURA))
	{
		return ColorString(ddutil::NOT_ENOUGH_AURA, ddutil::TEXT_COLOR);
	}

	Player* minion = dynamic_cast<Player*>(summon->makeCopy());

	if (minion == nullptr)
	{
		return ColorString("ERROR: this move is set up incorrectly", ddutil::TEXT_COLOR);
	}

	ColorString minionName = minion->getColorString();

	gamePtr->addPlayer(minion);

	if (playerSelf != nullptr && playerSelf->hasArtifact(ArtifactID::SummonersOrb))
	{
		self->applyBlock(SummonersOrb::BLOCK);
	}

	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" summons a ", ddutil::TEXT_COLOR) + minionName;
}

SorcererMoves::AuraDamageMove::AuraDamageMove(MoveId id, int damPerAura, int cost, std::string name, Strength str, WavFile theSound)
	:Move(
		id, 
		"Deals "+std::to_string(damPerAura) + " damage per Aura", 
		name, cost, str, true, theSound
	)
{
	this->damPerAura = damPerAura;
}

ColorString SorcererMoves::AuraDamageMove::doAction(Creature* self, Creature* other)
{
	Sorcerer* sorcSelf = dynamic_cast<Sorcerer*>(self);
	if (sorcSelf != nullptr)
	{
		int damage = sorcSelf->getAura() * damPerAura;
		SimpleAttackMove tempMove(MoveId::TempMoveId, damage, false, 0, "temp", Strength::Moderate, sound);
		return tempMove.doAction(self, other);
	}
	else
	{
		return ColorString(ddutil::CANT_USE_MOVE, ddutil::RED);
	}
}

// Weak

SorcererMoves::EnergyStrike::EnergyStrike()
	:SimpleAttackMove(MoveId::SorceressEnergyStrike, DAMAGE, false, COST, "Energy Strike", Strength::Weak, WavFile("magicattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::MagicBarrier::MagicBarrier()
	:MakeBarrierMove(MoveId::SorceressMagicBarrier, BLOCK_AMOUNT, COST, "Magic Barrier", Strength::Weak)
{}

SorcererMoves::Heal::Heal()
	: Move(
		MoveId::SorceressHeal,
		"Heals " + std::to_string(HEAL_AMOUNT) + " HP, and uses " + std::to_string(AURA) + " Aura",
		"Heal",
		COST,
		Strength::Weak,
		true,
		WavFile("heal", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SorcererMoves::Heal::doAction(Creature* self, Creature* other)
{
	Sorcerer* sorcerer = dynamic_cast<Sorcerer*>(self);
	if (sorcerer != nullptr && !sorcerer->useAura(AURA))
	{
		return ColorString(ddutil::NOT_ENOUGH_AURA, ddutil::TEXT_COLOR);
	}

	int actualHealAmount = other->increaseHealth(HEAL_AMOUNT);

	return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString(" heals ", ddutil::TEXT_COLOR) + 
		ColorString(std::to_string(actualHealAmount) + " health", ddutil::HEAL_COLOR);
}

// Moderate

SorcererMoves::ElectricShock::ElectricShock()
	:StatusAttackMove(MoveId::SorceressElectricShock, DAMAGE, new ZappedStatus(), ZAPPED_LENGTH, COST, "Electric Shock", Strength::Moderate, WavFile("electricattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::MirrorImage::MirrorImage()
	:SelfBlockMove(MoveId::SorceressMirrorImage, BLOCK, COST, "Mirror Image", Strength::Powerful, WavFile("magicspell2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::Levitate::Levitate()
	:SelfBlockMove(MoveId::SorceressLevitate, BLOCK, COST, "Levitate", Strength::Moderate, WavFile("magicspell2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::Fireball::Fireball()
	:StatusAttackMove(MoveId::SorceressFireball, DAMAGE, new BurntStatus(), BURN_TIMES, COST, "Fireball", Strength::Moderate, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{}

SorcererMoves::IceOrb::IceOrb()
	: StatusAttackMove(MoveId::SorceressIceOrb, DAMAGE, new FrostBurntStatus(), FBURN_TIMES, COST, "Ice Orb", Strength::Moderate, WavFile("magicattack2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{}

SorcererMoves::BlinkStrike::BlinkStrike()
	:SimpleAttackMove(MoveId::SorceressBlinkStrike, DAMAGE, false, COST, "Blink Strike", Strength::Moderate, WavFile("attack2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::Recover::Recover()
	:SimpleHealMove(MoveId::SorceressRecover,HEAL_AMOUNT, false, COST, "Recover", Strength::Moderate)
{
}

SorcererMoves::SummonFireImp::SummonFireImp(Game* game)
	:SummonMove(MoveId::SorceressSummonFireImp, new FireImp(game), COST, "Summon Fire Imp", Strength::Moderate, WavFile("magicspell3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::SummonIceImp::SummonIceImp(Game* game)
	:SummonMove(MoveId::SorceressSummonIceImp,new IceImp(game), COST, "Summon Ice Imp", Strength::Moderate, WavFile("magicspell3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::EnergyBarrier::EnergyBarrier()
	: MakeBarrierMove(MoveId::SorceressEnergyBarrier, BLOCK_AMOUNT, COST, "Energy Barrier", Strength::Moderate)
{}

SorcererMoves::CleansingTouch::CleansingTouch()
	: Move(
		MoveId::SorceressCleansingTouch,
		"Heals " + std::to_string(HEAL_AMOUNT) +" HP, and uses " + std::to_string(AURA) + " Aura",
		"Cleansing Touch",
		COST,
		Strength::Moderate,
		true,
		WavFile("heal", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SorcererMoves::CleansingTouch::doAction(Creature* self, Creature* other)
{
	Sorcerer* sorcerer = dynamic_cast<Sorcerer*>(self);
	if (sorcerer != nullptr && !sorcerer->useAura(AURA))
	{
		return ColorString(ddutil::NOT_ENOUGH_AURA, ddutil::TEXT_COLOR);
	}

	int actualHealAmount = other->increaseHealth(HEAL_AMOUNT);

	return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString(" heals ", ddutil::TEXT_COLOR) + 
		ColorString(std::to_string(actualHealAmount) + " health", ddutil::HEAL_COLOR);
}

SorcererMoves::EnergySword::EnergySword()
	:SimpleAttackMove(MoveId::SorceressEnergySword, DAMAGE, true, COST, "Energy Sword", Strength::Moderate, WavFile("energyattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::PlasmaStrike::PlasmaStrike()
	:SimpleAttackMove(MoveId::SorceressPlasmaStrike, DAMAGE, false, COST, "Plasma Strike", Strength::Moderate, WavFile("energyattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::FairySummon::FairySummon(Game* game)
	:SummonMove(MoveId::SorceressFairySummon, new Fairy(game), COST, "Summon Fairy", Strength::Powerful, WavFile("magicspell3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::SummonFireDragon::SummonFireDragon(Game* game)
	:SummonMove(MoveId::SorceressSummonFireDragon, new FireDragon(game), COST, "Summon Fire Dragon", Strength::Powerful, WavFile("magicspell3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{

}

SorcererMoves::SummonIceDragon::SummonIceDragon(Game* game)
	:SummonMove(MoveId::SorceressSummonIceDragon, new IceDragon(game), COST, "Summon Ice Dragon", Strength::Powerful, WavFile("magicspell3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

// Powerful

SorcererMoves::Blizzard::Blizzard()
	:Move(MoveId::SorceressBlizzard, "Deals " + std::to_string(BASE_DAMAGE) + " damage (x"+std::to_string(MULTIPLIER)+" if enemy is frostburnt)", 
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
	:Move(MoveId::SorceressCataclysm, "Deals " + std::to_string(BASE_DAMAGE) + " damage (x" + std::to_string(MULTIPLIER) + " if enemy is burnt)", 
		"Cataclysm", COST, Strength::Powerful, true, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SorcererMoves::Cataclysm::doAction(Creature* self, Creature* other)
{
	int damageAmount = BASE_DAMAGE;
	if (other->hasStatus(StatusID::Burnt))
	{
		damageAmount *= 2;
	}

	ddutil::DamageReport damage = other->reduceHealth(damageAmount, self);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" deals ", ddutil::TEXT_COLOR) +
		ddutil::genericDamageAlert(damage) + ColorString(" to the ", ddutil::TEXT_COLOR) + other->getColorString();
}

SorcererMoves::PlasmaSword::PlasmaSword()
	:SimpleAttackMove(MoveId::SorceressPlasmaSword, DAMAGE, true, COST, "Plasma Sword", Strength::Powerful, WavFile("energyattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::EtherealBarrier::EtherealBarrier()
	:MakeBarrierMove(MoveId::SorceressEtherealBarrier, BLOCK_AMOUNT, COST, "Ethereal Barrier", Strength::Powerful)
{
}

SorcererMoves::StarStrike::StarStrike()
	:SimpleAttackMove(MoveId::SorceressStarStrike, DAMAGE, false, COST, "Star Strike", Strength::Powerful, WavFile("starattack", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::Combust::Combust()
	:Move(MoveId::SorceressCombust, "Deals "+std::to_string(DAM_PER_BURN)+" damage per burn on the target and removes all burn", "Combust",
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
	:Move(MoveId::SorceressFreeze, "Deals " + std::to_string(DAM_PER_FBURN) + " damage per frostburn on the target and removes all frostburn", "Freeze",
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
	:StatusAttackMove(MoveId::SorceressLightningStrike, BASE_DAMAGE, new ZappedStatus(), ZAPPED_LENGTH, COST, "Lightning Strike", Strength::Powerful, WavFile("lightning", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}


// Mythical

SorcererMoves::CleansingAura::CleansingAura()
	:Move(MoveId::SorceressCleansingAura, "Heals "+std::to_string(BASE_HEAL)+" health, plus "+std::to_string(BONUS_HEAL)+" health per status effect, and uses "+std::to_string(AURA)+" Aura",
		"Cleansing Aura", COST, Strength::Mythical, true, WavFile("heal", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SorcererMoves::CleansingAura::doAction(Creature* self, Creature* other)
{
	Sorcerer* sorcerer = dynamic_cast<Sorcerer*>(self);
	if (sorcerer != nullptr && !sorcerer->useAura(AURA))
	{
		return ColorString(ddutil::NOT_ENOUGH_AURA, ddutil::TEXT_COLOR);
	}

	int healAmount = BASE_HEAL + (BONUS_HEAL * other->getNumberOfStatusTypes());

	int actualHealAmount = other->increaseHealth(healAmount);

	return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString(" heals ", ddutil::TEXT_COLOR) + 
		ColorString(std::to_string(actualHealAmount) + " health", ddutil::HEAL_COLOR);
}

SorcererMoves::EnergyAura::EnergyAura()
	:Move(MoveId::SorceressEnergyAura, "Gives 2 additional vitality per turn this battle, and uses "+std::to_string(AURA)+" Aura", "Energy Aura", COST, Strength::Mythical, true, WavFile("magicspell3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
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
	Sorcerer* sorcerer = dynamic_cast<Sorcerer*>(self);
	if (sorcerer != nullptr && !sorcerer->useAura(AURA))
	{
		return ColorString(ddutil::NOT_ENOUGH_AURA, ddutil::TEXT_COLOR);
	}

	player->adjustVitalityGainTemp(VITALITY_GAIN);

	return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + 
		ColorString(" gains 2 additional vitality per turn", ddutil::TEXT_COLOR);
}

SorcererMoves::EtherealBlade::EtherealBlade()
	:SimpleAttackMove(MoveId::SorceressEtherealBlade, DAMAGE, true, 4, "Ethereal Blade", Strength::Mythical, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::SpiritCall::SpiritCall(Game* game)
	:SummonMove(MoveId::SorceressSpiritCall, new SpiritKnight(game), COST, "Spirit Call", Strength::Mythical, WavFile("spiritcall", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SorcererMoves::Supernova::Supernova()
	: Move(MoveId::SorceressSupernova, "Deals " + std::to_string(DAMAGE) + " damage to EVERYONE except the caster",
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
	:Move(
		MoveId::SorceressCastInvulnerability,
		"Gives target Invulnerable (1), Hexed ("+std::to_string(HEX)+"), and uses "+std::to_string(AURA)+" Aura", "Cast Invulnerability",
		COST,
		Strength::Mythical, 
		true,
		WavFile("ghostinvulnerable",
		ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SorcererMoves::CastInvulnerability::doAction(Creature* self, Creature* other)
{
	Sorcerer* sorcerer = dynamic_cast<Sorcerer*>(self);
	if (sorcerer != nullptr && !sorcerer->useAura(AURA))
	{
		return ColorString(ddutil::NOT_ENOUGH_AURA, ddutil::TEXT_COLOR);
	}

	other->applyStatus(new InvulnerableStatus(), 1);
	other->applyStatus(new HexedStatus(), HEX);
	return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString(" will be invulnerable this turn", ddutil::TEXT_COLOR);
}

SorcererMoves::ChainLightning::ChainLightning()
	:Move(MoveId::SorceressChainLightning,
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
	:Move(MoveId::SorceressElementalForm, "Makes every attack apply Zapped, Burnt, and Frostburnt",
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

SorcererMoves::ElementalBarrier::ElementalBarrier()
	:MakeBarrierMove(MoveId::SorceressElementalBarrier, BLOCK, COST, "Elemental Barrier", Strength::Mythical)
{
}

SorcererMoves::UltimateShock::UltimateShock()
	: Move(
		MoveId::SorceressUltimateShock,
		"Turn all the target's status effects into Zapped",
		"Ultimate Shock",
		COST,
		Strength::Mythical,
		true,
		WavFile("electricattack2", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SorcererMoves::UltimateShock::doAction(Creature* self, Creature* other)
{
	int numZap = other->getNumberOfStatuses();
	other->clearAllStatuses();
	if (numZap > 0)
	{
		other->applyStatus(new ZappedStatus(), numZap);
	}
	return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString("'s", other->getColor()) +
		ColorString(" statuses are replaced with ", ddutil::TEXT_COLOR) + ColorString("Zapped (" + std::to_string(numZap) + ")", ZappedStatus::COLOR);
}

SorcererMoves::XCast::XCast()
	:MultiAttackMove(
		MoveId::SorceressXCast,
		DAM,
		TIMES,
		COST,
		"X-Cast",
		Strength::Mythical,
		WavFile("dualenergyattack", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

SorcererMoves::TreeOfPower::TreeOfPower()
	:Move(
		MoveId::SorceressTreeOfPower,
		"Deals "+std::to_string(DAMAGE)+" damage, but permanently reduces max HP by "+std::to_string(MAX_HP_RED),
		"Tree of Power",
		COST,
		Strength::Mythical,
		true,
		WavFile("dualenergyattack", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SorcererMoves::TreeOfPower::doAction(Creature* self, Creature* other)
{
	self->decreaseMaxHealth(MAX_HP_RED);
	SimpleAttackMove tempMove(MoveId::SorceressTreeOfPower, DAMAGE, false, 0, "Tree Of Power", Strength::Mythical, sound);
	return tempMove.doAction(self, other) + ColorString(" and loses ", ddutil::TEXT_COLOR) +
		ColorString(std::to_string(MAX_HP_RED) + " max HP", ddutil::DAMAGE_COLOR);
}

SorcererMoves::AuraBomb::AuraBomb()
	:AuraDamageMove(
		MoveId::SorceressAuraBomb,
		DAM_PER_AURA,
		COST,
		"Aura Bomb",
		Strength::Mythical,
		WavFile("explosion", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

SorcererMoves::TreeOfLife::TreeOfLife()
	:Move(
		MoveId::SorceressTreeOfLife,
		"Deals " + std::to_string(DAMAGE_HEAL) + " damage and heals the user for how much damage was dealt",
		"Tree of Life",
		COST,
		Strength::Mythical,
		true,
		WavFile("lightningheal", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SorcererMoves::TreeOfLife::doAction(Creature* self, Creature* other)
{
	auto damRep = other->reduceHealth(DAMAGE_HEAL, self);
	int heal = damRep.getDamageTaken();
	int healAmount = self->increaseHealth(heal);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" deals ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep) +
		ColorString(" and", ddutil::TEXT_COLOR) + ColorString(" heals " + std::to_string(healAmount) + " HP", ddutil::HEAL_COLOR);
}

SorcererMoves::Resurrect::Resurrect()
	:Move(
		MoveId::SorceressResurrect,
		"Revives a dead minion from this fight",
		"Resurrect",
		COST,
		Strength::Mythical,
		false,
		WavFile("lightningheal", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SorcererMoves::Resurrect::doAction(Creature* self, Creature* other)
{
	Player* minion = self->getGamePtr()->reviveFirstDeadMinion();
	if (minion == nullptr)
	{
		return ColorString(ddutil::CANT_USE_MOVE, ddutil::TEXT_COLOR);
	}
	else
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
			ColorString(" resurrects the ", ddutil::TEXT_COLOR) + minion->getColorString();
	}
}

SorcererMoves::SelfHex::SelfHex()
	:Move(
		MoveId::SorceressSelfHex,
		"Removes all status effects but applies Hexed (" + std::to_string(HEX_LEN) + ")",
		"Self Hex",
		COST,
		Strength::Powerful,
		false,
		WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SorcererMoves::SelfHex::doAction(Creature* self, Creature* other)
{
	self->clearAllStatuses();
	self->applyStatus(new HexedStatus(), HEX_LEN);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" hexes", HexedStatus::COLOR)+ColorString(" herself to remove all her status effects", ddutil::TEXT_COLOR);
}

SorcererMoves::BindingGrasp::BindingGrasp()
	:Move(
		MoveId::SorceressTreeOfLife,
		"Deals " + std::to_string(DAMAGE_HEAL) + " damage and heals the user for how much damage was dealt",
		"Binding Grasp",
		COST,
		Strength::Powerful,
		true,
		WavFile("attackheal", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SorcererMoves::BindingGrasp::doAction(Creature* self, Creature* other)
{
	auto damRep = other->reduceHealth(DAMAGE_HEAL, self);
	int heal = damRep.getDamageTaken();
	int healAmount = self->increaseHealth(heal);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" deals ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep) +
		ColorString(" and", ddutil::TEXT_COLOR) + ColorString(" heals " + std::to_string(healAmount) + " HP", ddutil::HEAL_COLOR);
}

SorcererMoves::AuraBlast::AuraBlast()
	:AuraDamageMove(
		MoveId::SorceressAuraBlast,
		DAM_PER_AURA,
		COST,
		"Aura Blast",
		Strength::Powerful,
		WavFile("energyattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

SorcererMoves::BranchWhip::BranchWhip()
	:Move(
		MoveId::SorceressBranchWhip,
		"Deals "+std::to_string(DAMAGE)+" and heals "+std::to_string(HEAL)+" HP",
		"Branch Whip",
		COST,
		Strength::Moderate,
		true,
		WavFile("attackheal", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SorcererMoves::BranchWhip::doAction(Creature* self, Creature* other)
{
	int healAmount = self->increaseHealth(HEAL);
	SimpleAttackMove tempMove(MoveId::SorceressBranchWhip, DAMAGE, false, 0, "Branch Whip", Strength::Moderate, sound);
	return tempMove.doAction(self, other) + ColorString(" and ", ddutil::TEXT_COLOR) + ColorString("heals " + std::to_string(HEAL) + " HP", ddutil::HEAL_COLOR);
}

SorcererMoves::AuraStrike::AuraStrike()
	:AuraDamageMove(
		MoveId::SorceressAuraStrike,
		DAM_PER_AURA,
		COST,
		"Aura Strike",
		Strength::Moderate,
		WavFile("energyattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}
