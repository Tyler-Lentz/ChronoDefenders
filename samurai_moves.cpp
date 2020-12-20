#include "samurai_moves.h"
#include "player.h"
#include "creature.h"
#include "colorstring.h"
#include "game.h"
#include "move.h"
#include "enemy.h"
#include "soundfile.h"

// Base



SamuraiMoves::ZenMove::ZenMove(int zenLevel, int cost, std::string name, Strength str)
	:Move("Enter Zen "+std::to_string(zenLevel)+", which applies healing per turn", name, cost, str, false, WavFile("magicspell2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
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


SamuraiMoves::StormMove::StormMove(int targetStorm, int otherStorm, bool chooseTarget, int cost, std::string name, Strength str, WavFile theSound)
	:Move(chooseTarget ?
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


SamuraiMoves::VitalityBoostedAttack::VitalityBoostedAttack(int theBaseDamage, int theMultiplier, int theVitalityAmount,
	std::string name, int cost, Strength str, WavFile theSound)
	:Move("Deals " + std::to_string(theBaseDamage) + " damage (x" + std::to_string(theMultiplier) +
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

	SimpleAttackMove temp = SimpleAttackMove(actualDamage, false, 0, "TEMP", Strength::Powerful, sound);
	return temp.doAction(self, other);
}

// Weak

SamuraiMoves::Slice::Slice()
	:SimpleAttackMove(DAMAGE, false, COST, "Slice", Strength::Weak, WavFile("attack2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Deflect::Deflect()
	:SelfBlockMove(BLOCK, COST, "Deflect", Strength::Weak, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

// Moderate

SamuraiMoves::ThrowingKnives::ThrowingKnives()
	:StatusAttackMove(DAMAGE, new BleedingStatus(), BLEED_AMOUNT, COST, "Throwing Knives",
		Strength::Moderate, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::PerilousStrike::PerilousStrike()
	:SelfDamageAttackMove(DAMAGE, false, SELF_DAMAGE, COST, "Perilous Strike", Strength::Moderate, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::CalculatedSlice::CalculatedSlice()
	:AttackAndBlockMove(DAMAGE, BLOCK, false, COST, "Calculated Slice", Strength::Moderate, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Sidestep::Sidestep()
	:SelfBlockMove(BLOCK_AMOUNT, COST, "Sidestep", Strength::Moderate, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Meditation::Meditation()
	:ZenMove(ZEN_AMOUNT, COST, "Meditation", Strength::Moderate)
{
	
}


SamuraiMoves::Storm::Storm()
	:StormMove(STORM_LEVEL, STORM_LEVEL, false, COST, "Storm", Strength::Moderate, WavFile("lightning", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::LightningRod::LightningRod()
	:Move("If under a Storm, gain "+std::to_string(VITALITY_GAIN)+" Vitality", "Lightning Rod", COST, Strength::Moderate, false, WavFile("lightning", ddutil::SF_LOOP, ddutil::SF_ASYNC))
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
	:SimpleAttackMove(DAMAGE, false, COST, "Assault", Strength::Moderate, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::DualSlice::DualSlice()
	:MultiAttackMove(DAMAGE, TIMES, COST, "Dual Slice", Strength::Moderate, WavFile("dualattack", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::BodySlam::BodySlam()
	: BlockBreakingMove(DAMAGE, MULTIPLIER, COST, "Body Slam", Strength::Moderate, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

// Powerful

SamuraiMoves::Shuriken::Shuriken()
	:StatusAttackMove(DAMAGE, new BleedingStatus(), BLEED_AMOUNT, COST, "Shuriken", Strength::Powerful,
		WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Typhoon::Typhoon()
	:StormMove(TARGET_STORM_LEVEL, OTHER_STORM_LEVEL, true, COST, "Typhoon", Strength::Powerful, WavFile("wind", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::DragonSlice::DragonSlice()
	:StatusAttackMove(DAMAGE, new BurntStatus(), BURN_AMOUNT, COST, "Dragon Slice", Strength::Powerful, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::PerilousPlunge::PerilousPlunge()
	:SelfDamageAttackMove(DAMAGE, false, SELF_DAMAGE, COST, "Perilous Plunge", Strength::Powerful, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Zen::Zen()
	:ZenMove(ZEN_LEVEL, COST, "Zen", Strength::Powerful)
{
}

SamuraiMoves::ChargedAssault::ChargedAssault()
	:VitalityBoostedAttack(DAMAGE, MULTIPLIER, VITALITY_AMOUNT, "Charged Assault", COST, Strength::Powerful, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Desperation::Desperation()
	:Move("Deals "+std::to_string(DAMAGE)+" damage if HP is below "+std::to_string(HEALTH_PERCENTAGE)+"%",
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

	SimpleAttackMove tempMove(damage, false, 0, "TEMP", Strength::Powerful, sound);
	return tempMove.doAction(self, other);
}

SamuraiMoves::CrushingBlow::CrushingBlow()
	:BlockBreakingMove(DAMAGE, MULTIPLIER, COST, "Crushing Blow", Strength::Powerful, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::CriticalStrike::CriticalStrike()
	:AttackAndBlockMove(DAMAGE, BLOCK, false, COST, "Critical Strike", Strength::Powerful, WavFile("attackblock", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{

}

SamuraiMoves::FinishingBlow::FinishingBlow()
	:VitalityBoostedAttack(DAMAGE, MULTIPLIER, VITALITY_AMOUNT, "Finishing Blow", COST, Strength::Powerful, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::EyeOfTheStorm::EyeOfTheStorm()
	:Move("The target blocks "+std::to_string(BLOCK)+" damage. If under a Storm, block x"+std::to_string(MULTIPLIER)+" and remove the Storm",
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

	SelfBlockMove tempMove(blockAmount, 0, "TEMP", Strength::Powerful, sound);
	return tempMove.doAction(other, other);
}

// Mythical

SamuraiMoves::TippedKunai::TippedKunai()
	:StatusAttackMove(DAMAGE, new PoisonedStatus(), POISON_AMOUNT, COST, "Tipped Kunai", Strength::Mythical,
		WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::BlindingFury::BlindingFury()
	:SimpleAttackMove(DAMAGE, false, COST, "Blinding Fury", Strength::Mythical, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::Tsunami::Tsunami()
	:Move("Deals " + std::to_string(DAMAGE) + " damage (x" + std::to_string(VITALITY_MULTIPLIER) +
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
	VitalityBoostedAttack tempMove(actualDamage, VITALITY_MULTIPLIER, VITALITY_LEVEL, "TEMP", 0, Strength::Mythical, sound);
	return tempMove.doAction(self, other);
}

SamuraiMoves::LeavesFromTheVine::LeavesFromTheVine()
	:Move("The target loses all statuses and gain "+std::to_string(BLOCK_PER_STATUS)+" block per status",
		"Leaves From the Vine", COST, Strength::Mythical, true, WavFile("magicspell2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SamuraiMoves::LeavesFromTheVine::doAction(Creature* self, Creature* other)
{
	int blockAmount = BLOCK_PER_STATUS * other->getNumberOfStatuses();
	other->clearAllStatuses();
	other->applyBlock(blockAmount);
	return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString(" is cleansed of all their statuses and will block ", ddutil::TEXT_COLOR) +
		ddutil::genericBlockAlert(blockAmount);
}

SamuraiMoves::Enlightenment::Enlightenment()
	:ZenMove(ZEN_AMOUNT, COST, "Enlightenment", Strength::Mythical)
{
}

SamuraiMoves::Eviscerate::Eviscerate()
	:SelfDamageAttackMove(DAMAGE, false, SELF_DAMAGE, COST, "Eviscerate", Strength::Mythical, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::DualingDragons::DualingDragons()
	:Move("Deals "+std::to_string(DAMAGE)+" damage "+std::to_string(TIMES)+" times and applies burn "+std::to_string(BURN_AMOUNT)+ " times",
		"Dualing Dragons", COST, Strength::Mythical, true, WavFile("dualattack", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString SamuraiMoves::DualingDragons::doAction(Creature* self, Creature* other)
{
	BurntStatus* newStatus = new BurntStatus();
	ColorString statusName = newStatus->getName();
	other->applyStatus(newStatus, BURN_AMOUNT);
	
	MultiAttackMove tempMove(DAMAGE, TIMES, 0, "TEMP", Strength::Mythical, sound);
	return tempMove.doAction(self, other);
}

SamuraiMoves::Caltrops::Caltrops()
	:SimpleStatusMove(new ThornsStatus(), THORNS_AMOUNT, true, COST, "Caltrops", Strength::Mythical,
		WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

SamuraiMoves::DragonForm::DragonForm()
	:Move("Applies a buff which increases damage by 50% for " + std::to_string(DURATION) + " turns", "Dragon Form", COST,
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
