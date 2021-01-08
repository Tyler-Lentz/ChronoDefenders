#include "move.h"
#include "utilities.h"
#include "colorstring.h"
#include "creature.h"
#include "soundfile.h"
#include "gunslinger_moves.h"
#include "sorcerer_moves.h"
#include "samurai_moves.h"
#include "cosmic_moves.h"

#include <string>

Move::Move(MoveId id, std::string desc, std::string theName, int theCost, Strength howStrong, bool targetChoose, WavFile theSound)
{
	description = desc;
	cost = theCost;
	strength = howStrong;
	name = theName;
	chooseTarget = targetChoose;
	sound = theSound;
	this->id = id;
}

std::string Move::getDescription()
{
	return description;
}

ColorString Move::getColorString()
{
	int color = ddutil::TEXT_COLOR;
	switch (strength)
	{
	case Strength::Weak:
		color = ddutil::WEAK_COLOR;
		break;
	case Strength::Moderate:
		color = ddutil::MODERATE_COLOR;
		break;
	case Strength::Powerful:
		color = ddutil::POWERFUL_COLOR;
		break;
	case Strength::Mythical:
		color = ddutil::MYTHICAL_COLOR;
		break;
	case Strength::Cosmic:
		color = ddutil::COSMIC_COLOR;
		break;
	}
	return ColorString(name, color);
}

ColorString Move::getFullInformation()
{
	return ColorString("(" + std::to_string(cost) + ") ", ddutil::VITALITY_COLOR) +
		getColorString() + ColorString(": ", ddutil::TEXT_COLOR) +
		ColorString(getDescription(), ddutil::TEXT_COLOR);
}

int Move::getCost()
{
	return cost;
}

Strength Move::getStrength()
{
	return strength;
}


bool Move::canChooseTarget()
{
	return chooseTarget;
}

void Move::playSoundEffect()
{
	sound.play();
}

MoveId Move::getId()
{
	return id;
}

