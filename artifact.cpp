#include "artifact.h"
#include "utilities.h"
#include "move.h"
#include "colorstring.h"
#include "game.h"
#include "status.h"
#include "creature.h"
#include "player.h"
#include "soundfile.h"

Artifact::Artifact(std::string theName, ColorString theDescription, ArtifactID theID, Strength theStrength, int theColor, Game* theGame)
{
	color = theColor;
	name = ColorString(theName, color);
	description = theDescription;
	id = theID;
	strength = theStrength;
	game = theGame;
}

ColorString Artifact::getFullInformation()
{
	return name + ColorString(": ", ddutil::TEXT_COLOR) + description;
}

ColorString Artifact::getName()
{
	return name;
}

ColorString Artifact::getDescription()
{
	return description;
}

ArtifactID Artifact::getID()
{
	return id;
}

Artifact* Artifact::generateRandomArtifact(Strength strength)
{
	// TODO
	return nullptr;
}

Artifact* Artifact::getArtifactFromID(Game* game, ArtifactID id)
{
	switch (id)
	{
	case ArtifactID::AdamantiteShield:
		return new AdamantiteShield(game);
	case ArtifactID::AncientBook:
		return new AncientBook(game);
	case ArtifactID::AncientMask:
		return new AncientMask(game);
	case ArtifactID::AncientScroll:
		return new AncientScroll(game);
	case ArtifactID::AncientTome:
		return new AncientTome(game);
	case ArtifactID::AncientWand:
		return new AncientWand(game);
	case ArtifactID::BattleStandard:
		return new BattleStandard(game);
	case ArtifactID::BurningOrb:
		return new BurningOrb(game);
	case ArtifactID::Chainmail:
		return new Chainmail(game);
	case ArtifactID::DarkSeed:
		return new DarkSeed(game);
	case ArtifactID::ElectricOrb:
		return new ElectricOrb(game);
	case ArtifactID::EternalSeed:
		return new EternalSeed(game);
	case ArtifactID::EtherealDagger:
		return new EtherealDagger(game);
	case ArtifactID::FrozenOrb:
		return new FrozenOrb(game);
	case ArtifactID::Katana:
		return new Katana(game);
	case ArtifactID::MagicAmulet:
		return new MagicAmulet(game);
	case ArtifactID::MagicBean:
		return new MagicBean(game);
	case ArtifactID::MagicLeech:
		return new MagicLeech(game);
	case ArtifactID::MagicPotion:
		return new MagicPotion(game);
	case ArtifactID::MagicPowder:
		return new MagicPowder(game);
	case ArtifactID::MagicQuill:
		return new MagicQuill(game);
	case ArtifactID::PoisonVial:
		return new PoisonVial(game);
	case ArtifactID::PossessedDoll:
		return new PossessedDoll(game);
	case ArtifactID::PotentPowder:
		return new PotentPowder(game);
	case ArtifactID::PurpleAmulet:
		return new PurpleAmulet(game);
	case ArtifactID::QuailFeather:
		return new QuailFeather(game);
	case ArtifactID::RustyDagger:
		return new RustyDagger(game);
	case ArtifactID::SacrificialKnife:
		return new SacrificialKnife(game);
	case ArtifactID::Shuriken:
		return new Shuriken(game);
	case ArtifactID::SmellingSalts:
		return new SmellingSalts(game);
	case ArtifactID::SpectralAmulet:
		return new SpectralAmulet(game);
	case ArtifactID::Spurs:
		return new Spurs(game);
	case ArtifactID::SwiftnessBracelet:
		return new SwiftnessBracelet(game);
	case ArtifactID::Tesseract:
		return new Tesseract(game);
	case ArtifactID::VitalStone:
		return new VitalStone(game);
	case ArtifactID::WarriorHelm:
		return new WarriorHelm(game);
	default:
		return nullptr;
	}
}

