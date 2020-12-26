#include "gunslinger_moves.h"
#include "move.h"
#include "colorstring.h"
#include "creature.h"
#include "utilities.h"
#include "player.h"


std::string GunslingerMoves::suitToString(Suit suit)
{
	switch (suit)
	{
	case Suit::Spade:
		return "Spade (Damage)";
	case Suit::Club:
		return "Club (Damage)";
	case Suit::Diamond:
		return "Diamond (EXP)";
	case Suit::Heart:
		return "Heart (Health)";
	default:
		return "BAD CARD GENERATION";
	}
}


// Base

GunslingerMoves::GunMove::GunMove(MoveId id, int dam, int bulletAmt, int cost, std::string name, Strength str, WavFile theSound)
	:Move(id, "Deals " + std::to_string(dam) + " damage with " + std::to_string(bulletAmt) +
		((bulletAmt > 1) ? " bullets" : " bullet"),
		name, cost, str, true, theSound)
{
	damage = dam;
	bulletAmount = bulletAmt;
}

ColorString GunslingerMoves::GunMove::doAction(Creature* self, Creature* other)
{
	Gunslinger* player = dynamic_cast<Gunslinger*>(self);

	if (player == nullptr)
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" does not have bullets", ddutil::TEXT_COLOR);
	}

	if (player->useBullet(bulletAmount))
	{
		ddutil::DamageReport dam = other->reduceHealth(damage, self);
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" deals ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(dam)
			+ ColorString(" to The ", ddutil::TEXT_COLOR) + other->getColorString();
	}
	else
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" does not have enough bullets", ddutil::TEXT_COLOR);
	}
}


GunslingerMoves::MakeBulletMove::MakeBulletMove(MoveId id, int bulletAmt, int cost, std::string name, Strength str, WavFile sound)
	:Move(id, "Creates " + std::to_string(bulletAmt) + ((bulletAmt > 1) ? " bullets" : " bullet"), name, cost, str, false, sound)
{
	bulletAmount = bulletAmt;
}

ColorString GunslingerMoves::MakeBulletMove::doAction(Creature* self, Creature* other)
{
	Gunslinger* player = dynamic_cast<Gunslinger*>(self);
	if (player == nullptr)
	{
		return ColorString("ERROR: This creature should not have this move", ddutil::TEXT_COLOR);
	}
	player->createBullets(bulletAmount);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" gets " + std::to_string(bulletAmount) + " new bullets", ddutil::TEXT_COLOR);
}

GunslingerMoves::ThrowDynamiteMove::ThrowDynamiteMove(MoveId id, int turnsToWait, int cost, std::string name, Strength str)
	:Move(id, "Throws dynamite which explodes for " + std::to_string(Dynamite::DAMAGE) + " damage in " + std::to_string(turnsToWait) + " turns",
		name, cost, str, true, WavFile("bombcombined", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
	explosionTime = turnsToWait;
}

ColorString GunslingerMoves::ThrowDynamiteMove::doAction(Creature* self, Creature* other)
{
	Status* status = new Dynamite();
	ColorString statusName = status->getName();
	other->applyStatus(status, explosionTime);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" throws ", ddutil::TEXT_COLOR) + statusName + ColorString(" at the ", ddutil::TEXT_COLOR) +
		other->getColorString();
}

GunslingerMoves::CardDrawMove::CardDrawMove(MoveId id, int theMinDraw, int theMaxDraw, Suit theSuit, int cost, std::string name, Strength str)
	:Move(id, "Draws a " + suitToString(theSuit) + " from " + ddutil::cardNumToString(theMinDraw) + " to " + ddutil::cardNumToString(theMaxDraw),
		name, cost, str, false, WavFile("drawcard", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
	suit = theSuit;
	minDraw = theMinDraw;
	maxDraw = theMaxDraw;
}

ColorString GunslingerMoves::CardDrawMove::doAction(Creature* self, Creature* other)
{
	CardStatus* card = nullptr;
	switch (suit)
	{
	case Suit::Spade:
		card = new SpadeStatus(ddutil::random(minDraw, maxDraw));
		break;
	case Suit::Club:
		card = new ClubStatus(ddutil::random(minDraw, maxDraw));
		break;
	case Suit::Diamond:
		card = new DiamondStatus(ddutil::random(minDraw, maxDraw));
		break;
	case Suit::Heart:
		card = new HeartStatus(ddutil::random(minDraw, maxDraw));
		break;
	}

	if (card == nullptr)
	{
		return ColorString("ERROR: bugged card generation", ddutil::TEXT_COLOR);
	}

	ColorString cardName = card->getName();
	self->applyStatus(card);

	if (self->hasStatus(StatusID::Jester))
	{
		Status* dupedCard = card->makeCopy();
		self->applyStatus(dupedCard);
	}

	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" draws the ", ddutil::TEXT_COLOR) + cardName;
}


