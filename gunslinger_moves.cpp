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

GunslingerMoves::GunMove::GunMove(int dam, int bulletAmt, int cost, std::string name, Strength str, WavFile theSound)
	:Move("Deals " + std::to_string(dam) + " damage with " + std::to_string(bulletAmt) +
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


GunslingerMoves::MakeBulletMove::MakeBulletMove(int bulletAmt, int cost, std::string name, Strength str, WavFile sound)
	:Move("Creates " + std::to_string(bulletAmt) + ((bulletAmt > 1) ? " bullets" : " bullet"), name, cost, str, false, sound)
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

GunslingerMoves::ThrowDynamiteMove::ThrowDynamiteMove(int turnsToWait, int cost, std::string name, Strength str)
	:Move("Throws dynamite which explodes for " + std::to_string(Dynamite::DAMAGE) + " damage in " + std::to_string(turnsToWait) + " turns",
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

GunslingerMoves::CardDrawMove::CardDrawMove(int theMinDraw, int theMaxDraw, Suit theSuit, int cost, std::string name, Strength str)
	:Move("Draws a " + suitToString(theSuit) + " from " + ddutil::cardNumToString(theMinDraw) + " to " + ddutil::cardNumToString(theMaxDraw),
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
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" draws the ", ddutil::TEXT_COLOR) + cardName;
}


// Weak

GunslingerMoves::Peashooter::Peashooter()
	:GunMove(DAMAGE, BULLET_AMOUNT, COST, "Peashooter", Strength::Weak, WavFile("shot1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{}


GunslingerMoves::Dodge::Dodge()
	:SelfBlockMove(BLOCK_AMOUNT, COST, "Dodge", Strength::Weak, WavFile("jump", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::MakeBullets::MakeBullets()
	:MakeBulletMove(BULLET_AMOUNT, COST, "Make Bullets", Strength::Weak)
{
}

// Moderate

GunslingerMoves::Backflip::Backflip()
	:SelfBlockMove(BLOCK_AMOUNT, COST, "Backflip", Strength::Moderate, WavFile("jump", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::Punch::Punch()
	:SimpleAttackMove(DAMAGE, false, COST, "Punch", Strength::Moderate, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::DrawSpade::DrawSpade()
	:CardDrawMove(MIN_DRAW, MAX_DRAW, Suit::Spade, COST, "Draw Spade", Strength::Moderate)
{
}


GunslingerMoves::DrawHeart::DrawHeart()
	: CardDrawMove(MIN_DRAW, MAX_DRAW, Suit::Heart, COST, "Draw Heart", Strength::Moderate)
{
}

GunslingerMoves::DrawClub::DrawClub()
	: CardDrawMove(MIN_DRAW, MAX_DRAW, Suit::Club, COST, "Draw Club", Strength::Moderate)
{
}

GunslingerMoves::DrawDiamond::DrawDiamond()
	: CardDrawMove(MIN_DRAW, MAX_DRAW, Suit::Diamond, COST, "Draw Diamond", Strength::Moderate)
{
}


GunslingerMoves::CardThrow::CardThrow()
	:Move("Gives leftmost card effect to the target", "Card Throw", COST, Strength::Moderate, true, WavFile("cardthrow", ddutil::SF_LOOP, ddutil::SF_ASYNC))
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
	:GunMove(DAMAGE, BULLET_AMOUNT, COST, "Revolver", Strength::Moderate, WavFile("shot2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

// doesnt derive from simple status move because needs a special message specific to throwing dynamite
GunslingerMoves::ThrowDynamite::ThrowDynamite()
	:ThrowDynamiteMove(EXPLOSION_TIME, COST, "Throw Dynamite", Strength::Moderate)
{
}

GunslingerMoves::CraftBullets::CraftBullets()
	:MakeBulletMove(BULLET_AMOUNT, COST, "Craft Bullets", Strength::Moderate)
{
}

GunslingerMoves::BottleStrike::BottleStrike()
	:SimpleStatusMove(
		new VulnerableStatus(), VULN_LENGTH, true, COST, "Bottle Strike",
		Strength::Moderate, WavFile("attack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}


// Powerful

GunslingerMoves::Leap::Leap()
	:SelfBlockMove(BLOCK_AMOUNT, COST, "Leap", Strength::Powerful, WavFile("jump", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}


GunslingerMoves::PumpShotgun::PumpShotgun()
	:GunMove(DAMAGE, BULLET_AMOUNT, COST, "Pump Shotgun", Strength::Powerful, WavFile("shotgun", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::ForgeBullets::ForgeBullets()
	:MakeBulletMove(BULLET_AMOUNT, COST, "Forge Bullets", Strength::Powerful)
{
	
}

GunslingerMoves::SupressiveFire::SupressiveFire()
	:Move("Shoot all your bullets; Each bullet increases dodge chance by 25%",
		"Suppressive Fire", COST, Strength::Powerful, false, WavFile("shot2", ddutil::SF_LOOP, ddutil::SF_ASYNC))
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

GunslingerMoves::QuickDynamite::QuickDynamite()
	:ThrowDynamiteMove(EXPLOSION_TIME, COST, "Quick Dynamite", Strength::Powerful)
{
}

GunslingerMoves::DrinkWhiskey::DrinkWhiskey()
	:Move("Gain "+std::to_string(VITALITY_GAIN)+" vitality but lose "+std::to_string(VITALITY_PER_TURN_LOSS)+" vitality at the beginning of every turn",
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
	:Move("Throw all your cards at the target","Card Flurry", COST, Strength::Powerful, true, WavFile("cardflurry", ddutil::SF_LOOP, ddutil::SF_ASYNC))
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
	:StatusAttackMove(DAMAGE, new VulnerableStatus(), VULN_LENGTH, COST, "Uppercut",
		Strength::Powerful, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}


// Mythical

GunslingerMoves::SuckerPunch::SuckerPunch()
	:StatusAttackMove(DAMAGE, new VulnerableStatus(), VULN_LENGTH, COST, "Sucker Punch",
		Strength::Mythical, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}


GunslingerMoves::SharpsRifle::SharpsRifle()
	:GunMove(DAMAGE, BULLET_AMOUNT, COST, "Sharps Rifle", Strength::Mythical, WavFile("shot3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::ColtWalker::ColtWalker()
	:GunMove(DAMAGE, BULLET_AMOUNT, COST, "Colt Walker", Strength::Mythical, WavFile("shot3", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::FaceOfSpades::FaceOfSpades()
	:CardDrawMove(MIN_DRAW, MAX_DRAW, Suit::Spade, COST, "Face of Spades", Strength::Mythical)
{
}


GunslingerMoves::FaceOfHearts::FaceOfHearts()
	:CardDrawMove(MIN_DRAW, MAX_DRAW, Suit::Heart, COST, "Face of Hearts", Strength::Mythical)
{
}


GunslingerMoves::FaceOfClubs::FaceOfClubs()
	:CardDrawMove(MIN_DRAW, MAX_DRAW, Suit::Club, COST, "Face of Clubs", Strength::Mythical)
{
}


GunslingerMoves::FaceOfDiamonds::FaceOfDiamonds()
	:CardDrawMove(MIN_DRAW, MAX_DRAW, Suit::Diamond, COST, "Face of Diamonds", Strength::Mythical)
{
}

GunslingerMoves::ConjureBullets::ConjureBullets()
	:MakeBulletMove(BULLET_AMOUNT, COST, "Conjure Bullets", Strength::Mythical, WavFile("magicspell1", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::BlackDynamite::BlackDynamite()
	:ThrowDynamiteMove(EXPLOSION_TIME, COST, "Black Dynamite", Strength::Mythical)
{
}

GunslingerMoves::ExplosiveShot::ExplosiveShot()
	:Move("Shoots the target to explode all Dynamite for "+std::to_string(BULLET_USE)+ " bullet", "Shoot Dynamite",
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

GunslingerMoves::BrewToxins::BrewToxins()
	:Move(
		"Makes all attacks apply "+std::to_string(POISON)+" Poison",
		"Brew Toxins",
		COST,
		Strength::Mythical,
		false,
		WavFile("gainblock", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString GunslingerMoves::BrewToxins::doAction(Creature* self, Creature* other)
{
	self->addAttackStatus(new PoisonedStatus(), POISON);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString("'s attacks will now apply " + std::to_string(POISON) + " additional ", ddutil::TEXT_COLOR) +
		ColorString("Poison", PoisonedStatus::COLOR);
}

GunslingerMoves::BrassKnuckles::BrassKnuckles()
	:SimpleAttackMove(DAMAGE, false, COST, "Brass Knuckles", Strength::Mythical, WavFile("attack5", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
}

GunslingerMoves::JesterForm::JesterForm()
	:Move("Applies a buff which gives a random face card for " + std::to_string(DURATION) + " turns", "Jester Form", COST,
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
