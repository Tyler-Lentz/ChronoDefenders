#ifndef SORCERER_MOVES_H
#define SORCERER_MOVES_H

#include "move.h"
#include "colorstring.h"

#include <string>

class Status;
class Creature;
class Player;
class Game;

namespace SorcererMoves
{
	// Base classes for sorcerer specific types of moves

	// base class for a move which applys block to the target
	class MakeBarrierMove : public Move
	{
	public:
		MakeBarrierMove(MoveId id, int barrierAmount, int cost, std::string name, Strength str);
		ColorString doAction(Creature* self, Creature* other) override;

	private:
		int blockAmount;
	};

	class SummonMove : public Move
	{
	public:
		SummonMove(MoveId id, Player* minion, int cost, std::string name, Strength str, WavFile theSound);
		~SummonMove();

		ColorString doAction(Creature* self, Creature* other) override;
		const static int AURA = 3;
	private:
		Player* summon;
	};

	// Weak
	class MagicBarrier : public MakeBarrierMove
	{
	public:
		MagicBarrier();

		const static int BLOCK_AMOUNT = 7;
		const static int COST = 3;
	};

	class EnergyStrike : public SimpleAttackMove
	{
	public:
		EnergyStrike();

		const static int DAMAGE = 4;
		const static int COST = 3;
	};

	class Heal : public Move 
	{
	public:
		Heal();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int HEAL_AMOUNT = 5;
		const static int COST = 3;
		const static int AURA = 2;
	};

	// Moderate

	class ElectricShock : public StatusAttackMove
	{
	public:
		ElectricShock();

		const static int ZAPPED_LENGTH = 1;
		const static int DAMAGE = 4;
		const static int COST = 2;
	};

	class Levitate : public SelfBlockMove
	{
	public:
		Levitate();

		const static int BLOCK = 7;
		const static int COST = 2;
	};

	class BlinkStrike : public SimpleAttackMove
	{
	public:
		BlinkStrike();

		const static int DAMAGE = 4;
		const static int COST = 1;
	};

	class Fireball : public StatusAttackMove
	{
	public:
		Fireball();

		const static int DAMAGE = 5;
		const static int COST = 2;
		const static int BURN_TIMES = 1;
	};

	class IceOrb : public StatusAttackMove
	{
	public:
		IceOrb();

		const static int DAMAGE = 5;
		const static int COST = 2;
		const static int FBURN_TIMES = 1;
	};

	class Recover : public SimpleHealMove
	{
	public:
		Recover();

		const static int HEAL_AMOUNT = 10;
		const static int COST = 3;
	};

	class SummonFireImp : public SummonMove
	{
	public:
		SummonFireImp(Game* game);

		const static int COST = 10;
	};

	class SummonIceImp : public SummonMove
	{
	public:
		SummonIceImp(Game* game);

		const static int COST = 10;
	};

	class EnergyBarrier : public MakeBarrierMove
	{
	public:
		EnergyBarrier();

		const static int BLOCK_AMOUNT = 10;
		const static int COST = 3;
	};

	class CleansingTouch : public Move
	{
	public:
		CleansingTouch();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int HEAL_AMOUNT = 7;
		const static int COST = 3;
		const static int AURA = 1;
	};

	class EnergySword : public SimpleAttackMove
	{
	public:
		EnergySword();

		const static int DAMAGE = 10;
		const static int COST = 4;
	};

	class PlasmaStrike : public SimpleAttackMove
	{
	public:
		PlasmaStrike();

		const static int DAMAGE = 10;
		const static int COST = 3;
	};

	// Powerful

	class MirrorImage : public SelfBlockMove
	{
	public:
		MirrorImage();

		const static int BLOCK = 10;
		const static int COST = 2;
	};

	class FairySummon : public SummonMove
	{
	public:
		FairySummon(Game* game);

		const static int COST = 10;
	};

	class SummonFireDragon : public SummonMove
	{
	public:
		SummonFireDragon(Game* game);

		const static int COST = 10;
	};

	class SummonIceDragon : public SummonMove
	{
	public:
		SummonIceDragon(Game* game);
		
		const static int COST = 10;
	};

	class Blizzard : public Move
	{
	public:
		Blizzard();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int BASE_DAMAGE = 13;
		const static int MULTIPLIER = 2;
		const static int COST = 4;
	};

	class Cataclysm : public Move
	{
	public:
		Cataclysm();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int BASE_DAMAGE = 13;
		const static int MULTIPLIER = 2;
		const static int COST = 4;
	};

	class LightningStrike : public StatusAttackMove
	{
	public:
		LightningStrike();

		const static int BASE_DAMAGE = 18;
		const static int COST = 5;
		const static int ZAPPED_LENGTH = 4;
	};

	class PlasmaSword : public SimpleAttackMove
	{
	public:
		PlasmaSword();

		const static int DAMAGE = 14;
		const static int COST = 4;
	};

	class EtherealBarrier : public MakeBarrierMove
	{
	public:
		EtherealBarrier();

		const static int BLOCK_AMOUNT = 13;
		const static int COST = 3;
	};

	class StarStrike : public SimpleAttackMove
	{
	public:
		StarStrike();

		const static int DAMAGE = 14;
		const static int COST = 3;
	};

	class Combust : public Move
	{
	public:
		Combust();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int DAM_PER_BURN = 5;
		const static int COST = 5;
	};

	class Freeze : public Move
	{
	public:
		Freeze();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int DAM_PER_FBURN = 5;
		const static int COST = 5;
	};

	// Mythical

	class ChainLightning : public Move
	{
	public:
		ChainLightning();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int COST = 8;
	};

	class CleansingAura : public Move
	{
	public:
		CleansingAura();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int BASE_HEAL = 10;
		const static int BONUS_HEAL = 5;
		const static int COST = 4;
		const static int AURA = 1;
	};

	class EnergyAura : public Move
	{
	public:
		EnergyAura();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int VITALITY_GAIN = 2;
		const static int COST = 10;
		const static int AURA = 2;
	};

	class EtherealBlade : public SimpleAttackMove
	{
	public:
		EtherealBlade();

		const static int DAMAGE = 20;
		const static int COST = 4;
	};

	class SpiritCall : public SummonMove
	{
	public:
		SpiritCall(Game* game);

		const static int COST = 15;
	};

	class Supernova : public Move
	{
	public:
		Supernova();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int DAMAGE = 100;
		const static int COST = 8;
	};

	class CastInvulnerability : public Move
	{
	public:
		CastInvulnerability();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int COST = 10;
		const static int AURA = 2;
		const static int HEX = 2; // hex for 2 length
	};

	class ElementalForm: public Move
	{
	public:
		ElementalForm();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int COST = 10;
		const static int DURATION = 4;
	};

	class ElementalBarrier : public MakeBarrierMove 
	{
	public:
		ElementalBarrier();
		const static int COST = 3;
		const static int BLOCK = 16;
	};
}

#endif