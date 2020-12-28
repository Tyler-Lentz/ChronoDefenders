#ifndef SAMURAI_MOVES_H
#define SAMURAI_MOVES_H

#include "move.h"
#include "colorstring.h"
#include "player.h"

class Creature;

namespace SamuraiMoves
{
	// Base
	

	class ZenMove : public Move
	{
	public:
		ZenMove(MoveId id, int zenLevel, int cost, std::string name, Strength str);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int zenAmount;
	};

	class StormMove : public Move
	{
	public:
		StormMove(MoveId id, int targetStorm, int otherStorm, bool chooseTarget, int cost, std::string name, Strength str, WavFile theSound);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int targetStormLevel; // how much storm the target receives (if there is a target)
		int nonTargetStormLevel; // how much storm the other people receive (everyone if there is no target)
	};

	// attacks that do "multiplier" times more damage when vitality is at a specific number
	class VitalityBoostedAttack : public Move
	{
	public:
		VitalityBoostedAttack(MoveId id, int theBaseDamage, int theMultplier, int theVitalityAmount, std::string name, int cost, Strength str, WavFile theSound);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int baseDamage;
		int multiplier;
		int vitalityAmount; // what vitality needs to be in order for the multiplier to trigger
	};

	// Weak
	class Slice : public SimpleAttackMove
	{
	public:
		Slice();

		const static int DAMAGE = 4;
		const static int COST = 1;
	};

	class Deflect : public SelfBlockMove
	{
	public:
		Deflect();

		const static int BLOCK = 4;
		const static int COST = 1;
	};

	// Moderate

	class ThrowingKnives : public StatusAttackMove
	{
	public:
		ThrowingKnives();

		const static int DAMAGE = 3;
		const static int BLEED_AMOUNT = 3;
		const static int COST = 1;
	};

	class PerilousStrike : public SelfDamageAttackMove
	{
	public:
		PerilousStrike();

		const static int DAMAGE = 12;
		const static int SELF_DAMAGE = 4;
		const static int COST = 2;
	};

	class CalculatedSlice : public AttackAndBlockMove
	{
	public:
		CalculatedSlice();

		const static int DAMAGE = 4;
		const static int BLOCK = 4;
		const static int COST = 1;
	};

	class Sidestep : public SelfBlockMove
	{
	public:
		Sidestep();

		const static int BLOCK_AMOUNT = 9;
		const static int COST = 1;
	};

	class Meditation : public ZenMove
	{
	public:
		Meditation();

		const static int ZEN_AMOUNT = 3;
		const static int COST = 3;
	};

	class Storm : public StormMove
	{
	public:
		Storm();

		const static int STORM_LEVEL = 1;
		const static int COST = 2;
	};

	class LightningRod : public Move
	{
	public:
		LightningRod();

		ColorString doAction(Creature* self, Creature* other) override;

		const static int VITALITY_GAIN = 3;
		const static int COST = 0;;
	};

	class Assault : public SimpleAttackMove
	{
	public:
		Assault();

		const static int DAMAGE = 20;
		const static int COST = 4;
	};

	class DualSlice : public MultiAttackMove
	{
	public:
		DualSlice();

		const static int DAMAGE = 4;
		const static int TIMES = 2;
		const static int COST = 1;
	};

	class BodySlam : public BlockBreakingMove
	{
	public:
		BodySlam();

		const static int DAMAGE = 10;
		const static int MULTIPLIER = 2;
		const static int COST = 2;
	};

	// Powerful

	class Shuriken: public StatusAttackMove
	{
	public:
		Shuriken();

		const static int DAMAGE = 8;
		const static int BLEED_AMOUNT = 4;
		const static int COST = 1;
	};

	class Typhoon : public StormMove
	{
	public:
		Typhoon();

		const static int TARGET_STORM_LEVEL = 2;
		const static int OTHER_STORM_LEVEL = 1;
		const static int COST = 2;
	};

	class DragonSlice : public StatusAttackMove
	{
	public:
		DragonSlice();

		const static int DAMAGE = 9;
		const static int BURN_AMOUNT = 1;
		const static int COST = 1;
	};

	class PerilousPlunge : public SelfDamageAttackMove
	{
	public:
		PerilousPlunge();

		const static int DAMAGE = 20;
		const static int SELF_DAMAGE = 5;
		const static int COST = 2;
	};

	class Zen : public ZenMove
	{
	public:
		Zen();

		const static int ZEN_LEVEL = 4;
		const static int COST = 4;
	};