Move* Move::getMoveFromId(MoveId id, Game* game)
{
	switch (id)
	{
	case MoveId::GunslingerBackflip:
		return new GunslingerMoves::Backflip();
	case MoveId::GunslingerBlackDynamite:
		return new GunslingerMoves::BlackDynamite();
	case MoveId::GunslingerBottleStrike:
		return new GunslingerMoves::BottleStrike();
	case MoveId::GunslingerBrassKnuckles:
		return new GunslingerMoves::BrassKnuckles();
	case MoveId::GunslingerCardFlurry:
		return new GunslingerMoves::CardFlurry();
	case MoveId::GunslingerCardThrow:
		return new GunslingerMoves::CardThrow();
	case MoveId::GunslingerColtWalker:
		return new GunslingerMoves::ColtWalker();
	case MoveId::GunslingerConjureBullets:
		return new GunslingerMoves::ConjureBullets();
	case MoveId::GunslingerCraftBullets:
		return new GunslingerMoves::CraftBullets();
	case MoveId::GunslingerDodge:
		return new GunslingerMoves::Dodge();
	case MoveId::GunslingerDoubleKick:
		return new GunslingerMoves::DoubleKick();
	case MoveId::GunslingerDrawClub:
		return new GunslingerMoves::DrawClub();
	case MoveId::GunslingerDrawHeart:
		return new GunslingerMoves::DrawHeart();
	case MoveId::GunslingerDrawSpade:
		return new GunslingerMoves::DrawSpade();
	case MoveId::GunslingerDrawDiamond:
		return new GunslingerMoves::DrawDiamond();
	case MoveId::GunslingerDrinkWhiskey:
		return new GunslingerMoves::DrinkWhiskey();
	case MoveId::GunslingerDropkick:
		return new GunslingerMoves::Dropkick();
	case MoveId::GunslingerExplosiveShot:
		return new GunslingerMoves::ExplosiveShot();
	case MoveId::GunslingerFaceOfClubs:
		return new GunslingerMoves::FaceOfClubs();
	case MoveId::GunslingerFaceOfHearts:
		return new GunslingerMoves::FaceOfHearts();
	case MoveId::GunslingerFaceOfDiamonds:
		return new GunslingerMoves::FaceOfDiamonds();
	case MoveId::GunslingerFaceOfSpades:
		return new GunslingerMoves::FaceOfSpades();
	case MoveId::GunslingerForgeBullets:
		return new GunslingerMoves::ForgeBullets();
	case MoveId::GunslingerHeavyRevolver:
		return new GunslingerMoves::HeavyRevolver();
	case MoveId::GunslingerJesterForm:
		return new GunslingerMoves::JesterForm();
	case MoveId::GunslingerLeap:
		return new GunslingerMoves::Leap();
	case MoveId::GunslingerMakeBullets:
		return new GunslingerMoves::MakeBullets();
	case MoveId::GunslingerPeashooter:
		return new GunslingerMoves::Peashooter();
	case MoveId::GunslingerPumpShotgun:
		return new GunslingerMoves::PumpShotgun();
	case MoveId::GunslingerPunch:
		return new GunslingerMoves::Punch();
	case MoveId::GunslingerQuickDynamite:
		return new GunslingerMoves::QuickDynamite();
	case MoveId::GunslingerRevolver:
		return new GunslingerMoves::Revolver();
	case MoveId::GunslingerSharpsRifle:
		return new GunslingerMoves::SharpsRifle();
	case MoveId::GunslingerSuckerPunch:
		return new GunslingerMoves::SuckerPunch();
	case MoveId::GunslingerSupressiveFire:
		return new GunslingerMoves::SupressiveFire();
	case MoveId::GunslingerThrowDynamite:
		return new GunslingerMoves::ThrowDynamite();
	case MoveId::GunslingerUppercut:
		return new GunslingerMoves::Uppercut();
	case MoveId::SamuraiAssault:
		return new SamuraiMoves::Assault();
	case MoveId::SamuraiBlindingFury:
		return new SamuraiMoves::BlindingFury();
	case MoveId::SamuraiBodySlam:
		return new SamuraiMoves::BodySlam();
	case MoveId::SamuraiCalculatedSlice:
		return new SamuraiMoves::CalculatedSlice();
	case MoveId::SamuraiCaltrops:
		return new SamuraiMoves::Caltrops();
	case MoveId::SamuraiChargedAssault:
		return new SamuraiMoves::ChargedAssault();
	case MoveId::SamuraiCriticalStrike:
		return new SamuraiMoves::CriticalStrike();
	case MoveId::SamuraiCrushingBlow:
		return new SamuraiMoves::CrushingBlow();
	case MoveId::SamuraiDeflect:
		return new SamuraiMoves::Deflect();
	case MoveId::SamuraiDesperation:
		return new SamuraiMoves::Desperation();
	case MoveId::SamuraiDragonBreath:
		return new SamuraiMoves::DragonBreath();
	case MoveId::SamuraiDragonForm:
		return new SamuraiMoves::DragonForm();
	case MoveId::SamuraiDragonSlice:
		return new SamuraiMoves::DragonSlice();
	case MoveId::SamuraiDualingDragons:
		return new SamuraiMoves::DualingDragons();
	case MoveId::SamuraiDualSlice:
		return new SamuraiMoves::DualSlice();
	case MoveId::SamuraiEnlightenment:
		return new SamuraiMoves::Enlightenment();
	case MoveId::SamuraiEviscerate:
		return new SamuraiMoves::Eviscerate();
	case MoveId::SamuraiEyeOfTheStorm:
		return new SamuraiMoves::EyeOfTheStorm();
	case MoveId::SamuraiFinishingBlow:
		return new SamuraiMoves::FinishingBlow();
	case MoveId::SamuraiLeavesFromTheVine:
		return new SamuraiMoves::LeavesFromTheVine();
	case MoveId::SamuraiLightningRod:
		return new SamuraiMoves::LightningRod();
	case MoveId::SamuraiMeditation:
		return new SamuraiMoves::Meditation();
	case MoveId::SamuraiPerilousPlunge:
		return new SamuraiMoves::PerilousPlunge();
	case MoveId::SamuraiPerilousStrike:
		return new SamuraiMoves::PerilousStrike();
	case MoveId::SamuraiShadowStep:
		return new SamuraiMoves::ShadowStep();
	case MoveId::SamuraiShinobiTactics:
		return new SamuraiMoves::ShinobiTactics();
	case MoveId::SamuraiShuriken:
		return new SamuraiMoves::Shuriken();
	case MoveId::SamuraiSidestep:
		return new SamuraiMoves::Sidestep();
	case MoveId::SamuraiSlice:
		return new SamuraiMoves::Slice();
	case MoveId::SamuraiStorm:
		return new SamuraiMoves::Storm();
	case MoveId::SamuraiThrowingKnives:
		return new SamuraiMoves::ThrowingKnives();
	case MoveId::SamuraiTippedKunai:
		return new SamuraiMoves::TippedKunai();
	case MoveId::SamuraiTornado:
		return new SamuraiMoves::Tornado();
	case MoveId::SamuraiTsunami:
		return new SamuraiMoves::Tsunami();
	case MoveId::SamuraiTyphoon:
		return new SamuraiMoves::Typhoon();
	case MoveId::SamuraiZen:
		return new SamuraiMoves::Zen();
	case MoveId::SorceressBlinkStrike:
		return new SorcererMoves::BlinkStrike();
	case MoveId::SorceressBlizzard:
		return new SorcererMoves::Blizzard();
	case MoveId::SorceressCastInvulnerability:
		return new SorcererMoves::CastInvulnerability();
	case MoveId::SorceressCataclysm:
		return new SorcererMoves::Cataclysm();
	case MoveId::SorceressChainLightning:
		return new SorcererMoves::ChainLightning();
	case MoveId::SorceressCleansingAura:
		return new SorcererMoves::CleansingAura();
	case MoveId::SorceressCleansingTouch:
		return new SorcererMoves::CleansingTouch();
	case MoveId::SorceressCombust:
		return new SorcererMoves::Combust();
	case MoveId::SorceressElectricShock:
		return new SorcererMoves::ElectricShock();
	case MoveId::SorceressElementalBarrier:
		return new SorcererMoves::ElementalBarrier();
	case MoveId::SorceressElementalForm:
		return new SorcererMoves::ElementalForm();
	case MoveId::SorceressEnergyAura:
		return new SorcererMoves::EnergyAura();
	case MoveId::SorceressEnergyBarrier:
		return new SorcererMoves::EnergyBarrier();
	case MoveId::SorceressEnergySword:
		return new SorcererMoves::EnergySword();
	case MoveId::SorceressEnergyStrike:
		return new SorcererMoves::EnergyStrike();
	case MoveId::SorceressEtherealBarrier:
		return new SorcererMoves::EtherealBarrier();
	case MoveId::SorceressEtherealBlade:
		return new SorcererMoves::EtherealBlade();
	case MoveId::SorceressFairySummon:
		return new SorcererMoves::FairySummon(game);
	case MoveId::SorceressFireball:
		return new SorcererMoves::Fireball();
	case MoveId::SorceressFreeze:
		return new SorcererMoves::Freeze();
	case MoveId::SorceressHeal:
		return new SorcererMoves::Heal();
	case MoveId::SorceressIceOrb:
		return new SorcererMoves::IceOrb();
	case MoveId::SorceressLevitate:
		return new SorcererMoves::Levitate();
	case MoveId::SorceressLightningStrike:
		return new SorcererMoves::LightningStrike();
	case MoveId::SorceressMagicBarrier:
		return new SorcererMoves::MagicBarrier();
	case MoveId::SorceressMirrorImage:
		return new SorcererMoves::MirrorImage();
	case MoveId::SorceressPlasmaStrike:
		return new SorcererMoves::PlasmaStrike();
	case MoveId::SorceressPlasmaSword:
		return new SorcererMoves::PlasmaSword();
	case MoveId::SorceressRecover:
		return new SorcererMoves::Recover();
	case MoveId::SorceressSpiritCall:
		return new SorcererMoves::SpiritCall(game);
	case MoveId::SorceressStarStrike:
		return new SorcererMoves::StarStrike();
	case MoveId::SorceressSummonFireDragon:
		return new SorcererMoves::SummonFireDragon(game);
	case MoveId::SorceressSummonFireImp:
		return new SorcererMoves::SummonFireImp(game);
	case MoveId::SorceressSummonIceDragon:
		return new SorcererMoves::SummonIceDragon(game);
	case MoveId::SorceressSummonIceImp:
		return new SorcererMoves::SummonIceImp(game);
	case MoveId::SorceressSupernova:
		return new SorcererMoves::Supernova();
	case MoveId::CosmicBlackHole:
		return new CosmicMoves::BlackHole();
	case MoveId::CosmicStarRegen:
		return new CosmicMoves::StarRegen();
	case MoveId::CosmicWarpBarrier:
		return new CosmicMoves::WarpBarrier();
	case MoveId::CosmicDisturbance:
		return new CosmicMoves::Disturbance();
	case MoveId::CosmicDeletion:
		return new CosmicMoves::Deletion();
	case MoveId::CosmicCosmicBeam:
		return new CosmicMoves::CosmicBeam();
	case MoveId::CosmicStarDust:
		return new CosmicMoves::StarDust();
	case MoveId::CosmicEternalTomb:
		return new CosmicMoves::EternalTomb();
	case MoveId::CosmicSeeingEye:
		return new CosmicMoves::SeeingEye();
	case MoveId::SamuraiWarHorn:
		return new SamuraiMoves::WarHorn();
	case MoveId::SamuraiInstinct:
		return new SamuraiMoves::Instinct();
	case MoveId::SamuraiUnhinge:
		return new SamuraiMoves::Unhinge();
	case MoveId::SamuraiFlameVeil:
		return new SamuraiMoves::FlameVeil();
	case MoveId::SamuraiHelpingHand:
		return new SamuraiMoves::HelpingHand();
	case MoveId::SamuraiTackle:
		return new SamuraiMoves::Tackle();
	case MoveId::SamuraiToughenUp:
		return new SamuraiMoves::ToughenUp();
	case MoveId::SamuraiHoldOut:
		return new SamuraiMoves::HoldOut();
	case MoveId::SamuraiDecimate:
		return new SamuraiMoves::Decimate();
	case MoveId::SamuraiDragonsWill:
		return new SamuraiMoves::DragonsWill();
	default:
		return nullptr;
	}
}

