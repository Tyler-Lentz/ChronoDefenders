#include "samurai_moves.h"
#include "player.h"
#include "creature.h"
#include "colorstring.h"
#include "game.h"
#include "move.h"
#include "enemy.h"
#include "soundfile.h"

// Base



SamuraiMoves::ZenMove::ZenMove(MoveId id, int zenLevel, int cost, std::string name, Strength str)
	:Move(
		id,
		"Enter Zen "+std::to_string(zenLevel)+", which heals "+std::to_string(ZenStatus::HP_GAIN)+" HP per turn",
		name,
		cost,
		str,
		false,
		WavFile("magicspell2", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
	zenAmount = zenLevel;
}

ColorString SamuraiMoves::ZenMove::doAction(Creature* self, Creature* other)
{
	ZenStatus* newStatus = new ZenStatus();
	ColorString statusName = newStatus->getName();
	self->applyStatus(newStatus, zenAmount);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" enters ", ddutil::TEXT_COLOR) + statusName;
}


SamuraiMoves::StormMove::StormMove(MoveId id, int targetStorm, int otherStorm, bool chooseTarget, int cost, std::string name, Strength str, WavFile theSound)
	:Move(id, chooseTarget ?
		"Apply Storm " + std::to_string(targetStorm) + " to the target and Storm " + std::to_string(otherStorm) + " to everyone else" :
		"Apply Storm " + std::to_string(otherStorm) + " to everybody",
		name, cost, str, chooseTarget, theSound)
{
	targetStormLevel = targetStorm;
	nonTargetStormLevel = otherStorm;
}

ColorString SamuraiMoves::StormMove::doAction(Creature* self, Creature* other)
{
	StormStatus* newStatus = new StormStatus();
	ColorString statusName = newStatus->getName();
	Game* game = self->getGamePtr();

	std::vector<Creature*> creatureList;
	creatureList.push_back(game->getCurrentEnemy());
	for (Player* p : game->getPlayerParty())
	{
		creatureList.push_back(p);
	}

	for (Creature* c : creatureList)
	{
		if (c == other)
		{
			c->applyStatus(newStatus->makeCopy(), targetStormLevel);
		}
		else
		{
			c->applyStatus(newStatus->makeCopy(), nonTargetStormLevel);
		}
	}

	// Display output
	if (canChooseTarget())
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString(" will be under a ", ddutil::TEXT_COLOR) +
			this->getColorString() + ColorString(" for " + std::to_string(targetStormLevel) + " turn" + (nonTargetStormLevel > 1 ? "s" : ""), ddutil::TEXT_COLOR) +
			ColorString(" while everyone else will be for " + std::to_string(nonTargetStormLevel) + " turn" + (nonTargetStormLevel > 1 ? "s" : ""), ddutil::TEXT_COLOR);
	}
	else
	{
		return ColorString("A ", ddutil::TEXT_COLOR) + this->getColorString() +
			ColorString(" will cover the battlefield for " + std::to_string(nonTargetStormLevel) + " turn" + (nonTargetStormLevel > 1 ? "s" : ""), ddutil::TEXT_COLOR);
	}
}


SamuraiMoves::VitalityBoostedAttack::VitalityBoostedAttack(MoveId id, int theBaseDamage, int theMultiplier, int theVitalityAmount,
	std::string name, int cost, Strength str, WavFile theSound)
	:Move(id, "Deals " + std::to_string(theBaseDamage) + " damage (x" + std::to_string(theMultiplier) +
		" if vitality is " + std::to_string(theVitalityAmount) + " after attacking)",
		name, cost, str, true, theSound)
{
	baseDamage = theBaseDamage;
	multiplier = theMultiplier;
	vitalityAmount = theVitalityAmount;
}

ColorString SamuraiMoves::VitalityBoostedAttack::doAction(Creature* self, Creature* other)
{
	int actualDamage = baseDamage;
	Player* player = dynamic_cast<Player*>(self);
	if (player != nullptr && player->getVitality() == vitalityAmount)
	{
		actualDamage *= multiplier;
	}

	SimpleAttackMove temp = SimpleAttackMove(MoveId::TempMoveId,actualDamage, false, 0, "TEMP", Strength::Powerful, sound);
	return temp.doAction(self, other);
}