bool Artifact::operator<(const Artifact& other) const
{
	if (strength == Strength::Moderate)
	{
		if (other.strength == Strength::Powerful || other.strength == Strength::Mythical)
		{
			return true;
		}
		// else other strength is moderate, and sort by alphabetizing down below the if chain
	}
	else if (strength == Strength::Powerful)
	{
		if (other.strength == Strength::Moderate)
		{
			return false;
		}
		else if (other.strength == Strength::Mythical)
		{
			return true;
		}
		// same strength, sort by alphabetizing below
	}
	else if (strength == Strength::Mythical)
	{
		if (other.strength != Strength::Mythical)
		{
			return false;
		}
		// same strength, sort by alphabetizing
	}

	return (name.getString() < other.name.getString());
}

ModerateArtifact::ModerateArtifact(std::string name, ColorString description, ArtifactID theID, Game* theGame)
	:Artifact(name, description, theID, Strength::Moderate, ddutil::MODERATE_COLOR, theGame)
{
}

void ModerateArtifact::playFindSound()
{
	playSound(WavFile("moderateartifact", false, false));
}

RustyDagger::RustyDagger(Game* theGame)
	:ModerateArtifact(
		"Rusty Dagger",
		ColorString("Applies ", ddutil::TEXT_COLOR) + 
			ColorString("Bleeding", BleedingStatus::COLOR) + 
			ColorString(" on every attack", ddutil::TEXT_COLOR),
		ArtifactID::RustyDagger,
		theGame
	)
{
}

void RustyDagger::equipAction(Player* player)
{
	player->addAttackStatus(new BleedingStatus(), BLEED_AMOUNT);
}

PurpleAmulet::PurpleAmulet(Game* theGame)
	:ModerateArtifact(
		"Purple Amulet", 
		ColorString("Gives ", ddutil::TEXT_COLOR) +
		ColorString(std::to_string(EXP_PERCENT_GAIN) + "% more experience", ddutil::EXPERIENCE_COLOR),
		ArtifactID::PurpleAmulet,
		theGame
	)
{
}

void PurpleAmulet::equipAction(Player* player)
{
	player->increasePercentXPBoost(EXP_PERCENT_GAIN);
}

SwiftnessBracelet::SwiftnessBracelet(Game* theGame)
	:ModerateArtifact(
		"Swiftness Bracelet",
		ColorString("Gives a base "+std::to_string(DODGE_CHANCE)+"% chance to dodge attacks", ddutil::TEXT_COLOR),
		ArtifactID::SwiftnessBracelet,
		theGame
	)
{
}

void SwiftnessBracelet::equipAction(Player* player)
{
	player->increaseBaseDodgeChance(DODGE_CHANCE);
	player->resetDodgeChance();

}

MagicBean::MagicBean(Game* theGame)
	:ModerateArtifact(
		"Magic Bean",
		ColorString("Increases Max HP by " + std::to_string(MAX_HEALTH_INCREASE), ddutil::HEAL_COLOR),
		ArtifactID::MagicBean,
		theGame
	)
{
}

void MagicBean::equipAction(Player* player)
{
	player->increaseMaxHealth(MAX_HEALTH_INCREASE);
}

MagicPowder::MagicPowder(Game* game)
	:ModerateArtifact(
		"Magic Powder",
		ColorString("Increases Starting Vitality by " + std::to_string(S_VITALITY_INCREASE), ddutil::VITALITY_COLOR),
		ArtifactID::MagicPowder,
		game
	)
{
}

void MagicPowder::equipAction(Player* player)
{
	player->increaseStartingVitality(S_VITALITY_INCREASE);
}

AncientBook::AncientBook(Game* game)
	:ModerateArtifact(
		"Ancient Book",
		ColorString("Gives " + std::to_string(EXP_GAIN) + " experience", ddutil::EXPERIENCE_COLOR),
		ArtifactID::AncientBook,
		game
	)
{
}

Chainmail::Chainmail(Game* game)
	:ModerateArtifact(
		"Chainmail",
		ColorString("Gives ", ddutil::TEXT_COLOR) +
		ColorString(std::to_string(BLOCK_GAIN)+ " Block", ddutil::BLOCK_COLOR) +
		ColorString(" every turn", ddutil::TEXT_COLOR),
		ArtifactID::Chainmail,
		game
	)
{
}

void Chainmail::equipAction(Player* player)
{
	player->increaseBaseBlock(BLOCK_GAIN);
}

SmellingSalts::SmellingSalts(Game* game)
	:ModerateArtifact("Smelling Salts", ColorString("Allows holder to select from one more move", ddutil::TEXT_COLOR),
		ArtifactID::SmellingSalts, game)
{
}

