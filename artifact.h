#ifndef ARTIFACT_H
#define ARTIFACT_H

#include <string>
#include "colorstring.h"
#include "move.h"
#include "utilities.h"

class Game;
class Player;
class Enemy;

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
	BattleStandard,

	// Common Artifacts (Don't actually delete, but toggle a 'consumed' var)
	PurpleShackles, // -1 vitality per turn in next battle, but receive 50XP. Consume on use.	
	PurpleEnergy, // start with 0 vitality in next battle, but receive 50XP. Consume on use.
	RedShackles, // -1 vitality per turn in next battle, but +2 max hp Consume on use.
	RedEnergy, // start with 0 vitality in next battle, but +2 max hp. Consume on use.
	GreenShackles, // - 1 vitality per turn in next battle, but heal 8Hp. Consumed
	GreenEnergy, // start with 0 vitality in next battle, but heal 8hp. Consumed
	BrandingIron, // Take 5 damage, but permanently increase base block by 2. Consume on use.
	EnergyBar, // Start battle with 1 more vitality. Consume on use.
	Fungus, // +1 vitality per turn in next battle, but lose 8HP. Consume on use.
	CorruptIdol, // + 1 vitality per turn in next battle, but receive no XP for the battle. Consume on use.
	Ectoplasm, // +2% dodge chance
	Meat, // heal 4HP at the start of battle. Consume on use.
	ShriveledSeed, // +1 max hp
	LightningInABottle, // deal 10% of enemy's hp as damage. Consume on use.
	Cookie, // clear all status effects at start of battle. Consume on use
	WaxWings, // Ignore room boundary restrictions. Destroyed upon taking damage.
};

class Artifact
{
public:
	Artifact(std::string theName, ColorString theDescription, ArtifactID theID, Strength str, int color, Game* theGame);

	virtual void equipAction(Player* player) = 0;
	virtual ColorString startOfBattleAction(Player* player, Enemy* enemy) { return ColorString(); };
	virtual void playFindSound() = 0;

	ColorString getFullInformation();
	ColorString getName();
	ColorString getDescription();
	ArtifactID getID();

	static std::vector<Artifact*> getRandomCommonArtifacts(Game* game);
	static Artifact* getArtifactFromID(Game* game, ArtifactID id);

	// for use of sorting in the artifact menu
	bool operator<(const Artifact& other) const;

	bool isConsumed();
	void setAsConsumed();
protected:
	ColorString name;
	ColorString description;
	Game* game;
	ArtifactID id;
	Strength strength;
	int color;

private:
	bool consumed;
};

class CommonArtifact : public Artifact
{
public:
	CommonArtifact(std::string name, ColorString description, ArtifactID theID, Game* theGame);
	void playFindSound() override {}; // no find sound
};

class PurpleShackles : public CommonArtifact
{
public:
	PurpleShackles(Game* game);
	ColorString startOfBattleAction(Player* player, Enemy* enemy) override;
	void equipAction(Player* player) override;
	
	const static int VIT_PER_TURN = -1;
	const static int XP = 40;
};

class PurpleEnergy : public CommonArtifact
{
public:
	PurpleEnergy(Game* game);
	void equipAction(Player* player) override;
	ColorString startOfBattleAction(Player* player, Enemy* enemy) override;
	
	const static int START_VIT = 0;
	const static int XP = 40;
};

class RedShackles: public CommonArtifact
{
public:
	RedShackles(Game* game);
	ColorString startOfBattleAction(Player* player, Enemy* enemy) override;
	void equipAction(Player* player) override;
	
	const static int VIT_PER_TURN = -1;
	const static int MAX_HP = 2;
};

class RedEnergy : public CommonArtifact
{
public:
	RedEnergy(Game* game);
	ColorString startOfBattleAction(Player* player, Enemy* enemy) override;
	void equipAction(Player* player) override;

	const static int START_VIT = 0;
	const static int MAX_HP = 2;
};

class GreenShackles : public CommonArtifact
{
public:
	GreenShackles(Game* game);
	ColorString startOfBattleAction(Player* player, Enemy* enemy) override;
	void equipAction(Player* player) override;

	const static int VIT_PER_TURN = -1;
	const static int HEAL = 8;
};

class GreenEnergy : public CommonArtifact
{
public:
	GreenEnergy(Game* game);
	ColorString startOfBattleAction(Player* player, Enemy* enemy) override;
	void equipAction(Player* player) override;

	const static int START_VIT = 0;
	const static int HEAL = 8;
};

class BrandingIron : public CommonArtifact
{
public:
	BrandingIron(Game* game);
	void equipAction(Player* player) override;

	const static int DAMAGE = 5;
	const static int BBLOCK_INC = 2;
};

class EnergyBar : public CommonArtifact
{
public:
	EnergyBar(Game* game);
	ColorString startOfBattleAction(Player* player, Enemy* enemy) override;
	void equipAction(Player* player) override {};

	const static int VIT_INC = 1;
};

class Fungus : public CommonArtifact
{
public:
	Fungus(Game* game);
	ColorString startOfBattleAction(Player* player, Enemy* enemy) override;
	void equipAction(Player* player) override;

	const static int VIT_PER_TURN_GAIN = 1;
	const static int HP_COST = 8;
};

class CorruptIdol : public CommonArtifact
{
public:
	CorruptIdol(Game* game);
	ColorString startOfBattleAction(Player* player, Enemy* enemy) override;
	void equipAction(Player* player) override {};

	const static int VIT_PER_TURN_GAIN = 1;
	// receive no XP
};

class Ectoplasm : public CommonArtifact
{
public:
	Ectoplasm(Game* game);
	void equipAction(Player* player) override;

	const static int DODGE_INC = 1;
};

class Meat : public CommonArtifact
{
public:
	Meat(Game* game);
	void equipAction(Player* player) override;

	const static int HEAL = 4;
};

class ShriveledSeed : public CommonArtifact
{
public:
	ShriveledSeed(Game* game);
	void equipAction(Player* player) override;

	const static int MAX_HP_INC = 1;
};

class LightningInABottle : public CommonArtifact
{
public:
	LightningInABottle(Game* game);
	ColorString startOfBattleAction(Player* player, Enemy* enemy) override;
	void equipAction(Player* player) override {};

	const static int PERC_DAM = 10;
};

class Cookie : public CommonArtifact
{
public:
	Cookie(Game* game);
	ColorString startOfBattleAction(Player* player, Enemy* enemy) override;
	void equipAction(Player* player) override {};
	// remove all statuses at start of battle
};

class WaxWings : public CommonArtifact
{
public:
	WaxWings(Game* game);
	void equipAction(Player* player) override {};
	// acts as a tag for the select room function
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