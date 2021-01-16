#include "enemy_moves.h"
#include "utilities.h"
#include "creature.h"
#include "game.h"
#include "player.h"

#include <string>

EnemyMoves::Strike::Strike(int theDamage, WavFile theSound)
	:Move(MoveId::EnemyMoveId, "Deals " + std::to_string(theDamage) + " damage", "Strike", 0, Strength::Moderate, false, theSound)
{
	strength = theDamage;
}

ColorString EnemyMoves::Strike::doAction(Creature* self, Creature* other)
{
	ddutil::DamageReport damage = other->reduceHealth(strength, self);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + 
		ColorString(" deals ", ddutil::TEXT_COLOR) + 
		ddutil::genericDamageAlert(damage) +
		ColorString(" to the ", ddutil::TEXT_COLOR) + other->getColorString();
}

void EnemyMoves::Strike::increaseStrength(int amount)
{
	strength += amount;
}

int EnemyMoves::Strike::getStrength()
{
	return strength;
}

EnemyMoves::Block::Block(int theAmount, WavFile theSound)
	:Move(MoveId::EnemyMoveId, "Blocks " + std::to_string(theAmount) + " damage", "Guard", 0, Strength::Moderate, false, theSound)
{
	amount = theAmount;
}

ColorString EnemyMoves::Block::doAction(Creature* self, Creature* other)
{
	self->applyBlock(amount);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + 
		ColorString(" will block ", ddutil::TEXT_COLOR) + ddutil::genericBlockAlert(amount);
}

EnemyMoves::Burn::Burn(int theAmount)
	:SimpleStatusMove(MoveId::EnemyMoveId, new BurntStatus(), theAmount, false, 0, "Burn", Strength::Moderate, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

EnemyMoves::Frostburn::Frostburn(int theAmount)
	: SimpleStatusMove(MoveId::EnemyMoveId, new FrostBurntStatus(), theAmount, false, 0, "Frostburn", Strength::Moderate, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}


EnemyMoves::Invulnerability::Invulnerability(int theLength)
	:SimpleStatusMove(MoveId::EnemyMoveId, new InvulnerableStatus(), theLength, false, 0, "Invulnerability", Strength::Powerful, WavFile("ghostinvulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

EnemyMoves::StrikeAndBlock::StrikeAndBlock(int theDamage, int blockPerAttack, WavFile theSound)
	:AttackAndBlockMove(MoveId::EnemyMoveId, theDamage, blockPerAttack, false, 0, "Strike and Block", Strength::Moderate, theSound)
{
}

EnemyMoves::Vulnerable::Vulnerable(int length)
	:SimpleStatusMove(MoveId::EnemyMoveId, new VulnerableStatus(), length, true, 0, "Leer", Strength::Moderate, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

EnemyMoves::Sacrifice::Sacrifice(int theLength)
	:Move(MoveId::EnemyMoveId, "Makes all players vulnerable in exchange for half of  the enemy's HP", "Sacrifice", 0,
		Strength::Powerful, false, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
	length = theLength;
}

ColorString EnemyMoves::Sacrifice::doAction(Creature* self, Creature* other)
{
	self->reduceHealth(self->getHealth() / 2, self, true);

	Game* game = self->getGamePtr();
	std::vector<Creature*> targets;
	for (Player* p : game->getPlayerParty())
	{
		targets.push_back(p);
	}
	for (Creature* c : targets)
	{
		c->applyStatus(new VulnerableStatus(), length);
	}
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" loses half its health but makes everyone ", ddutil::TEXT_COLOR) +
		ColorString("Vulnerable", VulnerableStatus::COLOR);
}

EnemyMoves::Heal::Heal(int theAmount)
	:SimpleHealMove(MoveId::EnemyMoveId, theAmount, false, 0, "Heal", Strength::Moderate)
{
}

EnemyMoves::CreateShield::CreateShield(int block, WavFile sound)
	:Move(MoveId::EnemyMoveId, "Blocks X damage per turn", "Create Shield", 0, Strength::Powerful, false, sound)
{
	blockAmount = block;
}

ColorString EnemyMoves::CreateShield::doAction(Creature* self, Creature* other)
{
	self->increaseBaseBlock(blockAmount);
	self->resetBlock();
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" creates a shield which will ", ddutil::TEXT_COLOR) +
		ColorString("block " + std::to_string(blockAmount) + " damage ", ddutil::BLOCK_COLOR) +
		ColorString("every turn", ddutil::TEXT_COLOR);
}

EnemyMoves::DualStrike::DualStrike(int damage, WavFile theSound)
	:MultiAttackMove(MoveId::EnemyMoveId, damage, 2, 0, "Dual Strike", Strength::Weak, theSound)
{

}

EnemyMoves::BlockBreakStrike::BlockBreakStrike(int damage, int multiplier, WavFile theSound)
	:BlockBreakingMove(MoveId::EnemyMoveId, damage, multiplier, 0, "", Strength::Weak, theSound)
{
}

EnemyMoves::GetStatusImmunity::GetStatusImmunity(WavFile sound)
	:Move(MoveId::EnemyMoveId, "","",0,Strength::Mythical,false,sound)
{
}

ColorString EnemyMoves::GetStatusImmunity::doAction(Creature* self, Creature* other)
{
	self->setImmunityToStatuses(true);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" is now immune to status damage", ddutil::TEXT_COLOR);
}

EnemyMoves::CreateThorns::CreateThorns(int amount)
	:Move(MoveId::EnemyMoveId, "","",0, Strength::Moderate, false, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
	thornAmount = amount;
}

ColorString EnemyMoves::CreateThorns::doAction(Creature* self, Creature* other)
{
	self->applyStatus(new ThornsStatus(), thornAmount);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" coats itself in ", ddutil::TEXT_COLOR) +
		ColorString("Thorns", ThornsStatus::COLOR);
}