void SmellingSalts::equipAction(Player* player)
{
	player->increaseMovesToChooseFrom(CARD_CHOICE_GAIN);
}

void AncientBook::equipAction(Player* player)
{
	player->gainExperience(EXP_GAIN);
}

PowerfulArtifact::PowerfulArtifact(std::string name, ColorString description, ArtifactID theID, Game* theGame)
	:Artifact(name, description, theID, Strength::Powerful, ddutil::POWERFUL_COLOR, theGame)
{
}

void PowerfulArtifact::playFindSound()
{
	playSound(WavFile("powerfulartifact", false, false));
}

SpectralAmulet::SpectralAmulet(Game* game)
	:PowerfulArtifact(
		"Spectral Amulet",
		ColorString("Applies ", ddutil::TEXT_COLOR) +
		ColorString("Invulnerable", InvulnerableStatus::COLOR) +
		ColorString(" on self at the beginning of battle", ddutil::TEXT_COLOR),
		ArtifactID::SpectralAmulet,
		game
	)
{
}

void SpectralAmulet::equipAction(Player* player)
{
	player->addSelfStartingStatus(new InvulnerableStatus(), INVULN_TURNS);
}

SacrificialKnife::SacrificialKnife(Game* game)
	:PowerfulArtifact(
		"Sacrifical Knife",
		ColorString("Applies ", ddutil::TEXT_COLOR) +
		ColorString("Vulnerable", VulnerableStatus::COLOR) +
		ColorString(" on the enemy at the beginning of battle", ddutil::TEXT_COLOR),
		ArtifactID::SacrificialKnife,
		game
	)
{
}

void SacrificialKnife::equipAction(Player* player)
{
	player->addEnemyStartingStatus(new VulnerableStatus(), VULN_TURNS);
}

BurningOrb::BurningOrb(Game* game)
	:PowerfulArtifact(
		"Burning Orb",
		ColorString("Applies ", ddutil::TEXT_COLOR) +
			ColorString("Burnt", BurntStatus::COLOR) +
			ColorString(" on every attack", ddutil::TEXT_COLOR),
		ArtifactID::BurningOrb,
		game
	)
{
}

void BurningOrb::equipAction(Player* player)
{
	player->addAttackStatus(new BurntStatus(), BURN_TURNS);
}

FrozenOrb::FrozenOrb(Game* game)
	:PowerfulArtifact(
		"Frozen Orb",
		ColorString("Applies ", ddutil::TEXT_COLOR) +
			ColorString("Frostburnt", FrostBurntStatus::COLOR) +
			ColorString(" on every attack", ddutil::TEXT_COLOR),
		ArtifactID::FrozenOrb,
		game
	)
{
}

void FrozenOrb::equipAction(Player* player)
{
	player->addAttackStatus(new FrostBurntStatus(), FBURN_TURNS);
}

ElectricOrb::ElectricOrb(Game* game)
	:PowerfulArtifact(
		"Electric Orb",
		ColorString("Applies ", ddutil::TEXT_COLOR) +
			ColorString("Zapped", ZappedStatus::COLOR) +
			ColorString(" on every attack", ddutil::TEXT_COLOR),
		ArtifactID::ElectricOrb,
		game
	)
{
}

void ElectricOrb::equipAction(Player* player)
{
	player->addAttackStatus(new ZappedStatus(), ZAPPED_TURNS);
}


QuailFeather::QuailFeather(Game* game)
	:PowerfulArtifact(
		"Quail Feather",
		ColorString("Gives a base " + std::to_string(DODGE_CHANCE) + "% chance to dodge attacks", ddutil::TEXT_COLOR),
		ArtifactID::QuailFeather,
		game
	)
{
}

void QuailFeather::equipAction(Player* player)
{
	player->increaseBaseDodgeChance(DODGE_CHANCE);
}

MagicPotion::MagicPotion(Game* game)
	:PowerfulArtifact(
		"Magic Potion",
		ColorString("Increases Max HP by "+std::to_string(MAX_HP_INC), ddutil::HEAL_COLOR),
		ArtifactID::MagicPotion,
		game
	)
{
}