StatusAttackMove::StatusAttackMove(MoveId id, int theDamage, Status* theStatus, int statusAmount, int cost, std::string name, Strength str, WavFile theSound)
	:Move(id, "Deals " + std::to_string(theDamage) + " damage and applies " + theStatus->getName().getString() +
		(statusAmount > 1 ? (" " + std::to_string(statusAmount) + " times") : ("")),
		name, cost, str, true, theSound)
{
	damage = theDamage;
	status = theStatus;
	statusStack = statusAmount;
}

StatusAttackMove::~StatusAttackMove()
{
	if (status != nullptr)
	{
		delete status;
	}
}

ColorString StatusAttackMove::doAction(Creature* self, Creature* other)
{
	ddutil::DamageReport damRep = other->reduceHealth(damage, self);
	Status* newStat = status->makeCopy();
	ColorString statusName = newStat->getName();
	other->applyStatus(newStat, statusStack);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" deals ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep) +
		ColorString(" and makes the ", ddutil::TEXT_COLOR) + other->getColorString() + ColorString(" ", ddutil::TEXT_COLOR) + statusName;
}

SimpleAttackMove::SimpleAttackMove(MoveId id, int dam, bool blockIgnore, int cost, std::string name, Strength str, WavFile theSound)
	:Move(id, "Deals " + std::to_string(dam) + " damage" + (blockIgnore ? " through block" : ""), name, cost, str, true, theSound)
{
	damage = dam;
	ignoreBlock = blockIgnore;
}

