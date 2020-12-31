#include "cosmic_moves.h"
#include "sorcerer_moves.h"
#include "move.h"
#include "game.h"
#include "player.h"
#include "creature.h"
#include "utilities.h"

CosmicMoves::BlackHole::BlackHole()
	:SimpleAttackMove(
		MoveId::CosmicBlackHole,
		DAMAGE,
		true,
		COST,
		"Black Hole",
		Strength::Cosmic,
		WavFile("blackhole", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

CosmicMoves::StarRegen::StarRegen()
	:SimpleHealMove(
		MoveId::CosmicStarRegen,
		HEAL,
		true,
		COST,
		"Cosmic Regen",
		Strength::Cosmic
	)
{
}

CosmicMoves::WarpBarrier::WarpBarrier()
	:SorcererMoves::MakeBarrierMove(
		MoveId::CosmicWarpBarrier,
		BLOCK,
		COST,
		"Warp Barrier",
		Strength::Cosmic
	)
{
}

CosmicMoves::Disturbance::Disturbance()
	:Move(
		MoveId::CosmicDisturbance,
		"Doubles current vitality, but sets vitality gain to 0 for the battle",
		"Disturbance",
		COST,
		Strength::Cosmic,
		false,
		WavFile("electricattack1", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString CosmicMoves::Disturbance::doAction(Creature* self, Creature* other)
{
	Player* player = dynamic_cast<Player*>(self);
	if (player == nullptr)
	{
		return ColorString("This creature does not have a vitality stat", ddutil::TEXT_COLOR);
	}
	if (player->getVitalityGain() == 0)
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
			ColorString("'s", player->getColor()) + ColorString(" vitality gain is already 0", ddutil::VITALITY_COLOR);
	}
	player->addVitality(player->getVitality());
	player->adjustVitalityGainTemp(player->getVitalityGain() * -1);
	return ColorString("The ", ddutil::TEXT_COLOR) + self->getColorString() +
		ColorString(" doubles their vitality, but sets vitality gain to 0", ddutil::VITALITY_COLOR);
}

CosmicMoves::Deletion::Deletion()
	:Move(
		MoveId::CosmicDeletion,
		"If the target has less than "+std::to_string(HP_LIM)+" HP, reduce it to 0",
		"Deletion",
		COST,
		Strength::Cosmic,
		true,
		WavFile("deletion", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString CosmicMoves::Deletion::doAction(Creature* self, Creature* other)
{
	if (other->getHealth() < HP_LIM)
	{
		other->setHealth(0);
		return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() +
			ColorString("'s", other->getColor()) + ColorString(" HP has been reduced to 0", ddutil::DAMAGE_COLOR);
	}
	else
	{
		return ColorString("The deletion is innefective", ddutil::TEXT_COLOR);
	}
}

CosmicMoves::CosmicBeam::CosmicBeam()
	:Move(
		MoveId::CosmicCosmicBeam,
		"Deals " + std::to_string(DAM_PER_VIT)+ " damage per vitality",
		"Cosmic Beam",
		COST,
		Strength::Cosmic,
		true,
		WavFile("magicattack3", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString CosmicMoves::CosmicBeam::doAction(Creature* self, Creature* other)
{
	Player* caster = dynamic_cast<Player*>(self);
	if (caster != nullptr)
	{
		int damage = caster->getVitality() * DAM_PER_VIT;
		SimpleAttackMove tempMove(MoveId::TempMoveId, damage, false, 0, "Cosmic Beam", Strength::Cosmic, sound);
		return tempMove.doAction(self, other);
	}
	else
	{
		return ColorString("This creature has no vitality stat", ddutil::TEXT_COLOR);
	}
}

CosmicMoves::StarDust::StarDust()
	:Move(
		MoveId::CosmicStarDust,
		"Removes all statuses from target and gives user "+std::to_string(VIT_PER_STAT)+ " vitality per status stack",
		"Star Dust",
		COST,
		Strength::Cosmic,
		true,
		WavFile("magicspell2", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

ColorString CosmicMoves::StarDust::doAction(Creature* self, Creature* other)
{
	int vitGain = other->getNumberOfStatuses();
	other->clearAllStatuses();
	Player* caster = dynamic_cast<Player*>(self);
	if (caster != nullptr)
	{
		caster->addVitality(vitGain);
		return ColorString("The ", ddutil::TEXT_COLOR) + other->getColorString() +
			ColorString(" loses all its status effects, and the ", ddutil::TEXT_COLOR) +
			self->getColorString() + ColorString(" gets " + std::to_string(vitGain) + " vitality", ddutil::VITALITY_COLOR);
	}
	else
	{
		return ColorString("The caster does not have a vitality stat", ddutil::TEXT_COLOR);
	}
}

CosmicMoves::EternalTomb::EternalTomb()
	:SimpleStatusMove(
		MoveId::CosmicEternalTomb,
		new EntombedStatus(),
		AMOUNT,
		true,
		COST,
		"Eternal Tomb",
		Strength::Cosmic,
		WavFile("lightning", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}

std::vector<Move*> CosmicMoves::getRandomMoves(int movesToChooseFrom)
{
	std::vector <Move*> moves;
	for (int i : ddutil::uniqueRandom(0, 8, movesToChooseFrom))
	{
		switch (i)
		{
		case 0:
			moves.push_back(new CosmicMoves::BlackHole());
			break;
		case 1:
			moves.push_back(new CosmicMoves::CosmicBeam());
			break;
		case 2:
			moves.push_back(new CosmicMoves::Deletion());
			break;
		case 3:
			moves.push_back(new CosmicMoves::Disturbance());
			break;
		case 4:
			moves.push_back(new CosmicMoves::EternalTomb());
			break;
		case 5:
			moves.push_back(new CosmicMoves::StarDust());
			break;
		case 6:
			moves.push_back(new CosmicMoves::StarRegen());
			break;
		case 7:
			moves.push_back(new CosmicMoves::WarpBarrier());
			break;
		case 8:
			moves.push_back(new CosmicMoves::SeeingEye());
			break;
		}
	}
	return moves;
}

CosmicMoves::SeeingEye::SeeingEye()
	:SimpleStatusMove(
		MoveId::CosmicSeeingEye,
		new MarkedStatus(),
		AMOUNT,
		true,
		COST,
		"Seeing Eye",
		Strength::Cosmic,
		WavFile("vulnerable", ddutil::SF_LOOP, ddutil::SF_ASYNC)
	)
{
}
