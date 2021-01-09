#ifndef GUNSLINGER_MOVES_H
#define GUNSLINGER_MOVES_H

#include "move.h"
#include "colorstring.h"
#include "utilities.h"
#include "player.h"

#include <string>

class Creature;

namespace GunslingerMoves
{
	// for use with the base card draw class
	enum class Suit
	{
		Spade,
		Heart,
		Club,
		Diamond
	};

	std::string suitToString(Suit suit);

	// Base moves for gunslinger specific types of moves
	class GunMove : public Move
	{
	public:
		GunMove(MoveId id, int damage, int bulletAmount, int cost, std::string name, Strength str, WavFile theSound);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int damage;
		int bulletAmount;
	};

	class MakeBulletMove : public Move
	{
	public:
		MakeBulletMove(MoveId id, int bulletAmount, int cost, std::string name, Strength str, WavFile sound = WavFile("craft", ddutil::SF_LOOP, ddutil::SF_ASYNC));
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int bulletAmount;
	};

	class ThrowDynamiteMove : public Move
	{
	public:
		ThrowDynamiteMove(MoveId id, int turnsToWait, int cost, std::string name, Strength str);
		ColorString doAction(Creature* self, Creature* other) override;
	private:
		int explosionTime;
	};

	class CardDrawMove : public Move
	{
	public:
		CardDrawMove(MoveId id, int minDraw, int maxDraw, Suit theSuit, int cost, std::string name, Strength str);

		ColorString doAction(Creature* self, Creature* other);
	private:
		Suit suit;
		int minDraw;
		int maxDraw;
	};

	class UniqueCardDrawMove : public Move
	{
	public:
		UniqueCardDrawMove(MoveId id, Status* card, int cost, Strength str, WavFile sound);
		~UniqueCardDrawMove();

		ColorString doAction(Creature* self, Creature* other) override;
	private:
		Status* status;
	};

	// Weak 
	class Peashooter : public GunMove
	{
	public:
		Peashooter();

		const static int DAMAGE = 3;
		const static int BULLET_AMOUNT = 1;
		const static int COST = 0;
	};

	class Dodge : public SelfBlockMove
	{
	public:
		Dodge();

		const static int BLOCK_AMOUNT = 5;
		const static int COST = 2;
	};

	class MakeBullets : public MakeBulletMove
	{
	public:
		MakeBullets();
		const static int BULLET_AMOUNT = 4;
		const static int COST = 4;
	};

	// Moderate
	
	class Dropkick : public SimpleAttackMove
	{
	public:
		Dropkick();

		const static int COST = 2;
		const static int DAMAGE = 10;
	};

	class BottleStrike : public SimpleStatusMove
	{
	public:
		BottleStrike();

		const static int VULN_LENGTH = 1;
		const static int COST = 1;
	};

	class Backflip : public SelfBlockMove
	{
	public:
		Backflip();

		const static int BLOCK_AMOUNT = 8;
		const static int COST = 2;
	};

	class Punch : public SimpleAttackMove
	{
	public:
		Punch();

		const static int DAMAGE = 6;
		const static int COST = 1;
	};

	class DrawSpade : public CardDrawMove
	{
	public:
		DrawSpade();

		const static int MIN_DRAW = 2;
		const static int MAX_DRAW = 10;
		const static int COST = 1;
	};

	class DrawHeart : public CardDrawMove
	{
	public:
		DrawHeart();

		const static int MIN_DRAW = 2;
		const static int MAX_DRAW = 10;
		const static int COST = 1;
	};

	class DrawDiamond : public CardDrawMove
	{
	public:
		DrawDiamond();

		const static int MIN_DRAW = 2;
		const static int MAX_DRAW = 10;
		const static int COST = 1;
	};

	class DrawClub : public CardDrawMove
	{
	public:
		DrawClub();

		const static int MIN_DRAW = 2;
		const static int MAX_DRAW = 10;
		const static int COST = 1;
	};

	class CardThrow : public Move
	{
	public:
		CardThrow();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int COST = 1;
	};

	class Revolver : public GunMove
	{
	public:
		Revolver();

		const static int DAMAGE = 6;
		const static int BULLET_AMOUNT = 1;
		const static int COST = 0;
	};

	class ThrowDynamite : public ThrowDynamiteMove
	{
	public:
		ThrowDynamite();

		const static int EXPLOSION_TIME = 3;
		const static int COST = 4;
	};

	class CraftBullets : public MakeBulletMove
	{
	public:
		CraftBullets();

		const static int BULLET_AMOUNT = 5;
		const static int COST = 4;
	};

	class TheMagician : public UniqueCardDrawMove 
	{
	public:
		TheMagician();

