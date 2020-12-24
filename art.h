#ifndef ART_H
#define ART_H

#include "colorstring.h"
#include "utilities.h"
#include <vector>

class Picture : public std::vector<ColorString>
{
public:
	int getHeight();
	int getLength();
};

class Art
{
public:
	static Picture getTitle();
	const static int TITLE_COLOR = ddutil::CYAN;
	static Picture getPaladin();
	static Picture getGunslinger();
	static Picture getJesterGunslinger();
	static Picture getWhiteBackgroundGunslinger();
	static Picture getSorcerer();
	static Picture getElementalSorceress();
	static Picture getWhiteBackgroundSorcerer();
	static Picture getMinotaur();
	static Picture getFireImp();
	static Picture getIceImp();
	static Picture getFairy();
	static Picture getFireDragon();
	static Picture getIceDragon();
	static Picture getSpiritKnight();
	static Picture getSamurai();
	static Picture getDragonSamurai();
	static Picture getWhiteBackgroundSamurai();
	static Picture getCampfire();
	static Picture getGhost();
	static Picture getBlueGhost();
	static Picture getBrute();
	static Picture getGiantSnail();
	const static int GIANT_SNAIL_COLOR = ddutil::LIGHTGREEN;
	static Picture getYellowGiantSnail();
	static Picture getBurningSkull();
	static Picture getDeadAdventurer();
	static Picture getBrokenMirror();
	static Picture getMirror();
	const static int BROKEN_MIRROR_COLOR = ddutil::BLUE;
	static Picture getDynamite();
	const static int DYNAMITE_COLOR = ddutil::RED;
	static Picture getTheWatcher();
	const static int WATCHER_COLOR = ddutil::MAGENTA;
	static Picture getTheWatcherWhiteBackground();
	static Picture getTreasureChest();
	const static int TREASURE_COLOR = ddutil::YELLOW;

	static Picture getMysteriousKnight();
	const static int MYSTERIOUS_KNIGHT_COLOR = ddutil::DARKGRAY;

	static Picture getMinion();
	const static int MINION_COLOR = ddutil::DARKGRAY;
	static Picture getAltMinion();
	const static int ALT_MINION_COLOR = ddutil::LIGHTGRAY;

	static Picture getHyperBeast();
	const static int HYPER_BEAST_COLOR = ddutil::CYAN;

	static Picture getGiantLizard();
	const static int GIANT_LIZARD_COLOR = ddutil::GREEN;
	static Picture getAltGiantLizard();
	const static int ALT_GIANT_LIZARD_COLOR = ddutil::LIGHTRED;

	static Picture getCollector();
	const static int COLLECTOR_COLOR = ddutil::MAGENTA;

	static Picture getAbyssBeast();
	const static int ABYSS_BEAST_COLOR = ddutil::DARKGRAY;

	static Picture getBasilisk();
	const static int BASILISK_COLOR = ddutil::LIGHTGRAY;

	static Picture getSphynx();
	const static int SPHYNX_COLOR = ddutil::YELLOW;

	static Picture getGoblin();
	const static int GOBLIN_COLOR = ddutil::LIGHTRED;

	static Picture getLavaBeast();
	const static int LAVA_BEAST_COLOR = ddutil::DARKGRAY;

	static Picture getFloatingMouth();
	const static int MOUTH_COLOR = ddutil::LIGHTRED;
	static Picture getMace();
	const static int MACE_COLOR = ddutil::LIGHTMAGENTA;

	static Picture getTheProtector();
	const static int PROTECTOR_COLOR = ddutil::LIGHTCYAN;

	static Picture getPatriarch();
	const static int PATRIARCH_COLOR = ddutil::DARKGRAY;

	static Picture getPortal();
	const static int PORTAL_COLOR = ddutil::BLUE;

	static Picture getPalace();
	const static int PALACE_COLOR = ddutil::YELLOW;

	static Picture getLaughingMask();
	const static int MASK_COLOR = ddutil::YELLOW;

	static Picture getDevilishMask();
	const static int DMASK_COLOR = ddutil::LIGHTRED;
	
	static Picture getAncientBird();
	const static int ANC_BIRD_COLOR = ddutil::GREEN;

	static Picture getBloodAltar();
	const static int BLOOD_ALTAR_COLOR = ddutil::LIGHTRED;

	static Picture getGoldAltar();
	const static int GOLD_ALTAR_COLOR = ddutil::YELLOW;

	static Picture getSpider();
	const static int SPIDER_COLOR = ddutil::BROWN;

	static Picture getVampireBat();
	const static int VAMP_BAT_COLOR = ddutil::LIGHTRED;

	static Picture getVampireBatAlt();
	const static int VAMP_BAT_ALT_COLOR = ddutil::RED;

	static Picture getCaveBat();
	const static int CAVEBAT_COLOR = ddutil::DARKGRAY;

	static Picture getCaveBatAlt();
	const static int CAVEBATALT_COLOR = ddutil::LIGHTGRAY;

	static Picture getAncientLizard();
	const static int ANC_LIZARD_COLOR = ddutil::LIGHTGREEN;

	static Picture getFireBat();
	const static int FIRE_BAT_COLOR = ddutil::YELLOW;

	static Picture getFireBatVariant();
	const static int FIRE_BAT_VARIANT_COLOR = ddutil::CYAN;

	static Picture getFirePlatypus();
	const static int FIRE_PLATYPUS_COLOR = ddutil::DARKGRAY;

	static Picture getSniffer();
	const static int SNIFFER_COLOR = ddutil::BLUE;

	static Picture getSnifferVariant();
	const static int SNIFFER_VARIANT_COLOR = ddutil::LIGHTCYAN;

	static Picture getCorruptedDisciple();
	const static int CORRUPTED_DISCIPLE_COLOR = ddutil::MAGENTA;

	static Picture getAncientMask();
	const static int ANCIENT_MASK_COLOR = ddutil::DARKGRAY;
	
	static Picture getGiantHead();
	const static int GIANT_HEAD_COLOR = ddutil::LIGHTGRAY;
};



#endif