ColorString SimpleAttackMove::doAction(Creature* self, Creature* other)
{
	ddutil::DamageReport damRep = other->reduceHealth(damage, self, ignoreBlock);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" deals ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep) +
		ColorString(" to the ", ddutil::TEXT_COLOR) + other->getColorString();
}

SimpleStatusMove::SimpleStatusMove(MoveId id, Status* stat, int statAmount, bool chooseTarget, int cost, std::string name, Strength str, WavFile theSound)
	:Move(id, "Applies " + stat->getName().getString() + "("+std::to_string(statAmount)+")" + " to " + ( chooseTarget ? "the target" : "the user"),
		name, cost, str, chooseTarget, theSound)
{
	status = stat;
	statusAmount = statAmount;
}

SimpleStatusMove::~SimpleStatusMove()
{
	if (status != nullptr)
	{
		delete status;
	}
}

ColorString SimpleStatusMove::doAction(Creature* self, Creature* other)
{
	Status* newStatus = status->makeCopy();
	ColorString statusName = newStatus->getName();
	other->applyStatus(newStatus, statusAmount);
	if (self == other)
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
			ColorString(" becomes ", ddutil::TEXT_COLOR) + statusName;
	}
	else
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
			ColorString(" makes the ", ddutil::TEXT_COLOR) + other->getColorString() +
			ColorString(" ", ddutil::TEXT_COLOR) + statusName;
	}
	
}


SelfBlockMove::SelfBlockMove(MoveId id, int blockAmount, int cost, std::string name, Strength str, WavFile theSound)
	:Move(id, "Blocks " + std::to_string(blockAmount) + " damage", name, cost, str, false, theSound)
{
	block = blockAmount;
}

