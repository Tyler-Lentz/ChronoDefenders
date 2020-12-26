#ifndef ARTIFACT_H
#define ARTIFACT_H

#include <string>
#include "colorstring.h"
#include "move.h"
#include "utilities.h"

class Game;
class Player;

enum class ArtifactID
{
	// Moderate
	RustyDagger, // event

	PurpleAmulet,
	SwiftnessBracelet,
	MagicBean,
	MagicPowder,
	AncientBook,
	Chainmail,
	SmellingSalts,

	// Powerful
	SpectralAmulet,
	SacrificialKnife,
	BurningOrb,
	FrozenOrb,
	QuailFeather,
	MagicPotion,
	MagicQuill,
	AncientTome,
	WarriorHelm,
	Shuriken,
	PotentPowder,
	ElectricOrb,

	AncientMask, // event

	// Mythical
	PoisonVial,
	Katana,
	AncientWand,
	EternalSeed,
	Spurs,
	AncientScroll,
	AdamantiteShield,
	VitalStone,

	EtherealDagger, // event
	
	
	// Boss Artifacts ( give vitality per turn but have downsides)
	Tesseract, // +2 vitality per turn but -10 max hp
	MagicAmulet, // + 1 vitality no downside
	PossessedDoll, // +2 vitality but start battle with hex
	DarkSeed, // +2 vitality but -50% healing
	MagicLeech, // +2 vitality but -33% experience
	BattleStandard
};

class Artifact
{
public:
	Artifact(std::string theName, ColorString theDescription, ArtifactID theID, Strength str, int color, Game* theGame);

	virtual void equipAction(Player* player) = 0;
	virtual void playFindSound() = 0;

	ColorString getFullInformation();
	ColorString getName();
	ColorString getDescription();
	ArtifactID getID();

	static Artifact* generateRandomArtifact(Strength strength);
	static Artifact* getArtifactFromID(Game* game, ArtifactID id);

	// for use of sorting in the artifact menu
	bool operator<(const Artifact& other) const;
private:
	ColorString name;
	ColorString description;
	Game* game;
	ArtifactID id;
	Strength strength;
	int color;
};

class ModerateArtifact : public Artifact
{
public:
	ModerateArtifact(std::string name, ColorString description, ArtifactID theID, Game* theGame);
	void playFindSound() override;
};

class RustyDagger : public ModerateArtifact
{
public:
	RustyDagger(Game* theGame);

	void equipAction(Player* player) override;

	const static int BLEED_AMOUNT = 1;
private:

};

class PurpleAmulet : public ModerateArtifact
{
public:
	PurpleAmulet(Game* theGame);

	void equipAction(Player* player) override;

	const static int EXP_PERCENT_GAIN = 20;
};

class SwiftnessBracelet : public ModerateArtifact
{
public:
	SwiftnessBracelet(Game* theGame);

	void equipAction(Player* player) override;

	const static int DODGE_CHANCE = 5;
};

class MagicBean : public ModerateArtifact
{
public:
	MagicBean(Game* theGame);

	void equipAction(Player* player) override;

	const static int MAX_HEALTH_INCREASE = 5;
};

class MagicPowder : public ModerateArtifact
{
public:
	MagicPowder(Game* game);

	void equipAction(Player* player) override;

	const static int S_VITALITY_INCREASE = 1;
};

class AncientBook : public ModerateArtifact
{
public:
	AncientBook(Game* game);

	void equipAction(Player* player) override;

	const static int EXP_GAIN = ddutil::MODERATE_COST;
};

class Chainmail : public ModerateArtifact
{
public:
	Chainmail(Game* game);
	void equipAction(Player* player) override;

	const static int BLOCK_GAIN = 2;
};

class SmellingSalts : public ModerateArtifact
{
public:
	SmellingSalts(Game* game);
	void equipAction(Player* player) override;

	const static int CARD_CHOICE_GAIN = 1;
};

class PowerfulArtifact : public Artifact
{
public:
	PowerfulArtifact(std::string name, ColorString description, ArtifactID theID, Game* theGame);
	void playFindSound() override;
};

class SpectralAmulet : public PowerfulArtifact
{
public:
	SpectralAmulet(Game* game);

	void equipAction(Player* player) override;

	const static int INVULN_TURNS = 1;
};

class SacrificialKnife : public PowerfulArtifact
{
public:
	SacrificialKnife(Game* game);

	void equipAction(Player* player) override;

	const static int VULN_TURNS = 1;
};

class BurningOrb : public PowerfulArtifact
{
public:
	BurningOrb(Game* game);

	void equipAction(Player* player) override;

	const static int BURN_TURNS = 1;
};

class FrozenOrb : public PowerfulArtifact
{
public:
	FrozenOrb(Game* game);

	void equipAction(Player* player) override;

	const static int FBURN_TURNS = 1;
};

class ElectricOrb : public PowerfulArtifact
{
public:
	ElectricOrb(Game* game);

	void equipAction(Player* player) override;
	const static int ZAPPED_TURNS = 1;
};