// pass through a negative number
EnemyMoves::TakeVitalityGain::TakeVitalityGain(int negAmount)
	:Move(MoveId::EnemyMoveId, "", "", 0, Strength::Weak, false, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
	amount = negAmount;
}

ColorString EnemyMoves::TakeVitalityGain::doAction(Creature* self, Creature* other)
{
	Player* p = dynamic_cast<Player*>(other);
	if (p == nullptr)
	{
		ddutil::errorMessage("TakeVitalityGain used on non player", __LINE__, __FILE__);
		return ColorString();
	}
	else
	{
		p->adjustVitalityGainTemp(amount);
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
			ColorString(" reduces The ", ddutil::TEXT_COLOR) + p->getColorString() + ColorString("'s ", ddutil::TEXT_COLOR) +
			ColorString("Vitality Gain", ddutil::VITALITY_COLOR) +
			ColorString(" by " + std::to_string(amount), ddutil::TEXT_COLOR);
	}
}

EnemyMoves::TakeVitality::TakeVitality(int theAmount)
	:Move(MoveId::EnemyMoveId, "", "", 0, Strength::Mythical, false, WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
	amount = theAmount;
}

ColorString EnemyMoves::TakeVitality::doAction(Creature* self, Creature* other)
{
	Player* p = dynamic_cast<Player*>(other);
	if (p == nullptr)
	{
		ddutil::errorMessage("TakeVitality used on non player", __LINE__, __FILE__);
		return ColorString();
	}
	else
	{
		int amountStolen = p->stealVitality(amount);
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
			ColorString(" reduces The ", ddutil::TEXT_COLOR) + p->getColorString() + ColorString("'s ", ddutil::TEXT_COLOR) +
			ColorString("Vitality ", ddutil::VITALITY_COLOR) +
			ColorString(" by " + std::to_string(amountStolen), ddutil::TEXT_COLOR);
	}
}

EnemyMoves::SuicideStrike::SuicideStrike(int theDamage, WavFile theSound)
	:EnemyMoves::Strike(theDamage, theSound)
{
}

ColorString EnemyMoves::SuicideStrike::doAction(Creature* self, Creature* other)
{
	self->reduceHealth(9999, nullptr, true);
	ColorString output = EnemyMoves::Strike::doAction(self, other);
	output += ColorString(" and dies in the process", ddutil::TEXT_COLOR);
	return output;
}

EnemyMoves::HealStrike::HealStrike(int theDamage, int theHeal, WavFile theSound)
	:Strike(theDamage, theSound)
{
	healAmount = theHeal;
}

ColorString EnemyMoves::HealStrike::doAction(Creature* self, Creature* other)
{
	int targetStartingHealth = other->getHealth();
	ColorString output = Strike::doAction(self, other);
	int healAmount = targetStartingHealth - other->getHealth();
	int netHeal = self->increaseHealth(healAmount);
	output += ColorString(" and heals ", ddutil::TEXT_COLOR) + ColorString(std::to_string(netHeal) + " HP", ddutil::HEAL_COLOR);
	return output;
}

EnemyMoves::StealMove::StealMove()
	:Move(MoveId::EnemyMoveId, "", "", 0, Strength::Mythical, true, WavFile("lightning", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString EnemyMoves::StealMove::doAction(Creature* self, Creature* other)
{
	other->removeNewestMove();
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" steals The ", ddutil::TEXT_COLOR) + other->getColorString() +
		ColorString("'s newest move!", ddutil::TEXT_COLOR);
}

EnemyMoves::AddBleedToAttacks::AddBleedToAttacks(int amount)
	:Move(
		MoveId::EnemyMoveId, 
		"", "", 0, Strength::Powerful, false, WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
	this->amount = amount;
}

ColorString EnemyMoves::AddBleedToAttacks::doAction(Creature* self, Creature* other)
{
	self->addAttackStatus(new BleedingStatus(), amount);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" will now apply ", ddutil::TEXT_COLOR) +
		ColorString(std::to_string(amount) + " Bleeding ", BleedingStatus::COLOR) +
		ColorString("per attack", ddutil::TEXT_COLOR);
}

EnemyMoves::AddScorchToAttacks::AddScorchToAttacks(int amount)
	:Move(
		MoveId::EnemyMoveId,
		"", "", 0, Strength::Powerful, false, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
	this->amount = amount;
}

ColorString EnemyMoves::AddScorchToAttacks::doAction(Creature* self, Creature* other)
{
	self->addAttackStatus(new ScorchedStatus(), amount);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" will now apply ", ddutil::TEXT_COLOR) +
		ColorString(std::to_string(amount) + " Scorched ", ScorchedStatus::COLOR) +
		ColorString("per attack", ddutil::TEXT_COLOR);
}