		const static int COST = 2;
	};

	class BowAndArrow : public SimpleStatusMove 
	{
	public:
		BowAndArrow();
		const static int COST = 1;
		const static int BLEED = 6;
	};

	// Powerful

	class DoubleKick : public MultiAttackMove
	{
	public:
		DoubleKick();

		const static int KICK_DAMAGE = 8;
		const static int KICK_TIMES = 2;
		const static int COST = 2;
	};

	class HeavyRevolver : public GunMove
	{
	public:
		HeavyRevolver();

		const static int DAMAGE = 9;
		const static int COST = 0;
		const static int BULLET_USE = 1;
	};

	class ExplosiveShot : public Move
	{
	public:
		ExplosiveShot();

		ColorString doAction(Creature* self, Creature* other) override;

		const static int COST = 1;
		const static int BULLET_USE = 1;
	};

	class Leap : public SelfBlockMove 
	{
	public:
		Leap();

		const static int BLOCK_AMOUNT = 11;
		const static int COST = 2;
	};

	class Uppercut : public StatusAttackMove
	{
	public:
		Uppercut();

		const static int DAMAGE = 7;
		const static int VULN_LENGTH = 1;
		const static int COST = 1;
	};

	class PumpShotgun : public GunMove
	{
	public:
		PumpShotgun();

		const static int DAMAGE = 30;
		const static int BULLET_AMOUNT = 4;
		const static int COST = 2;
	};

	
	class ForgeBullets : public MakeBulletMove
	{
	public:
		ForgeBullets();

		const static int BULLET_AMOUNT = 5;
		const static int COST = 3;
	};

	class QuickDynamite : public ThrowDynamiteMove
	{
	public:
		QuickDynamite();

		const static int EXPLOSION_TIME = 2;
		const static int COST = 4;
	};

	class DrinkWhiskey : public Move
	{
	public:
		DrinkWhiskey();
		ColorString doAction(Creature* self, Creature* other);
		
		const static unsigned int VITALITY_GAIN = 6;
		const static int VITALITY_PER_TURN_LOSS = -1;
		const static int COST = 0;
	};

	class CardFlurry : public Move
	{
	public:
		CardFlurry();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int COST = 1;
	};

	// Mythical

	class SupressiveFire : public Move
	{
	public:
		SupressiveFire();

		ColorString doAction(Creature* self, Creature* other) override;

		const static int DODGE_PER_BULLET = 15;
		const static int COST = 2;
	};

	class SuckerPunch : public StatusAttackMove
	{
	public:
		SuckerPunch();

		const static int DAMAGE = 9;
		const static int VULN_LENGTH = 2;
		const static int COST = 1;
	};

	class ConjureBullets : public MakeBulletMove
	{
	public:
		ConjureBullets();

		const static int BULLET_AMOUNT = 6;
		const static int COST = 2;
	};

	class SharpsRifle : public GunMove
	{
	public:
		SharpsRifle();

		const static int DAMAGE = 25;
		const static int BULLET_AMOUNT = 1;
		const static int COST = 2;
	};

	class ColtWalker : public GunMove
	{
	public:
		ColtWalker();

		const static int DAMAGE = 12;
		const static int BULLET_AMOUNT = 1;
		const static int COST = 0;
	};

	class FaceOfSpades : public CardDrawMove
	{
	public:
		FaceOfSpades();

		const static int MIN_DRAW = 11;
		const static int MAX_DRAW = 14;
		const static int COST = 1;
	};

	class FaceOfHearts : public CardDrawMove
	{
	public:
		FaceOfHearts();

		const static int MIN_DRAW = 11;
		const static int MAX_DRAW = 14;
		const static int COST = 1;
	};

	class FaceOfClubs : public CardDrawMove
	{
	public:
		FaceOfClubs();

		const static int MIN_DRAW = 11;
		const static int MAX_DRAW = 14;
		const static int COST = 1;
	};

	class FaceOfDiamonds : public CardDrawMove
	{
	public:
		FaceOfDiamonds();

		const static int MIN_DRAW = 11;
		const static int MAX_DRAW = 14;
		const static int COST = 1;
	};

	class BlackDynamite : public ThrowDynamiteMove
	{
	public:
		BlackDynamite();

		const static int EXPLOSION_TIME = 1;
		const static int COST = 4;
	};

	class BrassKnuckles : public SimpleAttackMove
	{
	public:
		BrassKnuckles();

		const static int DAMAGE = 12;
		const static int COST = 1;
	};

	class JesterForm : public Move
	{
	public:
		JesterForm();
		ColorString doAction(Creature* self, Creature* other) override;

		const static int COST = 6;
		const static int DURATION = 4;
	};
}

#endif