ColorString SelfBlockMove::doAction(Creature* self, Creature* other)
{
	self->applyBlock(block);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() + ColorString(" will block ", ddutil::TEXT_COLOR) + ddutil::genericBlockAlert(block);

}


AttackAndBlockMove::AttackAndBlockMove(MoveId id, int moveDamage, int moveBlock, bool blockIgnore, int cost, std::string name, Strength str, WavFile theSound)
	:SimpleAttackMove(id, moveDamage, blockIgnore, cost, name, str, theSound)
{
	block = moveBlock;
	description += " and blocks " + std::to_string(block) + " damage";
}


ColorString AttackAndBlockMove::doAction(Creature* self, Creature* other)
{
	ColorString baseStr = SimpleAttackMove::doAction(self, other);
	baseStr += ColorString(" and will block ", ddutil::TEXT_COLOR) + ddutil::genericBlockAlert(block);
	self->applyBlock(block);
	return baseStr;
}

SimpleHealMove::SimpleHealMove(MoveId id, int theHealAmount, bool canChooseTarget, int cost, std::string name, Strength str)
	:Move(id, "Heals " + std::to_string(theHealAmount) + " health for the " + (canChooseTarget ? "target" : "caster"), name, cost, str, canChooseTarget,
		WavFile("heal", ddutil::SF_LOOP, ddutil::SF_ASYNC))
{
	healAmount = theHealAmount;
}

ColorString SimpleHealMove::doAction(Creature* self, Creature* other)
{
	Creature* target;
	if (canChooseTarget())
	{
		target = other;
	}
	else
	{
		target = self;
	}

	int actualHealAmount = target->increaseHealth(healAmount);

	return ColorString("The ", ddutil::TEXT_COLOR) + target->getColorString() + ColorString(" heals ", ddutil::TEXT_COLOR) + ColorString(std::to_string(actualHealAmount) + " health", ddutil::HEAL_COLOR);
}

MultiAttackMove::MultiAttackMove(MoveId id, int damagePerHit, int numHits, int cost, std::string name, Strength str, WavFile theSound)
	:Move(id, "Deals " + std::to_string(damagePerHit) + " damage " + std::to_string(numHits) + " times", name, cost, str, true, theSound)
{
	damage = damagePerHit;
	times = numHits;
}

ColorString MultiAttackMove::doAction(Creature* self, Creature* other)
{
	ColorString info = ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" attacks the ", ddutil::TEXT_COLOR) + other->getColorString() +
		ColorString(" " + std::to_string(times) + " times: ", ddutil::TEXT_COLOR);
	for (int i = 0; i < times; i++)
	{
		info += ddutil::genericDamageAlert(other->reduceHealth(damage, self)) + ColorString("; ", ddutil::TEXT_COLOR);
	}
	ddutil::trimLength(info);
	return info;
}

BlockBreakingMove::BlockBreakingMove(MoveId id, int theBaseDamage, int theMultiplier, int cost, std::string name, Strength str, WavFile theSound)
	:Move(id, "Deals " + std::to_string(theBaseDamage) + " damage (x" + std::to_string(theMultiplier) + " if the enemy has block)", name, cost, str, true, theSound)
{
	baseDamage = theBaseDamage;
	multiplier = theMultiplier;
}

ColorString BlockBreakingMove::doAction(Creature* self, Creature* other)
{
	int actualDamage = baseDamage;
	if (other->getBlock() > 0)
	{
		actualDamage *= multiplier;
	}

	SimpleAttackMove tempMove(MoveId::TempMoveId, actualDamage, false, 0, "TEMP", Strength::Moderate, sound);
	return tempMove.doAction(self, other);
}

SelfDamageAttackMove::SelfDamageAttackMove(MoveId id, int moveDamage, bool blockIgnore, int damageTaken, int cost, std::string name, Strength str, WavFile theSound)
	:SimpleAttackMove(id, moveDamage, blockIgnore, cost, name, str, theSound)
{
	selfDamage = damageTaken;
	description += " but inflicts " + std::to_string(selfDamage) + " self-damage";
}

ColorString SelfDamageAttackMove::doAction(Creature* self, Creature* other)
{
	ColorString baseStr = SimpleAttackMove::doAction(self, other);

	ddutil::DamageReport d = self->reduceHealth(selfDamage, self);

	baseStr += ColorString(" and takes ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(d);
	return baseStr;
}