class QuailFeather : public PowerfulArtifact
{
public:
	QuailFeather(Game* game);

	void equipAction(Player* player) override;

	const static int DODGE_CHANCE = 10;
};

class MagicPotion : public PowerfulArtifact
{
public:
	MagicPotion(Game* game);

	void equipAction(Player* player) override;

	const static int MAX_HP_INC = 10;
};

class MagicQuill : public PowerfulArtifact
{
public:
	MagicQuill(Game* game);
	void equipAction(Player* player) override;

	const static int MOVESET_INCREASE = 1;
};

class AncientTome : public PowerfulArtifact
{
public:
	AncientTome(Game* game);
	void equipAction(Player* player) override;

	const static int EXP_GAIN = ddutil::POWERFUL_COST;
};

class WarriorHelm : public PowerfulArtifact
{
public:
	WarriorHelm(Game* game);
	void equipAction(Player* player) override;

	const static int BLOCK_AMOUNT = 4;
};

class Shuriken : public PowerfulArtifact
{
public:
	Shuriken(Game* game);
	void equipAction(Player* player) override;

	const static int BLEED_AMOUNT = 2;
};

class PotentPowder : public PowerfulArtifact
{
public:
	PotentPowder(Game* game);
	void equipAction(Player* player) override;

	const static int S_VIT_GAIN = 2;
};

class AncientMask : public PowerfulArtifact
{
public:
	AncientMask(Game* game);
	void equipAction(Player* player) override;

	const static int STUN_LENGTH = 1;
	const static int S_VIT_GAIN = 3;
};

class MythicalArtifact : public Artifact
{
public:
	MythicalArtifact(std::string name, ColorString description, ArtifactID theID, Game* theGame);
	void playFindSound() override;
};

class PoisonVial : public MythicalArtifact
{
public:
	PoisonVial(Game* game);

	void equipAction(Player* player) override;

	const static int POISON_AMOUNT = 1;
};



class Katana : public MythicalArtifact
{
public:
	Katana(Game* game);
	void equipAction(Player* player) override;
	const static int BLEED_AMOUNT = 3;
};

class AncientWand : public MythicalArtifact
{
public:
	AncientWand(Game* game);
	void equipAction(Player* player) override;
	const static int STATUS_AMOUNT = 1;
};

class VitalStone : public MythicalArtifact
{
public:
	VitalStone(Game* game);
	void equipAction(Player* player) override;
	const static int MAX_HP_INC = 15;
};

class EternalSeed : public MythicalArtifact
{
public:
	EternalSeed(Game* game);
	void equipAction(Player* player) override;
	const static int PERCENT_HEAL_GAIN = 50;
};

class Spurs : public MythicalArtifact
{
public:
	Spurs(Game* game);
	void equipAction(Player* player) override;
};

class AncientScroll : public MythicalArtifact
{
public:
	AncientScroll(Game* game);
	void equipAction(Player* player) override;
	const static int EXP_GAIN = ddutil::MYTHICAL_COST;
};

class AdamantiteShield : public MythicalArtifact
{
public:
	AdamantiteShield(Game* game);
	void equipAction(Player* player) override;
	const static int BLOCK_PER_TURN = 8;
};

class EtherealDagger : public MythicalArtifact
{
public:
	EtherealDagger(Game* game);
	void equipAction(Player* player) override;
	const static int VULN_AMOUNT = 1;
};


// Boss Artifacts
class BossArtifact : public Artifact
{
public:
	BossArtifact(std::string name, ColorString description, ArtifactID theID, Game* theGame);
	void playFindSound() override;
};

class Tesseract : public BossArtifact
{
public:
	Tesseract(Game* game);
	void equipAction(Player* player) override;
	const static int VIT_PER_TURN_GAIN = 2;
	const static int MAX_HP_REDUCTION = 15;
};

class MagicAmulet : public BossArtifact
{
public:
	MagicAmulet(Game* game);
	void equipAction(Player* player) override;

	const static int VIT_PER_TURN_GAIN = 1;
};

class PossessedDoll : public BossArtifact
{
public:
	PossessedDoll(Game* game);
	void equipAction(Player* player) override;

	const static int VIT_PER_TURN_GAIN = 2;
	const static int HEX_TURNS = 2;
};

class DarkSeed : public BossArtifact
{
public:
	DarkSeed(Game* game);
	void equipAction(Player* player) override;

	const static int VIT_PER_TURN_GAIN = 2;
	const static int HEALING_REDUCTION = 50;
};

class MagicLeech : public BossArtifact
{
public:
	MagicLeech(Game* game);
	void equipAction(Player* player) override;

	const static int VIT_PER_TURN_GAIN = 2;
	const static int EXP_REDUCTION = 33;
};

class BattleStandard : public BossArtifact
{
public:
	BattleStandard(Game* game);
	void equipAction(Player* player) override;

	const static int VIT_PER_TURN_GAIN = 2;
	const static int VULN_TURNS = 4;
};

#endif