	class ChargedAssault : public VitalityBoostedAttack
	{
	public:
		ChargedAssault();

		const static int DAMAGE = 20;
		const static int MULTIPLIER = 2;
		const static int COST = 4;
		const static int VITALITY_AMOUNT = Samurai::MAX_VITALITY - COST;
	};

	// deals DAMAGE damage when health is below HEALTH_PERCENTAGE% of max hp
	class Desperation : public Move
	{
	public:
		Desperation();
		ColorString doAction(Creature* self, Creature* other);

		const static int DAMAGE = 20;
		const static int HEALTH_PERCENTAGE = 33;
		const static int COST = 1;
	};

	class CrushingBlow : public BlockBreakingMove
	{
	public:
		CrushingBlow();

		const static int DAMAGE = 13;
		const static int MULTIPLIER = 2;
		const static int COST = 2;
	};

	class CriticalStrike : public AttackAndBlockMove
	{
	public:
		CriticalStrike();

		const static int DAMAGE = 8;
		const static int BLOCK = 8;
		const static int COST = 1;
	};


	class FinishingBlow : public VitalityBoostedAttack
	{
	public:
		FinishingBlow();

		const static int DAMAGE = 7;
		const static int MULTIPLIER = 2;
		const static int COST = 1;
		const static int VITALITY_AMOUNT = 0;
	};

	class EyeOfTheStorm : public Move
	{
	public:
		EyeOfTheStorm();

		ColorString doAction(Creature* self, Creature* other) override;

		const static int BLOCK = 12;
		const static int MULTIPLIER = 2;
		const static int COST = 2;
	};

	class ShadowStep : public Move
	{
	public:
		ShadowStep();

		ColorString doAction(Creature* self, Creature* other) override;
		const static int BLOCK = 15;
		const static int COST = 2;
		const static int HEXED_LENGTH = 2;
	};

	// Mythical

	class DragonForm : public Move
	{
	public:
		DragonForm();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int DURATION = 4;
		const static int COST = 8;
	};

	class Caltrops : public SimpleStatusMove
	{
	public:
		Caltrops();

		const static int THORNS_AMOUNT = 5;
		const static int COST = 1;
	};

	class TippedKunai : public StatusAttackMove 
	{
	public:
		TippedKunai();

		const static int DAMAGE = 8;
		const static int POISON_AMOUNT = 3;
		const static int COST = 1;
	};

	class BlindingFury : public SimpleAttackMove
	{
	public:
		BlindingFury();

		const static int DAMAGE = 15;
		const static int COST = 1;
	};

	class Tsunami : public Move
	{
	public:
		Tsunami();

		ColorString doAction(Creature* self, Creature* other) override;

		const static int DAMAGE = 30;
		const static int COST = 3;
		const static int VITALITY_MULTIPLIER = 2;
		const static int VITALITY_LEVEL = Samurai::MAX_VITALITY - COST;
		const static int STORM_MULTIPLIER = 2;
	};

	class LeavesFromTheVine : public Move
	{
	public:
		LeavesFromTheVine();

		ColorString doAction(Creature* self, Creature* other) override;

		const static int BLOCK_PER_STATUS = 10;
		const static int COST = 3;
	};

	class Enlightenment : public ZenMove
	{
	public:
		Enlightenment();

		const static int ZEN_AMOUNT = 5;
		const static int COST = 4;
	};

	class Eviscerate : public SelfDamageAttackMove
	{
	public:
		Eviscerate();

		const static int DAMAGE = 28;
		const static int SELF_DAMAGE = 6;
		const static int COST = 2;
	};

	class DualingDragons : public Move
	{
	public:
		DualingDragons();

		ColorString doAction(Creature* self, Creature* other);

		const static int DAMAGE = 5;
		const static int TIMES = 2;
		const static int BURN_AMOUNT = 2;
		const static int COST = 1;
	};

	class DragonBreath : public StatusAttackMove
	{
	public:
		DragonBreath();
		
		const static int DAMAGE = 10;
		const static int COST = 1;
		const static int SCORCH_LENGTH = 1;
	};

	class Tornado : public SimpleStatusMove
	{
	public:
		Tornado();

		const static int STORM_LENGTH = 2;
		const static int COST = 2;
	};

	class ShinobiTactics : public Move
	{
	public:
		ShinobiTactics();
		ColorString doAction(Creature* self, Creature* other);
		const static int BLOCK = 12;
		const static int COST = 1;
		const static int BLEED_AMOUNT = 9;
	};
}

#endif 