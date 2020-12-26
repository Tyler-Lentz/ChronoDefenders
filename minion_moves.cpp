#include "minion_moves.h"
#include "move.h"
#include "colorstring.h"
#include "creature.h"
#include "utilities.h"
#include "sorcerer_moves.h"

MinionMoves::FairyDust::FairyDust()
	:SimpleHealMove(MoveId::MinionMoveId, HEAL_AMOUNT, true, COST, "Fairy Dust", Strength::Powerful)
{
}

MinionMoves::FireBreath::FireBreath()
	:StatusAttackMove(MoveId::MinionMoveId, DAMAGE, new BurntStatus(), BURN_TIMES, COST, "Fire Breath", Strength::Powerful, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

MinionMoves::IceBreath::IceBreath()
	:StatusAttackMove(MoveId::MinionMoveId, DAMAGE, new FrostBurntStatus(), FBURN_TIMES, COST, "Ice Breath", Strength::Powerful, WavFile("burn", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

MinionMoves::SpiritStrike::SpiritStrike()
	:SimpleAttackMove(MoveId::MinionMoveId, DAMAGE, false, COST, "Spirit Strike", Strength::Mythical, WavFile("attack4", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

MinionMoves::SpiritProjection::SpiritProjection()
	: MakeBarrierMove(MoveId::MinionMoveId, BLOCK_AMOUNT, COST, "Spirit Projection", Strength::Mythical)
{
}
