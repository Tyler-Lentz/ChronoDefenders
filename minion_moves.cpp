#include "minion_moves.h"
#include "move.h"
#include "colorstring.h"
#include "creature.h"
#include "utilities.h"
#include "sorcerer_moves.h"

MinionMoves::FairyDust::FairyDust()
	:SimpleHealMove(HEAL_AMOUNT, true, COST, "Fairy Dust", Strength::Powerful)
{
}

MinionMoves::FireBreath::FireBreath()
	:StatusAttackMove(DAMAGE, new BurntStatus(), BURN_TIMES, COST, "Fire Breath", Strength::Powerful, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

MinionMoves::IceBreath::IceBreath()
	:StatusAttackMove(DAMAGE, new FrostBurntStatus(), FBURN_TIMES, COST, "Ice Breath", Strength::Powerful, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

MinionMoves::SpiritStrike::SpiritStrike()
	:SimpleAttackMove(DAMAGE, false, COST, "Spirit Strike", Strength::Mythical, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

MinionMoves::SpiritProjection::SpiritProjection()
	:MakeBarrierMove(BLOCK_AMOUNT, COST, "Spirit Projection", Strength::Mythical)
{
}
