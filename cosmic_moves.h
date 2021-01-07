#ifndef COSMIC_MOVES
#define COSMIC_MOVES

#include "sorcerer_moves.h"
#include "move.h"
#include "utilities.h"
#include "colorstring.h"
#include "soundfile.h"
class Creature;

namespace CosmicMoves
{
	std::vector<Move*> getRandomMoves(int movesToChooseFrom);

	class BlackHole : public SimpleAttackMove
	{
	public:
		BlackHole();

		const static int COST = 10;
		const static int DAMAGE = 30;
	};

	class StarRegen : public SimpleHealMove
	{
	public:
		StarRegen();

		const static int HEAL = 14;
		const static int COST = 15;
	};

	class WarpBarrier : public SorcererMoves::MakeBarrierMove
	{
	public:
		WarpBarrier();

		const static int BLOCK = 18;
		const static int COST = 10;
	};

	class Disturbance : public Move
	{
	public:
		Disturbance();	
		ColorString doAction(Creature* self, Creature* other) override;

		const static int COST = 0;
		const static int DAM_PER_DISTORTION = 5;
	};

	class Deletion : public Move 
	{
	public:
		Deletion();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int COST = 10;
		const static int HP_LIM = 50; // if 50 hp or less, set hp to zero 
	};

	class CosmicBeam : public Move
	{
	public:
		CosmicBeam();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int DAM_PER_VIT = 1;
		const static int COST = 0;
	};

	class StarDust : public Move
	{
	public:
		StarDust();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int COST = 15;
		const static int VIT_PER_STAT = 2;
		// removes all the status effects on the target, and gets 2 vitality per status stack
	};

	class EternalTomb : public SimpleStatusMove
	{
	public:
		EternalTomb();
		
		static const int AMOUNT = 1; // entombed status is not dependent on count, deals 1,2,4,8,16,32,64,128
		static const int COST = 10;
	};

	class SeeingEye : public SimpleStatusMove
	{
	public:
		SeeingEye();
		static const int AMOUNT = 3;
		static const int COST = 15;
	};
}


#endif