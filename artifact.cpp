#include "artifact.h"
#include "utilities.h"
#include "move.h"
#include "colorstring.h"
#include "game.h"
#include "status.h"
#include "creature.h"
#include "player.h"
#include "soundfile.h"
#include "enemy.h"
#include "gunslinger_moves.h"

Artifact::Artifact(std::string theName, ColorString theDescription, ArtifactID theID, Strength theStrength, int theColor, Game* theGame)
{
	color = theColor;
	name = ColorString(theName, color);
	description = theDescription;
	id = theID;
	strength = theStrength;
	game = theGame;
	consumed = false;
}

bool Artifact::isConsumed()
{
	return consumed;
}

void Artifact::setAsConsumed()
{
	consumed = true;
	description += ColorString(" (Used)", ddutil::YELLOW);
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

std::vector<Artifact*> Artifact::getRandomCommonArtifacts(Game* game)
{
	std::vector<Artifact*> artifacts;
	const int NUM_ARTIFACTS = 3;
	for (int i : ddutil::uniqueRandom(0, 15, NUM_ARTIFACTS))
	{
		switch (i)
		{
		case 0:
			artifacts.push_back(new PurpleShackles(game));
			break;
		case 1:
			artifacts.push_back(new PurpleEnergy(game));
			break;
		case 2:
			artifacts.push_back(new RedShackles(game));
			break;
		case 3:
			artifacts.push_back(new RedEnergy(game));
			break;
		case 4:
			artifacts.push_back(new GreenShackles(game));
			break;
		case 5:
			artifacts.push_back(new GreenEnergy(game));
			break;
		case 6:
			artifacts.push_back(new BrandingIron(game));
			break;
		case 7:
			artifacts.push_back(new EnergyBar(game));
			break;
		case 8:
			artifacts.push_back(new Fungus(game));
			break;
		case 9:
			artifacts.push_back(new CorruptIdol(game));
			break;
		case 10:
			artifacts.push_back(new Ectoplasm(game));
			break;
		case 11:
			artifacts.push_back(new Meat(game));
			break;
		case 12:
			artifacts.push_back(new ShriveledSeed(game));
			break;
		case 13:
			artifacts.push_back(new LightningInABottle(game));
			break;
		case 14:
			artifacts.push_back(new Cookie(game));
			break;
		case 15:
			artifacts.push_back(new WaxWings(game));
			break;
		}
	}
	return artifacts;
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
	case ArtifactID::PurpleShackles:
		return new PurpleShackles(game);
	case ArtifactID::PurpleEnergy:
		return new PurpleEnergy(game);
	case ArtifactID::RedShackles:
		return new RedShackles(game);
	case ArtifactID::RedEnergy:
		return new RedEnergy(game);
	case ArtifactID::GreenShackles:
		return new GreenShackles(game);
	case ArtifactID::GreenEnergy:
		return new GreenEnergy(game);
	case ArtifactID::BrandingIron:
		return new BrandingIron(game);
	case ArtifactID::EnergyBar:
		return new EnergyBar(game);
	case ArtifactID::Fungus:
		return new Fungus(game);
	case ArtifactID::CorruptIdol:
		return new CorruptIdol(game);
	case ArtifactID::Ectoplasm:
		return new Ectoplasm(game);
	case ArtifactID::Meat:
		return new Meat(game);
	case ArtifactID::ShriveledSeed:
		return new ShriveledSeed(game);
	case ArtifactID::LightningInABottle:
		return new LightningInABottle(game);
	case ArtifactID::Cookie:
		return new Cookie(game);
	case ArtifactID::WaxWings:
		return new WaxWings(game);
	case ArtifactID::ThornedArmor:
		return new ThornedArmor(game);
	case ArtifactID::TikiTotem:
		return new TikiTotem(game);
	case ArtifactID::StarCannon:
		return new StarCannon(game);
	case ArtifactID::BloodyTotem:
		return new BloodyTotem(game);
	case ArtifactID::CursedTome:
		return new CursedTome(game);
	case ArtifactID::ShipInABottle:
		return new ShipInABottle(game);
	case ArtifactID::MonkeysPaw:
		return new MonkeysPaw(game);
	case ArtifactID::Singularity:
		return new Singularity(game);
	case ArtifactID::BeserkersBrew:
		return new BeserkersBrew(game);
	case ArtifactID::NomadsMat:
		return new NomadsMat(game);
	case ArtifactID::NinjasCaltrops:
		return new NinjasCaltrops(game);
	case ArtifactID::ClericsRobes:
		return new ClericsRobes(game);
	case ArtifactID::SummonersOrb:
		return new SummonersOrb(game);
	case ArtifactID::TempestStaff:
		return new TempestsStaff(game);
	case ArtifactID::BrawlersBelt:
		return new BrawlersBelt(game);
	case ArtifactID::GamblersDeck:
		return new GamblersDeck(game);
	case ArtifactID::SharpshootersSack:
		return new SharpshootersSack(game);
	default:
		return nullptr;
	}
}

