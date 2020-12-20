#ifndef MINION_MOVES_H
#define MINION_MOVES_H

#include "move.h"
#include "colorstring.h"
#include "sorcerer_moves.h"

class Creature;

namespace MinionMoves
{
	class FairyDust : public SimpleHealMove
	{
	public:
		FairyDust();

		const static int HEAL_AMOUNT = 12;
		const static int COST = 4;
			
	};

	class FireBreath : public StatusAttackMove
	{
	public:
		FireBreath();

		const static int DAMAGE = 10;
		const static int COST = 2;
		const static int BURN_TIMES = 2;
	};

	class IceBreath : public StatusAttackMove
	{
	public:
		IceBreath();

		const static int DAMAGE = 10;
		const static int COST = 2;
		const static int FBURN_TIMES = 2;
	};

	class SpiritStrike : public SimpleAttackMove
	{
	public:
		SpiritStrike();

		const static int DAMAGE = 20;
		const static int COST = 2;
	};

	class SpiritProjection : public SorcererMoves::MakeBarrierMove
	{
	public:
		SpiritProjection();

		const static int BLOCK_AMOUNT = 15;
		const static int COST = 2;
	};
}

#endif