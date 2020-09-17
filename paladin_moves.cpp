#include "paladin_moves.h"
#include "move.h"
#include "creature.h"
#include "colorstring.h"
#include "player.h"
#include "status.h"

#include <string>

PaladinMoves::Pound::Pound()
	:Move("Deals 5 damage", "Pound", 2, Strength::Weak, true)
{}

ColorString PaladinMoves::Pound::doAction(Creature* self, Creature* other)
{
	ddutil::DamageReport damage = other->reduceHealth(5, self);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" deals ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damage) +
		ColorString(" to the ", ddutil::TEXT_COLOR) + other->getColorString();
}


PaladinMoves::Shield::Shield()
	:Move("Blocks 5 damage", "Shield", 2, Strength::Weak, false)
{}

ColorString PaladinMoves::Shield::doAction(Creature* self, Creature* other)
{
	const int BLOCK_AMOUNT = 5;

	self->applyBlock(BLOCK_AMOUNT);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" will block ",ddutil::TEXT_COLOR) + ddutil::genericBlockAlert(BLOCK_AMOUNT);
}


PaladinMoves::Smite::Smite()
	:Move("Applys Off Balance for this turn", "Smite", 1, Strength::Moderate, true)
{}

ColorString PaladinMoves::Smite::doAction(Creature* self, Creature* other)
{
	Status* status = new OffBalanceStatus();
	ColorString statusName = status->getName();
	other->applyStatus(status);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + 
		ColorString(" makes the ", ddutil::TEXT_COLOR) + other->getColorString() + statusName;
}



PaladinMoves::Miracle::Miracle()
	:Move("If health is less than 25%, gain 2 vitality", "Miracle", 1, Strength::Moderate, false)
{}

ColorString PaladinMoves::Miracle::doAction(Creature* self, Creature* other)
{
	Player* player = dynamic_cast<Player*>(self);
	if (player->getHealth() <= player->getMaxHealth(25))
	{
		player->addVitality(2);
		ColorString s1 = ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" gains ", ddutil::TEXT_COLOR);
		ColorString s2 = ColorString("2 vitality.", ddutil::VITALITY_COLOR);
		return s1 + s2;
	}
	else
	{
		player->addVitality(1); // refund the vitality
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString("'s health is not below 25%", ddutil::TEXT_COLOR);
	}
}