void MagicPotion::equipAction(Player* player)
{
	player->increaseMaxHealth(MAX_HP_INC);
}

AncientTome::AncientTome(Game* game)
	:PowerfulArtifact(
		"Ancient Tome",
		ColorString("Gives " + std::to_string(EXP_GAIN) + " experience", ddutil::EXPERIENCE_COLOR),
		ArtifactID::AncientTome,
		game
	)
{
}

void AncientTome::equipAction(Player* player)
{
	player->gainExperience(EXP_GAIN);
}

MagicQuill::MagicQuill(Game* game)
	:PowerfulArtifact(
		"Magic Quill",
		ColorString("Increases max moves by " +
			std::to_string(MOVESET_INCREASE) +
			"(cannot increase beyond " +
			std::to_string(ddutil::ABSOLUTE_MAX_MOVES) +
			")",
			ddutil::TEXT_COLOR
		),
		ArtifactID::MagicQuill,
		game
	)
{
}

void MagicQuill::equipAction(Player* player)
{
	player->increaseMovesetLimit(MOVESET_INCREASE);
}

WarriorHelm::WarriorHelm(Game* game)
	:PowerfulArtifact(
		"Warrior Helm",
		ColorString("Gives ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(BLOCK_AMOUNT)+" Block ", ddutil::BLOCK_COLOR) +
			ColorString("every turn", ddutil::TEXT_COLOR),
		ArtifactID::WarriorHelm,
		game
	)
{
}

void WarriorHelm::equipAction(Player* player)
{
	player->increaseBaseBlock(BLOCK_AMOUNT);
}

Shuriken::Shuriken(Game* game)
	:PowerfulArtifact(
		"Shuriken",
		ColorString("Applies ", ddutil::TEXT_COLOR) +
		ColorString("Bleeding ", BleedingStatus::COLOR) +
		ColorString(std::to_string(BLEED_AMOUNT) + " times on every attack", ddutil::TEXT_COLOR),
		ArtifactID::Shuriken,
		game
	)
{
}

void Shuriken::equipAction(Player* player)
{
	player->addAttackStatus(new BleedingStatus(), BLEED_AMOUNT);
}

PotentPowder::PotentPowder(Game* game)
	:PowerfulArtifact(
		"Potent Powder",
		ColorString("Increases Starting Vitality by " + std::to_string(S_VIT_GAIN), ddutil::VITALITY_COLOR),
		ArtifactID::PotentPowder,
		game
	)
{
}

void PotentPowder::equipAction(Player* player)
{
	player->increaseStartingVitality(S_VIT_GAIN);
}

AncientMask::AncientMask(Game* game)
	:PowerfulArtifact("Ancient Mask",
		ColorString("Stuns", StunnedStatus::COLOR) +
		ColorString(" the wearer for "+std::to_string(STUN_LENGTH)+" turn, but increases ", ddutil::TEXT_COLOR) + 
		ColorString("starting vitality by " + std::to_string(S_VIT_GAIN), ddutil::VITALITY_COLOR),
		ArtifactID::AncientMask, game)
{
}

void AncientMask::equipAction(Player* player)
{
	player->increaseStartingVitality(S_VIT_GAIN);
	player->addSelfStartingStatus(new StunnedStatus(), STUN_LENGTH);
}

// Mythical

MythicalArtifact::MythicalArtifact(std::string name, ColorString description, ArtifactID theID, Game* theGame)
	:Artifact(name, description, theID, Strength::Mythical, ddutil::MYTHICAL_COLOR, theGame)
{
}

void MythicalArtifact::playFindSound()
{
	playSound(WavFile("mythicalartifact", false, false));
}

VitalStone::VitalStone(Game* game)
	:MythicalArtifact(
		"Vital Stone",
		ColorString("Increases Max HP by " + std::to_string(MAX_HP_INC), ddutil::HEAL_COLOR),
		ArtifactID::VitalStone,
		game
	)
{
}

void VitalStone::equipAction(Player* player)
{
	player->increaseMaxHealth(MAX_HP_INC);
}


PoisonVial::PoisonVial(Game* game)
	:MythicalArtifact(
		"Poison Vial",
		ColorString("Applies ", ddutil::TEXT_COLOR) +
			ColorString("Poison", PoisonedStatus::COLOR) +
			ColorString(" on every attack", ddutil::TEXT_COLOR),
		ArtifactID::PoisonVial,
		game
	)
{
}

void PoisonVial::equipAction(Player* player)
{
	player->addAttackStatus(new PoisonedStatus(), POISON_AMOUNT);
}

Katana::Katana(Game* game)
	:MythicalArtifact(
		"Katana",
		ColorString("Applies ", ddutil::TEXT_COLOR) +
			ColorString("Bleeding ", BleedingStatus::COLOR) +
			ColorString(std::to_string(BLEED_AMOUNT)+" times on every attack", ddutil::TEXT_COLOR),
		ArtifactID::Katana,
		game
	)
{
}

void Katana::equipAction(Player* player)
{
	player->addAttackStatus(new BleedingStatus(), BLEED_AMOUNT);
}

AncientWand::AncientWand(Game* game)
	:MythicalArtifact(
		"Ancient Wand",
		ColorString("Applies ", ddutil::TEXT_COLOR) +
		ColorString("Frostburnt, ", FrostBurntStatus::COLOR) +
			ColorString("Zapped, ", ZappedStatus::COLOR) + ColorString("and ", ddutil::TEXT_COLOR) +
			ColorString("Burnt ", BurntStatus::COLOR) +
			ColorString("on every attack", ddutil::TEXT_COLOR),
		ArtifactID::AncientWand,
		game
	)
{
}

void AncientWand::equipAction(Player* player)
{
	player->addAttackStatus(new BurntStatus(), STATUS_AMOUNT);
	player->addAttackStatus(new FrostBurntStatus(), STATUS_AMOUNT);
	player->addAttackStatus(new ZappedStatus(), STATUS_AMOUNT);
}

EternalSeed::EternalSeed(Game* game)
	:MythicalArtifact(
		"Eternal Seed",
		ColorString("Boosts all healing by "+std::to_string(PERCENT_HEAL_GAIN)+"%", ddutil::HEAL_COLOR),
		ArtifactID::EternalSeed,
		game
	)
{
}

void EternalSeed::equipAction(Player* player)
{
	player->increasePercentHealBoost(PERCENT_HEAL_GAIN);
}

Spurs::Spurs(Game* game)
	:MythicalArtifact(
		"Spurs",
		ColorString("Grants immunity to damaging statuses", ddutil::TEXT_COLOR),
		ArtifactID::Spurs,
		game
	)
{
}

void Spurs::equipAction(Player* player)
{
	player->setImmunityToStatuses(true);
}

AncientScroll::AncientScroll(Game* game)
	:MythicalArtifact(
		"Ancient Scroll",
		ColorString("Gives " + std::to_string(EXP_GAIN) + " experience", ddutil::EXPERIENCE_COLOR),
		ArtifactID::AncientTome,
		game
	)
{
}

void AncientScroll::equipAction(Player* player)
{
	player->gainExperience(EXP_GAIN);
}

AdamantiteShield::AdamantiteShield(Game* game)
	:MythicalArtifact(
		"Adamantite Shield",
		ColorString("Gives ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(BLOCK_PER_TURN) + " Block ", ddutil::BLOCK_COLOR) +
			ColorString("every turn", ddutil::TEXT_COLOR),
		ArtifactID::AdamantiteShield,
		game
	)
{
}

void AdamantiteShield::equipAction(Player* player)
{
	player->increaseBaseBlock(BLOCK_PER_TURN);
}

EtherealDagger::EtherealDagger(Game* game)
	:MythicalArtifact(
		"Ethereal Dagger",
		ColorString("Applies ", ddutil::TEXT_COLOR) +
			ColorString("Vulnerable ", VulnerableStatus::COLOR) +
			ColorString("on every attack", ddutil::TEXT_COLOR),
		ArtifactID::EtherealDagger,
		game
	)
{
}

void EtherealDagger::equipAction(Player* player)
{
	player->addAttackStatus(new VulnerableStatus(), VULN_AMOUNT);
}


// BOSS artifacts
BossArtifact::BossArtifact(std::string name, ColorString description, ArtifactID theID, Game* theGame)
	:Artifact(name, description, theID, Strength::Mythical, ddutil::MYTHICAL_COLOR, theGame)
{
}

void BossArtifact::playFindSound()
{
	playSound(WavFile("bossartifact", false, false));
}


MagicAmulet::MagicAmulet(Game* game)
	:BossArtifact(
		"Magic Amulet",
		ColorString("Gives " + std::to_string(VIT_PER_TURN_GAIN) + " vitality per turn", ddutil::VITALITY_COLOR),
		ArtifactID::MagicAmulet,
		game
	)
{}

void MagicAmulet::equipAction(Player* player)
{
	player->increaseVitalityPerTurn(VIT_PER_TURN_GAIN);
}

Tesseract::Tesseract(Game* game)
	:BossArtifact(
		"Tesseract",
		ColorString("Gives " + std::to_string(VIT_PER_TURN_GAIN) + " vitality per turn ", ddutil::VITALITY_COLOR) + 
			ColorString("but takes "+std::to_string(MAX_HP_REDUCTION)+" Max HP", ddutil::DAMAGE_COLOR),
		ArtifactID::Tesseract,
		game
	)
{}

void Tesseract::equipAction(Player* player)
{
	player->increaseVitalityPerTurn(VIT_PER_TURN_GAIN);
	player->decreaseMaxHealth(MAX_HP_REDUCTION);
}

PossessedDoll::PossessedDoll(Game* game)
	:BossArtifact(
		"Possessed Doll",
		ColorString("Gives " + std::to_string(VIT_PER_TURN_GAIN) + " vitality per turn ", ddutil::VITALITY_COLOR) + 
			ColorString("but gives ",ddutil::TEXT_COLOR) + ColorString("Hexed " + std::to_string(HEX_TURNS), HexedStatus::COLOR) +
			ColorString(" at the start of battle", ddutil::TEXT_COLOR),
		ArtifactID::PossessedDoll,
		game
	)
{
}

void PossessedDoll::equipAction(Player* player)
{
	player->increaseVitalityPerTurn(VIT_PER_TURN_GAIN);
	player->addSelfStartingStatus(new HexedStatus(), HEX_TURNS);
}

DarkSeed::DarkSeed(Game* game)
	:BossArtifact(
		"Dark Seed",
		ColorString("Gives " + std::to_string(VIT_PER_TURN_GAIN) + " vitality per turn ", ddutil::VITALITY_COLOR) +
		ColorString("but reduces all healing by "+std::to_string(HEALING_REDUCTION)+"%", ddutil::DAMAGE_COLOR),
		ArtifactID::DarkSeed,
		game
	)
{
}

void DarkSeed::equipAction(Player* player)
{
	player->increaseVitalityPerTurn(VIT_PER_TURN_GAIN);
	player->decreasePercentHealBoost(HEALING_REDUCTION);
}

MagicLeech::MagicLeech(Game* game)
	:BossArtifact(
		"Magic Leech",
		ColorString("Gives " + std::to_string(VIT_PER_TURN_GAIN) + " vitality per turn ", ddutil::VITALITY_COLOR) +
			ColorString("but reduces EXP gain by " + std::to_string(EXP_REDUCTION) + "%", ddutil::EXPERIENCE_COLOR),
		ArtifactID::MagicLeech,
		game
	)
{
}

void MagicLeech::equipAction(Player* player)
{
	player->increaseVitalityPerTurn(VIT_PER_TURN_GAIN);
	player->decreasePercentXPBoost(EXP_REDUCTION);
}

BattleStandard::BattleStandard(Game* game)
	:BossArtifact(
		"Battle Standard",
		ColorString("Gives " + std::to_string(VIT_PER_TURN_GAIN) + " vitality per turn ", ddutil::VITALITY_COLOR) +
			ColorString("but gives " + std::to_string(VULN_TURNS) + " turns of ", ddutil::TEXT_COLOR) +
			ColorString("Vulnerable", VulnerableStatus::COLOR) + ColorString(" at the start of battle", ddutil::TEXT_COLOR),
		ArtifactID::BattleStandard, 
		game
	)
{
}

void BattleStandard::equipAction(Player* player)
{
	player->increaseVitalityPerTurn(VIT_PER_TURN_GAIN);
	player->addSelfStartingStatus(new VulnerableStatus(), VULN_TURNS);
}