// Weak

SamuraiMoves::Slice::Slice()
	:SimpleAttackMove(MoveId::SamuraiSlice, DAMAGE, false, COST, "Slice", Strength::Weak, WavFile("attack2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Deflect::Deflect()
	:SelfBlockMove(MoveId::SamuraiDeflect,BLOCK, COST, "Deflect", Strength::Weak, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

// Moderate

SamuraiMoves::ThrowingKnives::ThrowingKnives()
	:StatusAttackMove(MoveId::SamuraiThrowingKnives, DAMAGE, new BleedingStatus(), BLEED_AMOUNT, COST, "Throwing Knives",
		Strength::Moderate, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::PerilousStrike::PerilousStrike()
	:SelfDamageAttackMove(MoveId::SamuraiPerilousStrike, DAMAGE, false, SELF_DAMAGE, COST, "Perilous Strike", Strength::Moderate, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::CalculatedSlice::CalculatedSlice()
	:AttackAndBlockMove(MoveId::SamuraiCalculatedSlice, DAMAGE, BLOCK, false, COST, "Calculated Slice", Strength::Moderate, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Sidestep::Sidestep()
	:SelfBlockMove(MoveId::SamuraiSidestep, BLOCK_AMOUNT, COST, "Sidestep", Strength::Moderate, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Meditation::Meditation()
	:ZenMove(MoveId::SamuraiMeditation, ZEN_AMOUNT, COST, "Meditation", Strength::Moderate)
{
	
}


SamuraiMoves::Storm::Storm()
	:StormMove(MoveId::SamuraiStorm, STORM_LEVEL, STORM_LEVEL, false, COST, "Storm", Strength::Moderate, WavFile("lightning", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::LightningRod::LightningRod()
	:Move(MoveId::SamuraiLightningRod, "If under a Storm, gain "+std::to_string(VITALITY_GAIN)+" Vitality", "Lightning Rod", COST, Strength::Moderate, false, WavFile("lightning", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SamuraiMoves::LightningRod::doAction(Creature* self, Creature* other)
{
	if (self->hasStatus(StatusID::Storm))
	{
		Player* player = dynamic_cast<Player*>(self);
		if (player == nullptr)
		{
			return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
				ColorString(" cannot gain vitality", ddutil::TEXT_COLOR);
		}

		player->addVitality(VITALITY_GAIN);
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
			ColorString(" gains ", ddutil::TEXT_COLOR) + ColorString(std::to_string(VITALITY_GAIN) + " vitality", ddutil::VITALITY_COLOR);
	}
	else
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + this->getColorString() + 
			ColorString(" fails because there is no Storm", ddutil::TEXT_COLOR);
	}
}

SamuraiMoves::Assault::Assault()
	:SimpleAttackMove(MoveId::SamuraiAssault, DAMAGE, false, COST, "Assault", Strength::Moderate, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::DualSlice::DualSlice()
	:MultiAttackMove(MoveId::SamuraiDualSlice, DAMAGE, TIMES, COST, "Dual Slice", Strength::Moderate, WavFile("dualattack", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::BodySlam::BodySlam()
	: BlockBreakingMove(MoveId::SamuraiBodySlam, DAMAGE, MULTIPLIER, COST, "Body Slam", Strength::Moderate, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

// Powerful

SamuraiMoves::ShadowStep::ShadowStep()
	:Move(MoveId::SamuraiShadowStep, "Blocks " + std::to_string(BLOCK) + " damage, but applies Hexed " + std::to_string(HEXED_LENGTH),
		"Shadow Step", COST, Strength::Powerful, false, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SamuraiMoves::ShadowStep::doAction(Creature* self, Creature* other)
{
	self->applyBlock(BLOCK);
	self->applyStatus(new HexedStatus(), HEXED_LENGTH);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" will ", ddutil::TEXT_COLOR) + ColorString("block " + std::to_string(BLOCK) + " damage", ddutil::BLOCK_COLOR) +
		ColorString(", but is now ", ddutil::TEXT_COLOR) + ColorString("Hexed", HexedStatus::COLOR) +
		ColorString(" for " + std::to_string(HEXED_LENGTH) + " turns", ddutil::TEXT_COLOR);
}

SamuraiMoves::Shuriken::Shuriken()
	:StatusAttackMove(MoveId::SamuraiShuriken, DAMAGE, new BleedingStatus(), BLEED_AMOUNT, COST, "Shuriken", Strength::Powerful,
		WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Typhoon::Typhoon()
	:StormMove(MoveId::SamuraiTyphoon, TARGET_STORM_LEVEL, OTHER_STORM_LEVEL, true, COST, "Typhoon", Strength::Powerful, WavFile("wind", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::DragonSlice::DragonSlice()
	:StatusAttackMove(MoveId::SamuraiDragonSlice, DAMAGE, new BurntStatus(), BURN_AMOUNT, COST, "Dragon Slice", Strength::Powerful, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::PerilousPlunge::PerilousPlunge()
	:SelfDamageAttackMove(MoveId::SamuraiPerilousPlunge, DAMAGE, false, SELF_DAMAGE, COST, "Perilous Plunge", Strength::Powerful, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Zen::Zen()
	:ZenMove(MoveId::SamuraiZen, ZEN_LEVEL, COST, "Zen", Strength::Powerful)
{
}

SamuraiMoves::ChargedAssault::ChargedAssault()
	:VitalityBoostedAttack(MoveId::SamuraiChargedAssault, DAMAGE, MULTIPLIER, VITALITY_AMOUNT, "Charged Assault", COST, Strength::Powerful, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Desperation::Desperation()
	:Move(MoveId::SamuraiDesperation, "Deals "+std::to_string(DAMAGE)+" damage if HP is below "+std::to_string(HEALTH_PERCENTAGE)+"%",
		"Desperation", COST, Strength::Powerful, true, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SamuraiMoves::Desperation::doAction(Creature* self, Creature* other)
{
	int damage = 0;
	if (self->getHealth() <= self->getMaxHealth(HEALTH_PERCENTAGE))
	{
		damage = DAMAGE;
	}
	else
	{
		return ColorString(ddutil::CANT_USE_MOVE, ddutil::TEXT_COLOR);
	}

	SimpleAttackMove tempMove(MoveId::TempMoveId, damage, false, 0, "TEMP", Strength::Powerful, sound);
	return tempMove.doAction(self, other);
}

SamuraiMoves::CrushingBlow::CrushingBlow()
	:BlockBreakingMove(MoveId::SamuraiCrushingBlow, DAMAGE, MULTIPLIER, COST, "Crushing Blow", Strength::Powerful, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::CriticalStrike::CriticalStrike()
	:AttackAndBlockMove(MoveId::SamuraiCriticalStrike, DAMAGE, BLOCK, false, COST, "Critical Strike", Strength::Powerful, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{

}

SamuraiMoves::FinishingBlow::FinishingBlow()
	:VitalityBoostedAttack(MoveId::SamuraiFinishingBlow, DAMAGE, MULTIPLIER, VITALITY_AMOUNT, "Finishing Blow", COST, Strength::Powerful, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::EyeOfTheStorm::EyeOfTheStorm()
	:Move(MoveId::SamuraiEyeOfTheStorm, "The target blocks "+std::to_string(BLOCK)+" damage. If under a Storm, block x"+std::to_string(MULTIPLIER)+" and remove the Storm",
		"Eye of the Storm", COST, Strength::Powerful, true, WavFile("magicspell1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SamuraiMoves::EyeOfTheStorm::doAction(Creature* self, Creature* other)
{
	int blockAmount = BLOCK;
	if (other->hasStatus(StatusID::Storm))
	{
		blockAmount *= MULTIPLIER;
		other->removeStatus(StatusID::Storm);
	}

	SelfBlockMove tempMove(MoveId::TempMoveId, blockAmount, 0, "TEMP", Strength::Powerful, sound);
	return tempMove.doAction(other, other);
}

// Mythical

SamuraiMoves::TippedKunai::TippedKunai()
	:StatusAttackMove(MoveId::SamuraiTippedKunai, DAMAGE, new PoisonedStatus(), POISON_AMOUNT, COST, "Tipped Kunai", Strength::Mythical,
		WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::BlindingFury::BlindingFury()
	:SimpleAttackMove(MoveId::SamuraiBlindingFury, DAMAGE, false, COST, "Blinding Fury", Strength::Mythical, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Tsunami::Tsunami()
	:Move(MoveId::SamuraiTsunami,"Deals " + std::to_string(DAMAGE) + " damage (x" + std::to_string(VITALITY_MULTIPLIER) +
		" if vitality is " + std::to_string(VITALITY_LEVEL) + " after attacking; x"+
		std::to_string(STORM_MULTIPLIER)+" if target under Storm)", "Tsunami", COST, Strength::Mythical, true, 
		WavFile("tsunami", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SamuraiMoves::Tsunami::doAction(Creature* self, Creature* other)
{
	int actualDamage = DAMAGE;
	if (other->hasStatus(StatusID::Storm))
	{
		actualDamage *= STORM_MULTIPLIER;
	}
	VitalityBoostedAttack tempMove(MoveId::TempMoveId, actualDamage, VITALITY_MULTIPLIER, VITALITY_LEVEL, "TEMP", 0, Strength::Mythical, sound);
	return tempMove.doAction(self, other);
}

SamuraiMoves::LeavesFromTheVine::LeavesFromTheVine()
	:Move(MoveId::SamuraiLeavesFromTheVine, "The target loses all statuses and gain "+std::to_string(BLOCK_PER_STATUS)+" block per status",
		"Leaves From the Vine", COST, Strength::Mythical, true, WavFile("magicspell2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SamuraiMoves::LeavesFromTheVine::doAction(Creature* self, Creature* other)
{
	int blockAmount = BLOCK_PER_STATUS * other->getNumberOfStatuses();
	other->clearAllStatuses();
	other->resetPicture();
	other->applyBlock(blockAmount);
	return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString(" is cleansed of all their statuses and will block ", ddutil::TEXT_COLOR) +
		ddutil::genericBlockAlert(blockAmount);
}

SamuraiMoves::Enlightenment::Enlightenment()
	:ZenMove(MoveId::SamuraiEnlightenment, ZEN_AMOUNT, COST, "Enlightenment", Strength::Mythical)
{
}

SamuraiMoves::Eviscerate::Eviscerate()
	:SelfDamageAttackMove(MoveId::SamuraiEviscerate, DAMAGE, false, SELF_DAMAGE, COST, "Eviscerate", Strength::Mythical, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::DualingDragons::DualingDragons()
	:Move(MoveId::SamuraiDualingDragons, "Deals "+std::to_string(DAMAGE)+" damage "+std::to_string(TIMES)+" times and applies burn "+std::to_string(BURN_AMOUNT)+ " times",
		"Dualing Dragons", COST, Strength::Mythical, true, WavFile("dualattack", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SamuraiMoves::DualingDragons::doAction(Creature* self, Creature* other)
{
	BurntStatus* newStatus = new BurntStatus();
	ColorString statusName = newStatus->getName();
	other->applyStatus(newStatus, BURN_AMOUNT);
	
	MultiAttackMove tempMove(MoveId::TempMoveId, DAMAGE, TIMES, 0, "TEMP", Strength::Mythical, sound);
	return tempMove.doAction(self, other);
}

SamuraiMoves::Caltrops::Caltrops()
	:SimpleStatusMove(MoveId::SamuraiCaltrops, new ThornsStatus(), THORNS_AMOUNT, true, COST, "Caltrops", Strength::Mythical,
		WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::DragonForm::DragonForm()
	:Move(MoveId::SamuraiDragonForm, "Applies a buff which increases damage by "+std::to_string(DragonStatus::PERCENT_DAM_INC)+"% for " + std::to_string(DURATION) + " turns", "Dragon Form", COST,
		Strength::Mythical, false, WavFile("dragonform", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SamuraiMoves::DragonForm::doAction(Creature* self, Creature* other)
{
	DragonStatus* newStatus = new DragonStatus();
	ColorString statusName = newStatus->getName();
	self->applyStatus(newStatus, DURATION);
	self->changePicture(Art::getDragonSamurai());

	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" enters ", ddutil::TEXT_COLOR) + statusName +
		ColorString(" for " + std::to_string(DURATION) + " turns", ddutil::TEXT_COLOR);
}

SamuraiMoves::DragonBreath::DragonBreath()
	:StatusAttackMove(MoveId::SamuraiDragonBreath, DAMAGE, new ScorchedStatus(), SCORCH_LENGTH, COST, "Dragon's Breath",
		Strength::Mythical, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Tornado::Tornado()
	:SimpleStatusMove(MoveId::SamuraiTornado, new StormStatus(), STORM_LENGTH, true, COST, "Tornado", 
		Strength::Mythical, WavFile("wind", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::ShinobiTactics::ShinobiTactics()
	:Move(MoveId::SamuraiShinobiTactics, "Blocks " + std::to_string(BLOCK) + " damage and applies "+std::to_string(BLEED_AMOUNT)+" Bleeding to the target",
		"Shinobi Tactics", COST, Strength::Mythical, true, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SamuraiMoves::ShinobiTactics::doAction(Creature* self, Creature* other)
{
	self->applyBlock(BLOCK);
	other->applyStatus(new BleedingStatus(), BLEED_AMOUNT);

	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" blocks " + std::to_string(BLOCK) + " damage ", ddutil::BLOCK_COLOR) +
		ColorString("and applies ", ddutil::TEXT_COLOR) + ColorString(std::to_string(BLEED_AMOUNT) + " Bleeding ", BleedingStatus::COLOR) +
		ColorString(" to the ", ddutil::TEXT_COLOR) + other->getColorString();
}

SamuraiMoves::WarHorn::WarHorn()
	:Move(
		MoveId::SamuraiWarHorn,
		"Applies Beserked for "+std::to_string(LENGTH)+" turns",
		"War Horn",
		COST,
		Strength::Powerful,
		false,
		WavFile("warhorn", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SamuraiMoves::WarHorn::doAction(Creature* self, Creature* other)
{
	self->applyStatus(new BeserkedStatus(), LENGTH);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" becomes ", ddutil::TEXT_COLOR) +
		ColorString("Beserked", BeserkedStatus::COLOR) + ColorString(" for " + std::to_string(LENGTH) + " turns", ddutil::TEXT_COLOR);
}

SamuraiMoves::Instinct::Instinct()
	:Move(
		MoveId::SamuraiInstinct,
		"Blocks damage equal to the amount of self-damage inflicted this fight",
		"Instinct",
		COST,
		Strength::Powerful,
		false,
		WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SamuraiMoves::Instinct::doAction(Creature* self, Creature* other)
{
	int block = self->getSelfDamageThisFight();
	SelfBlockMove tempMove(MoveId::SamuraiInstinct, block, 0, "Instinct", Strength::Powerful, sound);
	return tempMove.doAction(self, other);
}

SamuraiMoves::Unhinge::Unhinge()
	:Move(
		MoveId::SamuraiUnhinge,
		"Deals " + std::to_string(DAMAGE) + " damage, and applies Stunned (" + std::to_string(STUN) + ") to the user",
		"Unhinge",
		COST,
		Strength::Powerful,
		true,
		WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SamuraiMoves::Unhinge::doAction(Creature* self, Creature* other)
{
	self->applyStatus(new StunnedStatus(), STUN);
	SimpleAttackMove tempMove(MoveId::SamuraiUnhinge, DAMAGE, false, 0, "Unhinge", Strength::Powerful, sound);
	return tempMove.doAction(self, other);
}

SamuraiMoves::Revitalize::Revitalize()
	:Move(
		MoveId::SamuraiRevitalize,
		"If Beserked, gain "+std::to_string(VIT_GAIN)+" vitality and take "+std::to_string(SELF_DAMAGE)+" self-damage",
		"Revitalize",
		COST,
		Strength::Powerful,
		false,
		WavFile("gainpower", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SamuraiMoves::Revitalize::doAction(Creature* self, Creature* other)
{
	if (self->hasStatus(StatusID::Beserked))
	{
		auto damRep = self->reduceHealth(SELF_DAMAGE, self, false);
		Player* playerSelf = dynamic_cast<Player*>(self);
		if (playerSelf != nullptr)
		{
			playerSelf->addVitality(VIT_GAIN);
		}
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
			ColorString(" gains ", ddutil::TEXT_COLOR) + ColorString(std::to_string(VIT_GAIN) + " vitality ", ddutil::VITALITY_COLOR) +
			ColorString("and takes ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep);
	}
	else
	{
		return ColorString(ddutil::CANT_USE_MOVE, ddutil::TEXT_COLOR);
	}
}

SamuraiMoves::FlameVeil::FlameVeil()
	:Move(
		MoveId::SamuraiFlameVeil,
		"Blocks "+std::to_string(BLOCK)+"damage, gives self "+std::to_string(BURNS)+" burns",
		"Flame Veil",
		COST,
		Strength::Powerful,
		false,
		WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SamuraiMoves::FlameVeil::doAction(Creature* self, Creature* other)
{
	self->applyStatus(new BurntStatus(), BURNS);
	SelfBlockMove tempMove(MoveId::SamuraiFlameVeil, BLOCK, 0, "Flame Veil", Strength::Powerful, sound);
	return tempMove.doAction(self, other) + ColorString(" and gets ", ddutil::TEXT_COLOR) + 
		ColorString("Burnt (" + std::to_string(BURNS) + ")", BurntStatus::COLOR);
}

SamuraiMoves::HelpingHand::HelpingHand()
	:Move(
		MoveId::SamuraiHelpingHand,
		"If the target has less HP, gives them "+std::to_string(BLOCK)+" block",
		"Helping Hand",
		COST,
		Strength::Moderate,
		true,
		WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SamuraiMoves::HelpingHand::doAction(Creature* self, Creature* other)
{
	if (other->getHealth() < self->getHealth())
	{
		other->applyBlock(BLOCK);
		return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString(" gains ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(BLOCK) + " block", ddutil::BLOCK_COLOR);
	}
	else
	{
		return ColorString(ddutil::CANT_USE_MOVE, ddutil::TEXT_COLOR);
	}
}

SamuraiMoves::Tackle::Tackle()
	:Move(
		MoveId::SamuraiTackle,
		"Deals "+std::to_string(BASE_DAM)+" damage, plus " + std::to_string(DAM_PER_THORN) + " per stack of Thorned",
		"Tackle",
		COST,
		Strength::Moderate,
		true,
		WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SamuraiMoves::Tackle::doAction(Creature* self, Creature* other)
{
	int damage = BASE_DAM + self->getNumberOfStatuses(StatusID::Thorns);
	SimpleAttackMove tempMove(MoveId::SamuraiTackle, damage, false, 0, "Tackle", Strength::Moderate, sound);
	return tempMove.doAction(self, other);
}

SamuraiMoves::ToughenUp::ToughenUp()
	:Move(
		MoveId::SamuraiToughenUp,
		"Blocks " + std::to_string(BLOCK) + " damage, but inflicts " + std::to_string(SELF_DAM)+" self-damage",
		"Toughen Up",
		COST,
		Strength::Moderate,
		false,
		WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString SamuraiMoves::ToughenUp::doAction(Creature* self, Creature* other)
{
	auto damRep =  self->reduceHealth(SELF_DAM, self);
	SelfBlockMove tempMove(MoveId::SamuraiToughenUp, BLOCK, 0, "Toughen Up", Strength::Moderate, sound);
	return tempMove.doAction(self, other) +
			ColorString("and takes ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep);
}