// Weak

GunslingerMoves::Peashooter::Peashooter()
	:GunMove(MoveId::GunslingerPeashooter, DAMAGE, BULLET_AMOUNT, COST, "Peashooter", Strength::Weak, WavFile("shot1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{}


GunslingerMoves::Dodge::Dodge()
	:SelfBlockMove(MoveId::GunslingerDodge, BLOCK_AMOUNT, COST, "Dodge", Strength::Weak, WavFile("jump", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::MakeBullets::MakeBullets()
	:MakeBulletMove(MoveId::GunslingerMakeBullets, BULLET_AMOUNT, COST, "Make Bullets", Strength::Weak)
{
}

// Moderate

GunslingerMoves::Dropkick::Dropkick()
	:SimpleAttackMove(MoveId::GunslingerDropkick, DAMAGE, false, COST, "Dropkick", Strength::Moderate, WavFile("attack2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::Backflip::Backflip()
	:SelfBlockMove(MoveId::GunslingerBackflip, BLOCK_AMOUNT, COST, "Backflip", Strength::Moderate, WavFile("jump", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::Punch::Punch()
	:SimpleAttackMove(MoveId::GunslingerPunch, DAMAGE, false, COST, "Punch", Strength::Moderate, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::DrawSpade::DrawSpade()
	:CardDrawMove(MoveId::GunslingerDrawSpade, MIN_DRAW, MAX_DRAW, Suit::Spade, COST, "Draw Spade", Strength::Moderate)
{
}


GunslingerMoves::DrawHeart::DrawHeart()
	: CardDrawMove(MoveId::GunslingerDrawHeart, MIN_DRAW, MAX_DRAW, Suit::Heart, COST, "Draw Heart", Strength::Moderate)
{
}

GunslingerMoves::DrawClub::DrawClub()
	: CardDrawMove(MoveId::GunslingerDrawClub,MIN_DRAW, MAX_DRAW, Suit::Club, COST, "Draw Club", Strength::Moderate)
{
}

GunslingerMoves::DrawDiamond::DrawDiamond()
	: CardDrawMove(MoveId::GunslingerDrawDiamond, MIN_DRAW, MAX_DRAW, Suit::Diamond, COST, "Draw Diamond", Strength::Moderate)
{
}


GunslingerMoves::CardThrow::CardThrow()
	:Move(MoveId::GunslingerCardThrow, "Gives leftmost card effect to the target", "Card Throw", COST, Strength::Moderate, true, WavFile("cardthrow", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString GunslingerMoves::CardThrow::doAction(Creature* self, Creature* other)
{
	ColorString info = self->throwCard(other);
	if (info.getString() == "") // no cards to throw
	{
		Player* player = dynamic_cast<Player*>(self);
		if (player != nullptr)
		{
			player->addVitality(getCost());
		}
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + 
			ColorString(" has no cards to throw", ddutil::TEXT_COLOR);
	}
	else
	{
		return info;
	}
}

GunslingerMoves::Revolver::Revolver()
	:GunMove(MoveId::GunslingerRevolver, DAMAGE, BULLET_AMOUNT, COST, "Revolver", Strength::Moderate, WavFile("shot2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

// doesnt derive from simple status move because needs a special message specific to throwing dynamite
GunslingerMoves::ThrowDynamite::ThrowDynamite()
	:ThrowDynamiteMove(MoveId::GunslingerThrowDynamite, EXPLOSION_TIME, COST, "Throw Dynamite", Strength::Moderate)
{
}

GunslingerMoves::CraftBullets::CraftBullets()
	:MakeBulletMove(MoveId::GunslingerCraftBullets, BULLET_AMOUNT, COST, "Craft Bullets", Strength::Moderate)
{
}

GunslingerMoves::BottleStrike::BottleStrike()
	:SimpleStatusMove(MoveId::GunslingerBottleStrike,
		new VulnerableStatus(), VULN_LENGTH, true, COST, "Bottle Strike",
		Strength::Moderate, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}


// Powerful



GunslingerMoves::Leap::Leap()
	:SelfBlockMove(MoveId::GunslingerLeap, BLOCK_AMOUNT, COST, "Leap", Strength::Powerful, WavFile("jump", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}


GunslingerMoves::PumpShotgun::PumpShotgun()
	:GunMove(MoveId::GunslingerPumpShotgun,DAMAGE, BULLET_AMOUNT, COST, "Pump Shotgun", Strength::Powerful, WavFile("shotgun", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::ForgeBullets::ForgeBullets()
	:MakeBulletMove(MoveId::GunslingerForgeBullets, BULLET_AMOUNT, COST, "Forge Bullets", Strength::Powerful)
{
	
}

GunslingerMoves::QuickDynamite::QuickDynamite()
	:ThrowDynamiteMove(MoveId::GunslingerQuickDynamite, EXPLOSION_TIME, COST, "Quick Dynamite", Strength::Powerful)
{
}

GunslingerMoves::DrinkWhiskey::DrinkWhiskey()
	:Move(MoveId::GunslingerDrinkWhiskey, "Gain "+std::to_string(VITALITY_GAIN)+" vitality but lose "+std::to_string(VITALITY_PER_TURN_LOSS)+" vitality at the beginning of every turn",
		"Drink Whiskey", COST, Strength::Powerful, false, WavFile("drink", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString GunslingerMoves::DrinkWhiskey::doAction(Creature* self, Creature* other)
{
	Player* player = dynamic_cast<Player*>(self);
	if (player == nullptr)
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" can't gain vitality", ddutil::TEXT_COLOR);
	}


	if (player->getVitalityGain() <= 0)
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + 
			ColorString(" cannot gain any more vitality from whiskey", ddutil::TEXT_COLOR);
	}

	int startingVitality = player->getVitality();
	player->adjustVitalityGainTemp(VITALITY_PER_TURN_LOSS);
	player->addVitality(VITALITY_GAIN);
	int endingVitality = player->getVitality();
	int actualChange = endingVitality - startingVitality;

	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" gains ", ddutil::TEXT_COLOR) +
		ColorString(std::to_string(actualChange) + " vitality ", ddutil::VITALITY_COLOR);
	
}

GunslingerMoves::CardFlurry::CardFlurry()
	:Move(MoveId::GunslingerCardFlurry, "Throw all your cards at the target","Card Flurry", COST, Strength::Powerful, true, WavFile("cardflurry", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString GunslingerMoves::CardFlurry::doAction(Creature* self, Creature* other)
{
	ColorString info = self->throwCard(other);
	if (info.getString() == "") // no cards to throw
	{
		Player* player = dynamic_cast<Player*>(self);
		if (player != nullptr)
		{
			player->addVitality(getCost());
		}
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
			ColorString(" has no cards to throw", ddutil::TEXT_COLOR);
	}

	// throw all the rest of their cards
	while (true)
	{
		info = self->throwCard(other);
		if (info.getString() == "") // no cards to throw
		{
			return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
				ColorString(" throws all their cards at The ", ddutil::TEXT_COLOR) +
				other->getColorString();
		}
	}
	
}

GunslingerMoves::Uppercut::Uppercut()
	:StatusAttackMove(MoveId::GunslingerUppercut, DAMAGE, new VulnerableStatus(), VULN_LENGTH, COST, "Uppercut",
		Strength::Powerful, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::DoubleKick::DoubleKick()
	:MultiAttackMove(MoveId::GunslingerDoubleKick, KICK_DAMAGE, KICK_TIMES, COST, "Double Kick", Strength::Powerful, WavFile("dualattack", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::HeavyRevolver::HeavyRevolver()
	:GunMove(MoveId::GunslingerHeavyRevolver, DAMAGE, BULLET_USE, COST, "Heavy Revolver", Strength::Powerful, WavFile("shot3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

// Mythical

GunslingerMoves::SupressiveFire::SupressiveFire()
	:Move(MoveId::GunslingerSupressiveFire, "Shoot all your bullets; Each bullet increases dodge chance by "+std::to_string(DODGE_PER_BULLET)+"%",
		"Suppressive Fire", COST, Strength::Mythical, false, WavFile("jump", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString GunslingerMoves::SupressiveFire::doAction(Creature* self, Creature* other)
{
	Gunslinger* gunslinger = dynamic_cast<Gunslinger*>(self);

	if (gunslinger == nullptr)
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" cannot use bullet attacks", ddutil::TEXT_COLOR);
	}

	int dodgeChance = 0;
	int bulletsShot = 0;
	while (gunslinger->useBullet(1))
	{
		playSound(WavFile("shot2", false, false));
		dodgeChance += DODGE_PER_BULLET;
		bulletsShot++;
	}
	if (dodgeChance > 100)
	{
		dodgeChance = 100;
	}

	self->increaseDodgeChance(dodgeChance);

	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + 
		ColorString(" shoots " + std::to_string(bulletsShot) + " bullets for a "  +
		std::to_string(self->getDodgeChance()) + "% chance to dodge the next attack this turn", ddutil::TEXT_COLOR);
}
GunslingerMoves::SuckerPunch::SuckerPunch()
	:StatusAttackMove(MoveId::GunslingerSuckerPunch, DAMAGE, new VulnerableStatus(), VULN_LENGTH, COST, "Sucker Punch",
		Strength::Mythical, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}


GunslingerMoves::SharpsRifle::SharpsRifle()
	:GunMove(MoveId::GunslingerSharpsRifle, DAMAGE, BULLET_AMOUNT, COST, "Sharps Rifle", Strength::Mythical, WavFile("shot3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::ColtWalker::ColtWalker()
	:GunMove(MoveId::GunslingerSharpsRifle, DAMAGE, BULLET_AMOUNT, COST, "Colt Walker", Strength::Mythical, WavFile("shot3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::FaceOfSpades::FaceOfSpades()
	:CardDrawMove(MoveId::GunslingerFaceOfSpades, MIN_DRAW, MAX_DRAW, Suit::Spade, COST, "Face of Spades", Strength::Mythical)
{
}


GunslingerMoves::FaceOfHearts::FaceOfHearts()
	:CardDrawMove(MoveId::GunslingerFaceOfHearts, MIN_DRAW, MAX_DRAW, Suit::Heart, COST, "Face of Hearts", Strength::Mythical)
{
}


GunslingerMoves::FaceOfClubs::FaceOfClubs()
	:CardDrawMove(MoveId::GunslingerFaceOfClubs, MIN_DRAW, MAX_DRAW, Suit::Club, COST, "Face of Clubs", Strength::Mythical)
{
}


GunslingerMoves::FaceOfDiamonds::FaceOfDiamonds()
	:CardDrawMove(MoveId::GunslingerFaceOfDiamonds, MIN_DRAW, MAX_DRAW, Suit::Diamond, COST, "Face of Diamonds", Strength::Mythical)
{
}

GunslingerMoves::ConjureBullets::ConjureBullets()
	:MakeBulletMove(MoveId::GunslingerConjureBullets, BULLET_AMOUNT, COST, "Conjure Bullets", Strength::Mythical, WavFile("magicspell1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::BlackDynamite::BlackDynamite()
	:ThrowDynamiteMove(MoveId::GunslingerBlackDynamite,EXPLOSION_TIME, COST, "Black Dynamite", Strength::Mythical)
{
}

GunslingerMoves::ExplosiveShot::ExplosiveShot()
	:Move(MoveId::GunslingerExplosiveShot, "Shoots the target to explode all Dynamite for "+std::to_string(BULLET_USE)+ " bullet", "Shoot Dynamite",
		COST, Strength::Powerful, true, WavFile("shootexplosion", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString GunslingerMoves::ExplosiveShot::doAction(Creature* self, Creature* other)
{
	int numDynamite = other->getNumberOfStatuses(StatusID::Dynamite);
	Gunslinger* p = dynamic_cast<Gunslinger*>(self);
	if (p == nullptr)
	{
		return ColorString("ERROR: ExplosiveShot on non gunslinger", ddutil::TEXT_COLOR);
	}

	if (numDynamite > 0)
	{
		int damage = numDynamite * Dynamite::DAMAGE;
		other->clearAllStatuses(StatusID::Dynamite);
		ddutil::DamageReport damRep = other->reduceHealth(damage, nullptr, false);
		return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() +
			ColorString(" takes ", ddutil::TEXT_COLOR) +
			ddutil::genericDamageAlert(damRep) +
			ColorString("from the ", ddutil::TEXT_COLOR) +
			ColorString("Dynamite", Dynamite::COLOR);
	}
	else
	{
		p->addVitality(COST);
		return ColorString("There is no ", ddutil::TEXT_COLOR) +
			ColorString("Dynamite", Dynamite::COLOR) +
			ColorString(" to explode", ddutil::TEXT_COLOR);
	}
}


GunslingerMoves::BrassKnuckles::BrassKnuckles()
	:SimpleAttackMove(MoveId::GunslingerBrassKnuckles,DAMAGE, false, COST, "Brass Knuckles", Strength::Mythical, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::JesterForm::JesterForm()
	:Move(MoveId::GunslingerJesterForm, "Applies a buff which doubles the effects of card draw moves for " + std::to_string(DURATION) + " turns", "Jester Form", COST,
		Strength::Mythical, false, WavFile("laugh", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

ColorString GunslingerMoves::JesterForm::doAction(Creature* self, Creature* other)
{	
	JesterStatus* newStatus = new JesterStatus();
	ColorString statusName = newStatus->getName();
	self->applyStatus(newStatus, DURATION);
	self->changePicture(Art::getJesterGunslinger());

	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" enters ", ddutil::TEXT_COLOR) + statusName +
		ColorString(" for " + std::to_string(DURATION) + " turns", ddutil::TEXT_COLOR);
}
