#ifndef ENEMY_MOVES_H
#define ENEMY_MOVES_H

#include "move.h"
#include "colorstring.h"

class Creature;

namespace EnemyMoves
{
	class Strike : public Move
	{
	public:
		Strike(int theDamage, WavFile theSound);
		ColorString doAction(Creature* self, Creature* other) override;
		void increaseStrength(int amount);
		int getStrength();
	private:
		int strength; // how much damage it deals
	};

	class SuicideStrike : public Strike 
	{
	public:
		SuicideStrike(int theDamage, WavFile theSound);
		ColorString doAction(Creature* self, Creature* other) override;
	};

	class HealStrike : public Strike
	{
	public:
		HealStrike(int theDamage, int theHeal, WavFile theSound);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int healAmount;
	};

	class DualStrike : public MultiAttackMove
	{
	public:
		DualStrike(int damage, WavFile theSound);
	};

	class BlockBreakStrike : public BlockBreakingMove
	{
	public:
		BlockBreakStrike(int damage, int multiplier, WavFile theSound);
	};

	class Block : public Move
	{
	public:
		Block(int theAmount, WavFile theSound);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int amount;
	};

	class StrikeAndBlock : public AttackAndBlockMove
	{
	public:
		StrikeAndBlock(int theDamage, int blockPerAttack, WavFile theSound);
	};

	class Burn : public SimpleStatusMove
	{
	public:
		Burn(int theAmount);
		
	};

	class Heal : public SimpleHealMove
	{
	public:
		Heal(int theAmount);
	};

	class Frostburn : public SimpleStatusMove
	{
	public:
		Frostburn(int theAmount);
	};

	class Invulnerability : public SimpleStatusMove
	{
	public:
		Invulnerability(int theLength);
	};

	class Sacrifice : public Move
	{
	public:
		Sacrifice(int vulnerableLength);

		ColorString doAction(Creature* self, Creature* other) override;

	private:
		int length;
	};

	class Vulnerable : public SimpleStatusMove
	{
	public:
		Vulnerable(int length);
	};

	// block X per turn
	class CreateShield : public Move
	{
	public:
		CreateShield(int block, WavFile sound);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int blockAmount;
	};

	class GetStatusImmunity : public Move
	{
	public:
		GetStatusImmunity(WavFile sound);
		ColorString doAction(Creature* self, Creature* other) override;
	};

	class CreateThorns : public Move
	{
	public:
		CreateThorns(int amount);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int thornAmount;
	};

	class AddBleedToAttacks : public Move
	{
	public:
		AddBleedToAttacks(int amount);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int amount;
	};

	class AddScorchToAttacks : public Move
	{
	public:
		AddScorchToAttacks(int amount);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int amount;
	};

	class TakeVitalityGain : public Move
	{
	public:
		TakeVitalityGain(int theAmount);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int amount;
	};

	class StealMove : public Move
	{
	public:
		StealMove();
		ColorString(doAction(Creature* self, Creature* other)) override;
	};

	class TakeVitality : public Move
	{
	public:
		TakeVitality(int theAmount);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int amount;
	};
}

#endif