bool Artifact::operator<(const Artifact& other) const
{
	if (strength != other.strength)
	{
		return static_cast<int>(strength) < static_cast<int>(other.strength);
	}
	else
	{
		return (name.getString() < other.name.getString());
	}
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
			" (cannot increase beyond " +
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

CommonArtifact::CommonArtifact(std::string name, ColorString description, ArtifactID theID, Game* theGame)
	:Artifact(name, description, theID, Strength::Weak, ddutil::WEAK_COLOR, theGame)
{
}

PurpleShackles::PurpleShackles(Game* game)
	:CommonArtifact(
		"Purple Shackles",
		ColorString("Receive ", ddutil::TEXT_COLOR) + ColorString(std::to_string(VIT_PER_TURN) + " vitality ", ddutil::VITALITY_COLOR) +
			ColorString("per turn in the next battle, but gain ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(XP)+" XP ", ddutil::EXPERIENCE_COLOR) +
			ColorString("now", ddutil::TEXT_COLOR),
		ArtifactID::PurpleShackles,
		game
	)
{
	
}

ColorString PurpleShackles::startOfBattleAction(Player* player, Enemy* enemy)
{
	if (!isConsumed())
	{
		player->adjustVitalityGainTemp(VIT_PER_TURN);
		setAsConsumed();
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
			ColorString(" reduce the ", ddutil::TEXT_COLOR) + player->getColorString() +
			ColorString("'s", player->getColor()) + 
			ColorString(" vitality gain by " + std::to_string(VIT_PER_TURN), ddutil::VITALITY_COLOR);
	}
	return ColorString();
}

void PurpleShackles::equipAction(Player* player)
{
	player->gainExperience(XP);
}

PurpleEnergy::PurpleEnergy(Game* game)
	:CommonArtifact(
		"Purple Energy",
		ColorString("Start with ", ddutil::TEXT_COLOR) + ColorString(std::to_string(START_VIT) + " vitality ", ddutil::VITALITY_COLOR) +
			ColorString("in the next battle, but gain ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(XP)+" XP ", ddutil::EXPERIENCE_COLOR) +
			ColorString("now", ddutil::TEXT_COLOR),
		ArtifactID::PurpleEnergy,
		game
	)
{
}

void PurpleEnergy::equipAction(Player* player)
{
	player->gainExperience(XP);
}

ColorString PurpleEnergy::startOfBattleAction(Player* player, Enemy* enemy)
{
	if (!isConsumed())
	{
		player->setVitality(START_VIT);
		setAsConsumed();
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
			ColorString(" reduces the ", ddutil::TEXT_COLOR) + player->getColorString() +
			ColorString("'s", player->getColor()) + 
			ColorString(" vitality to " + std::to_string(START_VIT), ddutil::VITALITY_COLOR);
	}
	return ColorString();
}

RedShackles::RedShackles(Game* game)
	:CommonArtifact(
		"Red Shackles",
		ColorString("Receive ", ddutil::TEXT_COLOR) + ColorString(std::to_string(VIT_PER_TURN) + " vitality ", ddutil::VITALITY_COLOR) +
			ColorString("per turn in the next battle, but gain ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(MAX_HP)+" Max HP ", ddutil::HEAL_COLOR) +
			ColorString("now", ddutil::TEXT_COLOR),
		ArtifactID::RedShackles,
		game
	)
{
}

ColorString RedShackles::startOfBattleAction(Player* player, Enemy* enemy)
{
	if (!isConsumed())
	{
		player->adjustVitalityGainTemp(VIT_PER_TURN);
		setAsConsumed();
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
			ColorString(" reduce the ", ddutil::TEXT_COLOR) + player->getColorString() +
			ColorString("'s", player->getColor()) + 
			ColorString(" vitality gain by " + std::to_string(VIT_PER_TURN), ddutil::VITALITY_COLOR);
	}
	return ColorString();
}

void RedShackles::equipAction(Player* player)
{
	player->increaseMaxHealth(MAX_HP);
}

RedEnergy::RedEnergy(Game* game)
	:CommonArtifact(
		"Red Energy",
		ColorString("Start with ", ddutil::TEXT_COLOR) + ColorString(std::to_string(START_VIT) + " vitality ", ddutil::VITALITY_COLOR) +
			ColorString("in the next battle, but gain ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(MAX_HP)+" Max HP ", ddutil::HEAL_COLOR) +
			ColorString("now", ddutil::TEXT_COLOR),
		ArtifactID::RedEnergy,
		game
	)
{
}

ColorString RedEnergy::startOfBattleAction(Player* player, Enemy* enemy)
{
	if (!isConsumed())
	{
		player->setVitality(START_VIT);
		setAsConsumed();
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
			ColorString(" reduces the ", ddutil::TEXT_COLOR) + player->getColorString() +
			ColorString("'s", player->getColor()) + 
			ColorString(" vitality to " + std::to_string(START_VIT), ddutil::VITALITY_COLOR);
	}
	return ColorString();
}

void RedEnergy::equipAction(Player* player)
{
	player->increaseMaxHealth(MAX_HP);
}

GreenShackles::GreenShackles(Game* game)
	:CommonArtifact(
		"Green Shackles",
		ColorString("Receive ", ddutil::TEXT_COLOR) + ColorString(std::to_string(VIT_PER_TURN) + " vitality ", ddutil::VITALITY_COLOR) +
			ColorString("per turn in the next battle, but heal ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(HEAL)+" HP ", ddutil::HEAL_COLOR) +
			ColorString("now", ddutil::TEXT_COLOR),
		ArtifactID::GreenShackles,
		game
	)
{
}

ColorString GreenShackles::startOfBattleAction(Player* player, Enemy* enemy)
{
	if (!isConsumed())
	{
		player->adjustVitalityGainTemp(VIT_PER_TURN);
		setAsConsumed();
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
			ColorString(" reduce the ", ddutil::TEXT_COLOR) + player->getColorString() +
			ColorString("'s", player->getColor()) + 
			ColorString(" vitality gain by " + std::to_string(VIT_PER_TURN), ddutil::VITALITY_COLOR);
	}
	return ColorString();
}

void GreenShackles::equipAction(Player* player)
{
	player->increaseHealth(HEAL);
}

GreenEnergy::GreenEnergy(Game* game)
	:CommonArtifact(
		"Green Energy",
		ColorString("Start with ", ddutil::TEXT_COLOR) + ColorString(std::to_string(START_VIT) + " vitality ", ddutil::VITALITY_COLOR) +
			ColorString("in the next battle, but heal ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(HEAL)+" HP ", ddutil::HEAL_COLOR) +
			ColorString("now", ddutil::TEXT_COLOR),
		ArtifactID::GreenEnergy,
		game
	)
{
}

ColorString GreenEnergy::startOfBattleAction(Player* player, Enemy* enemy)
{
	if (!isConsumed())
	{
		player->setVitality(START_VIT);
		setAsConsumed();
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
			ColorString(" reduces the ", ddutil::TEXT_COLOR) + player->getColorString() +
			ColorString("'s", player->getColor()) + 
			ColorString(" vitality to " + std::to_string(START_VIT), ddutil::VITALITY_COLOR);
	}
	return ColorString();
}

void GreenEnergy::equipAction(Player* player)
{
	player->increaseHealth(HEAL);
}

BrandingIron::BrandingIron(Game* game)
	:CommonArtifact(
		"Branding Iron",
		ColorString("Take "+std::to_string(DAMAGE)+ " damage", ddutil::DAMAGE_COLOR) + ColorString(" and ", ddutil::TEXT_COLOR) +
			ColorString(" increase base block by "+std::to_string(BBLOCK_INC), ddutil::BLOCK_COLOR) +
			ColorString(" (Cannot kill)", ddutil::TEXT_COLOR),
		ArtifactID::BrandingIron,
		game
	)
{
}

void BrandingIron::equipAction(Player* player)
{
	player->reduceHealth(DAMAGE, nullptr, true);
	if (player->getHealth() < 0)
	{
		player->setHealth(1);
	}
	player->increaseBaseBlock(BBLOCK_INC);
}

EnergyBar::EnergyBar(Game* game)
	:CommonArtifact(
		"Energy Bar",
		ColorString("Start the next battle with ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(VIT_INC) + " additional vitality", ddutil::VITALITY_COLOR),
		ArtifactID::EnergyBar,
		game
	)
{
}

ColorString EnergyBar::startOfBattleAction(Player* player, Enemy* enemy)
{
	if (!isConsumed())
	{
		player->addVitality(VIT_INC);
		setAsConsumed();
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() + 
			ColorString(" gives the ", ddutil::TEXT_COLOR) + player->getColorString() +
			ColorString(" " + std::to_string(VIT_INC) + " vitality", ddutil::VITALITY_COLOR);
	}
	return ColorString();
}

Fungus::Fungus(Game* game)
	:CommonArtifact(
		"Fungus",
		ColorString("Receive ", ddutil::TEXT_COLOR) +
			ColorString("+" + std::to_string(VIT_PER_TURN_GAIN) + " vitality gain", ddutil::VITALITY_COLOR) +
			ColorString(" next battle, but ", ddutil::TEXT_COLOR) +
			ColorString("lose " + std::to_string(HP_COST) + " HP ", ddutil::DAMAGE_COLOR) +
			ColorString("now (Cannot Kill)", ddutil::TEXT_COLOR),
		ArtifactID::Fungus,
		game
	)
{
}

ColorString Fungus::startOfBattleAction(Player* player, Enemy* enemy)
{
	if (!isConsumed())
	{
		player->adjustVitalityGainTemp(VIT_PER_TURN_GAIN);
		setAsConsumed();
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
			ColorString(" increases the ", ddutil::TEXT_COLOR) + player->getColorString() +
			ColorString("'s", player->getColor()) + 
			ColorString(" vitality gain by " + std::to_string(VIT_PER_TURN_GAIN), ddutil::VITALITY_COLOR);
	}
	return ColorString();
}

void Fungus::equipAction(Player* player)
{
	player->reduceHealth(HP_COST, nullptr, true);
	if (player->getHealth() < 0)
	{
		player->setHealth(1);
	}
}

CorruptIdol::CorruptIdol(Game* game)
	:CommonArtifact(
		"Corrupt Idol",
		ColorString("Receive ", ddutil::TEXT_COLOR) +
			ColorString("+" + std::to_string(VIT_PER_TURN_GAIN) + " vitality gain", ddutil::VITALITY_COLOR) +
			ColorString(" next battle, but ", ddutil::TEXT_COLOR) +
			ColorString("receive no XP", ddutil::EXPERIENCE_COLOR),
		ArtifactID::CorruptIdol,
		game
	)
{
}

ColorString CorruptIdol::startOfBattleAction(Player* player, Enemy* enemy)
{
	if (!isConsumed())
	{
		player->adjustVitalityGainTemp(VIT_PER_TURN_GAIN);
		player->setShouldReceiveXP(false);
		setAsConsumed();
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
			ColorString(" increases the ", ddutil::TEXT_COLOR) + player->getColorString() +
			ColorString("'s", player->getColor()) +
			ColorString(" vitality gain by " + std::to_string(VIT_PER_TURN_GAIN), ddutil::TEXT_COLOR) +
			ColorString(", but will steal their ", ddutil::TEXT_COLOR) + ColorString("XP", ddutil::EXPERIENCE_COLOR);
	}
	return ColorString();
}

Ectoplasm::Ectoplasm(Game* game)
	:CommonArtifact(
		"Ectoplasm",
		ColorString("Increases dodge chance by "+std::to_string(DODGE_INC)+"%", ddutil::TEXT_COLOR),
		ArtifactID::Ectoplasm,
		game
	)
{
}

void Ectoplasm::equipAction(Player* player)
{
	player->increaseBaseDodgeChance(DODGE_INC);
}

Meat::Meat(Game* game)
	:CommonArtifact(
		"Meat",
		ColorString("Heals "+std::to_string(HEAL)+" HP", ddutil::HEAL_COLOR),
		ArtifactID::Meat,
		game
	)
{
}

void Meat::equipAction(Player* player)
{
	player->increaseHealth(HEAL);
	setAsConsumed();
}

ShriveledSeed::ShriveledSeed(Game* game)
	:CommonArtifact(
		"Shriveled Seed",
		ColorString("Increases Max HP by "+std::to_string(MAX_HP_INC), ddutil::HEAL_COLOR),
		ArtifactID::ShriveledSeed,
		game
	)
{
}

void ShriveledSeed::equipAction(Player* player)
{
	player->increaseMaxHealth(MAX_HP_INC);
}

LightningInABottle::LightningInABottle(Game* game)
	:CommonArtifact(
		"Lightning in a Bottle",
		ColorString("Reduces the next enemy's HP by "+std::to_string(PERC_DAM)+"%", ddutil::DAMAGE_COLOR),
		ArtifactID::LightningInABottle,
		game
	)
{
}

ColorString LightningInABottle::startOfBattleAction(Player* player, Enemy* enemy)
{
	if (!isConsumed())
	{
		int damage = static_cast<int>(enemy->getMaxHealth(100) * (PERC_DAM / 100.0));
		ddutil::DamageReport damRep = enemy->reduceHealth(damage, nullptr, true);
		setAsConsumed();
		playSound(WavFile("lightning", false, true));
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
			ColorString(" deals ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep) +
			ColorString(" to the ", ddutil::TEXT_COLOR) + enemy->getColorString();
	}
	return ColorString();
}

Cookie::Cookie(Game* game)
	:CommonArtifact(
		"Cookie",
		ColorString("Removes all status effects at the start of battle once", ddutil::TEXT_COLOR),
		ArtifactID::Cookie,
		game
	)
{
}

ColorString Cookie::startOfBattleAction(Player* player, Enemy* enemy)
{
	if (!isConsumed())
	{
		player->clearAllStatuses();
		setAsConsumed();
		return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
			ColorString(" clears the ", ddutil::TEXT_COLOR) + player->getColorString() +
			ColorString(" of all their status effects", ddutil::TEXT_COLOR);
	}
	return ColorString();
}

WaxWings::WaxWings(Game* game)
	:CommonArtifact(
		"Wax Wings",
		ColorString("Ignore path restrictions when choosing rooms. Destroyed upon taking damage.", ddutil::TEXT_COLOR),
		ArtifactID::WaxWings,
		game
	)	
{
}

ThornedArmor::ThornedArmor(Game* game)
	:PowerfulArtifact(
		"Thorned Armor",
		ColorString("Applies ", ddutil::TEXT_COLOR) + ColorString("Thorned " +std::to_string(THORNS), ThornsStatus::COLOR),
		ArtifactID::ThornedArmor, game
	)
{
}

void ThornedArmor::equipAction(Player* player)
{
	player->addSelfStartingStatus(new ThornsStatus(), THORNS);
}

TikiTotem::TikiTotem(Game* game)
	:ModerateArtifact(
		"Tiki Totem",
		ColorString("Deals ", ddutil::TEXT_COLOR) + ColorString(std::to_string(DAMAGE)+" damage ", ddutil::DAMAGE_COLOR) +
			ColorString("at the start of battle", ddutil::TEXT_COLOR),
		ArtifactID::TikiTotem,
		game
	)
{
}

ColorString TikiTotem::startOfBattleAction(Player* player, Enemy* enemy)
{
	ddutil::DamageReport damRep = enemy->reduceHealth(DAMAGE, nullptr, true);
	playSound(WavFile("vulnerable", false, true));
	return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
		ColorString(" deals ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep) +
		ColorString(" to the ", ddutil::TEXT_COLOR) + enemy->getColorString();
}

StarCannon::StarCannon(Game* game)
	:MythicalArtifact(
		"Star Cannon",
		ColorString("Deals ", ddutil::TEXT_COLOR) +
			ColorString(std::to_string(DAM) + " damage", ddutil::TEXT_COLOR) + 
			ColorString(" per ", ddutil::TEXT_COLOR) + ColorString(std::to_string(XP)+ " XP", ddutil::EXPERIENCE_COLOR) +
			ColorString(" at the start of battle", ddutil::TEXT_COLOR),
		ArtifactID::StarCannon,
		game
	)
{
}

ColorString StarCannon::startOfBattleAction(Player* player, Enemy* enemy)
{
	int damage = static_cast<int>(DAM * (player->getExperience() / static_cast<double>(XP)));
	ddutil::DamageReport damRep = enemy->reduceHealth(damage, nullptr, true);
	playSound(WavFile("magicattack3", false, true));
	return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
		ColorString(" deals ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep) +
		ColorString(" to the ", ddutil::TEXT_COLOR) + enemy->getColorString();
}

BloodyTotem::BloodyTotem(Game* game)
	:ModerateArtifact(
		"Bloody Totem", 
		ColorString("Heals " + std::to_string(HEAL) + " HP ", ddutil::HEAL_COLOR) +
			ColorString("at the start of every battle", ddutil::TEXT_COLOR),
		ArtifactID::BloodyTotem,
		game
	)
{
}

ColorString BloodyTotem::startOfBattleAction(Player* player, Enemy* enemy)
{
	int healAmount = player->increaseHealth(HEAL);
	return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
		ColorString(" heals ", ddutil::HEAL_COLOR) + ColorString("the ", ddutil::TEXT_COLOR) +
		player->getColorString() + ColorString(" for " + std::to_string(healAmount) + " HP", ddutil::HEAL_COLOR);
}

CursedTome::CursedTome(Game* game)
	:BossArtifact(
		"Cursed Tome",
		ColorString("Gives " + std::to_string(VIT_PER_TURN_GAIN) + " vitality per turn ", ddutil::VITALITY_COLOR) + 
			ColorString("but reduces move selection by " + std::to_string(MOVES_TO_CHOOSE_REDUCTION),ddutil::TEXT_COLOR),
		ArtifactID::CursedTome,
		game
	)
{
}

void CursedTome::equipAction(Player* player)
{
	player->increaseMovesToChooseFrom(MOVES_TO_CHOOSE_REDUCTION);
	player->increaseVitalityPerTurn(VIT_PER_TURN_GAIN);
}

ShipInABottle::ShipInABottle(Game* game)
	:BossArtifact(
		"Ship in a Bottle",
		ColorString("Once per battle, deals "+std::to_string(PER_DAM)+"% of enemy's max HP as damage", ddutil::DAMAGE_COLOR) +
			ColorString(" & applies ",ddutil::TEXT_COLOR) + ColorString("Storm 10"+std::to_string(STORM_LEN), StormStatus::COLOR),
		ArtifactID::ShipInABottle,
		game
	)
{
}

ColorString ShipInABottle::startOfBattleAction(Player* player, Enemy* enemy)
{
	int damage = static_cast<int>(enemy->getMaxHealth(100) * (PER_DAM / 100.0));
	ddutil::DamageReport damRep = enemy->reduceHealth(damage, nullptr, true);
	playSound(WavFile("bottleship", false, true));
	enemy->applyStatus(new StormStatus(), STORM_LEN);
	return ColorString("The ", ddutil::TEXT_COLOR) + getName() +
		ColorString(" deals ", ddutil::TEXT_COLOR) + ddutil::genericDamageAlert(damRep) +
		ColorString(" to the ", ddutil::TEXT_COLOR) + enemy->getColorString();
}


MonkeysPaw::MonkeysPaw(Game* game)
	:BossArtifact(
		"Monkey's Paw",
		ColorString("Doubles Max HP and sets HP to max", ddutil::HEAL_COLOR) + 
			ColorString(", but", ddutil::TEXT_COLOR) + 
			ColorString(" reduces all future healing by 75%", ddutil::DAMAGE_COLOR),
		ArtifactID::MonkeysPaw,
		game
	)
{
}

void MonkeysPaw::equipAction(Player* player)
{
	player->increaseMaxHealth(player->getMaxHealth(100));
	player->setHealth(player->getMaxHealth(100));
	player->increasePercentHealBoost(-75);
}

Singularity::Singularity(Game* game)
	:BossArtifact(
		"Singularity",
		ColorString("Bestows ultimate cosmic power", ddutil::COSMIC_COLOR) + 
			ColorString(", but ", ddutil::TEXT_COLOR) +
			ColorString("halves Max HP and wipes all current moves", ddutil::DAMAGE_COLOR),
		ArtifactID::Singularity,
		game
	)
{
}

void Singularity::equipAction(Player* player)
{
	player->decreaseMaxHealth(player->getMaxHealth(50));
	player->cosmicAscension();
}

BeserkersBrew::BeserkersBrew(Game* game)
	:ModerateArtifact(
		"Beserker's Brew",
		ColorString("Gives ", ddutil::TEXT_COLOR) + ColorString("Beserked ("+std::to_string(LENGTH)+")", BeserkedStatus::COLOR) +
			ColorString(" at the start of battle", ddutil::TEXT_COLOR),
		ArtifactID::BeserkersBrew,
		game
	)
{
}

void BeserkersBrew::equipAction(Player* player)
{
	player->addSelfStartingStatus(new BeserkedStatus(), LENGTH);
}

NomadsMat::NomadsMat(Game* game)
	:ModerateArtifact(
		"Nomad's Mat",
		ColorString("Start battle in ",ddutil::TEXT_COLOR) + ColorString("Zen ("+std::to_string(LENGTH)+")", ZenStatus::COLOR),
		ArtifactID::NomadsMat,
		game
	)
{
}

void NomadsMat::equipAction(Player* player)
{
	player->addSelfStartingStatus(new ZenStatus(), LENGTH);
}

NinjasCaltrops::NinjasCaltrops(Game* game)
	:ModerateArtifact(
		"Ninja's Caltrops",
		ColorString("Gives ", ddutil::TEXT_COLOR) + ColorString("Thorned ("+std::to_string(THORNS)+")", ThornsStatus::COLOR) +
			ColorString(" and increases dodge chance by "+std::to_string(DODGE_INC)+"%", ddutil::TEXT_COLOR),
		ArtifactID::NinjasCaltrops,
		game
	)
{
}

void NinjasCaltrops::equipAction(Player* player)
{
	player->increaseBaseDodgeChance(DODGE_INC);
	player->addSelfStartingStatus(new ThornsStatus(), THORNS);
}

ClericsRobes::ClericsRobes(Game* game)
	:ModerateArtifact(
		"Cleric's Robes",
		ColorString("Gain " +std::to_string(MAX_HP_INC) +" max HP", ddutil::HEAL_COLOR) + ColorString(" if ending a fight at full HP", ddutil::TEXT_COLOR),
		ArtifactID::ClericsRobes,
		game
	)
{
}

SummonersOrb::SummonersOrb(Game* game)
	:ModerateArtifact(
		"Summoner's Orb",
		ColorString("When using a summon move, gain ", ddutil::TEXT_COLOR) + ColorString(std::to_string(BLOCK) + " block", ddutil::BLOCK_COLOR) +
			ColorString(" and heal "+std::to_string(HEAL) + " HP", ddutil::HEAL_COLOR),
		ArtifactID::SummonersOrb,
		game
	)
{
}

TempestsStaff::TempestsStaff(Game* game)
	:ModerateArtifact(
		"Tempest's Staff",
		ColorString("When dealing damage, gain back ", ddutil::TEXT_COLOR) + ColorString(std::to_string(VIT)+" vitality", ddutil::VITALITY_COLOR),
		ArtifactID::TempestStaff,
		game
	)
{

}

BrawlersBelt::BrawlersBelt(Game* game)
	:ModerateArtifact(
		"Brawler's Belt",
		ColorString("If ending a turn with 0 vitality, ", ddutil::TEXT_COLOR) + ColorString("gain "+std::to_string(VIT_INC)+" extra vitality", ddutil::VITALITY_COLOR),
		ArtifactID::BrawlersBelt,
		game
	)
{
}

GamblersDeck::GamblersDeck(Game* game)
	:ModerateArtifact(
		"Gambler's Deck",
		ColorString("Replace one ", ddutil::TEXT_COLOR) + ColorString("Peashooter", ddutil::WEAK_COLOR) + 
			ColorString(" with ",ddutil::TEXT_COLOR) + ColorString("Card Throw", ddutil::MODERATE_COLOR) + 
			ColorString(" and start battle with a random card", ddutil::TEXT_COLOR),
		ArtifactID::GamblersDeck,
		game
	)
{
}

void GamblersDeck::equipAction(Player* player)
{
	player->removeFirstMove();
	player->addMove(new GunslingerMoves::CardThrow());
}

ColorString GamblersDeck::startOfBattleAction(Player* player, Enemy* enemy)
{
	Status* status = CardStatus::getRandomCard();
	player->applyStatus(status);
	return ColorString("The ", ddutil::TEXT_COLOR) + player->getColorString() +
		ColorString(" draws the ", ddutil::TEXT_COLOR) + status->getName() +
		ColorString(" from the ", ddutil::TEXT_COLOR) + getName();
}

SharpshootersSack::SharpshootersSack(Game* game)
	:ModerateArtifact(
		"Sharpshooter's Sack",
		ColorString("Gives " + std::to_string(BULLETS)+" bullets at the start of battle", ddutil::TEXT_COLOR),
		ArtifactID::SharpshootersSack,
		game
	)
{
}

ColorString SharpshootersSack::startOfBattleAction(Player* player, Enemy* enemy)
{
	Gunslinger* gunman = dynamic_cast<Gunslinger*>(player);
	if (gunman == nullptr)
	{
		return ColorString("The ", ddutil::TEXT_COLOR) + player->getColorString() + ColorString(" cannot use the ", ddutil::TEXT_COLOR) +
			getName();
	}
	else
	{
		gunman->createBullets(BULLETS);
		return ColorString("The ", ddutil::TEXT_COLOR) + player->getColorString() + ColorString(" gets " + std::to_string(BULLETS) + " bullets from the ", ddutil::TEXT_COLOR) +
			getName();
	}
}
