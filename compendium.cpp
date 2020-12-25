#include "compendium.h"
#include "utilities.h"
#include "soundfile.h"

#include <memory>

Compendium::Compendium(VirtualWindow* vwin)
{
	this->vwin = vwin;
	lastPage = 0; // start on the first page
	borderString = std::string(ddutil::CONSOLEX, '=');
	makeSamuraiList();
	makeGunslingerList();
	makeSorceressList();
	makeStatusList();

}

void Compendium::display()
{
	bool exit = false;
	while (!exit)
	{
		vwin->clearScreen();
		vwin->printArt(compendium.at(lastPage), Coordinate(0, 0), false);
		while (true) // input loop
		{
			if (ddutil::keypress(VK_RIGHT))
			{
				lastPage++;
				playSound(WavFile("menumove2", false, true));
				Sleep(ddutil::BUFFER_TIME);
				break;
			}
			if (ddutil::keypress(VK_LEFT))
			{
				lastPage--;
				playSound(WavFile("menumove2", false, true));
				Sleep(ddutil::BUFFER_TIME);
				break;
			}
			if (ddutil::keypress(VK_ESCAPE))
			{
				exit = true;
				playSound(WavFile("menumove2", false, true));
				Sleep(ddutil::BUFFER_TIME);
				break;
			}
		}
		if (lastPage < 0)
		{
			lastPage = compendium.size() - 1;
		}
		else if (lastPage >= static_cast<int>(compendium.size()))
		{
			lastPage = 0;
		}
	}
}

void Compendium::makeSamuraiList()
{
	Picture page;
	int color = ddutil::SAMURAI_COLOR;
	int size = ddutil::CONSOLEX;
	page.push_back(ColorString(borderString, color));
	page.push_back(ColorString(ddutil::padString("<- Samurai Moves ->", size), color));	
	page.push_back(ColorString(borderString, color));
		
	std::unique_ptr<Move> move;
	for (int i = 0; i <= 35; i++)
	{
		switch (i)
		{
		case 0:
			move = std::make_unique<SamuraiMoves::Slice>();
			break;
		case 1:
			move = std::make_unique<SamuraiMoves::Deflect>();
			break;
		case 2:
			move = std::make_unique<SamuraiMoves::ThrowingKnives>();
			break;
		case 3:
			move = std::make_unique<SamuraiMoves::PerilousStrike>();
			break;
		case 4:
			move = std::make_unique<SamuraiMoves::CalculatedSlice>();
			break;
		case 5:
			move = std::make_unique<SamuraiMoves::Sidestep>();
			break;
		case 6:
			move = std::make_unique<SamuraiMoves::Meditation>();
			break;
		case 7:
			move = std::make_unique<SamuraiMoves::Storm>();
			break;
		case 8:
			move = std::make_unique<SamuraiMoves::LightningRod>();
			break;
		case 9:
			move = std::make_unique<SamuraiMoves::Assault>();
			break;
		case 10:
			move = std::make_unique<SamuraiMoves::DualSlice>();
			break;
		case 11:
			move = std::make_unique<SamuraiMoves::BodySlam>();
			break;
		case 12:
			move = std::make_unique<SamuraiMoves::Shuriken>();
			break;
		case 13:
			move = std::make_unique<SamuraiMoves::Typhoon>();
			break;
		case 14:
			move = std::make_unique<SamuraiMoves::DragonSlice>();
			break;
		case 15:
			move = std::make_unique<SamuraiMoves::PerilousPlunge>();
			break;
		case 16:
			move = std::make_unique<SamuraiMoves::Zen>();
			break;
		case 17:
			move = std::make_unique<SamuraiMoves::ChargedAssault>();
			break;
		case 18:
			move = std::make_unique<SamuraiMoves::Desperation>();
			break;
		case 19:
			move = std::make_unique<SamuraiMoves::CrushingBlow>();
			break;
		case 20:
			move = std::make_unique<SamuraiMoves::CriticalStrike>();
			break;
		case 21:
			move = std::make_unique<SamuraiMoves::FinishingBlow>();
			break;
		case 22:
			move = std::make_unique<SamuraiMoves::EyeOfTheStorm>();
			break;
		case 23:
			move = std::make_unique<SamuraiMoves::ShadowStep>();
			break;
		case 24:
			move = std::make_unique<SamuraiMoves::DragonForm>();
			break;
		case 25:
			move = std::make_unique<SamuraiMoves::Caltrops>();
			break;
		case 26:
			move = std::make_unique<SamuraiMoves::TippedKunai>();
			break;
		case 27:
			move = std::make_unique<SamuraiMoves::BlindingFury>();
			break;
		case 28:
			move = std::make_unique<SamuraiMoves::Tsunami>();
			break;
		case 29:
			move = std::make_unique<SamuraiMoves::LeavesFromTheVine>();
			break;
		case 30:
			move = std::make_unique<SamuraiMoves::Enlightenment>();
			break;
		case 31:
			move = std::make_unique<SamuraiMoves::Eviscerate>();
			break;
		case 32:
			move = std::make_unique<SamuraiMoves::DualingDragons>();
			break;
		case 33:
			move = std::make_unique<SamuraiMoves::DragonBreath>();
			break;
		case 34:
			move = std::make_unique<SamuraiMoves::Tornado>();
			break;
		case 35:
			move = std::make_unique<SamuraiMoves::ShinobiTactics>();
			break;
		}
		page.push_back(move->getFullInformation());
	}

	compendium.push_back(page);
}

void Compendium::makeGunslingerList()
{
	Picture page;
	int color = ddutil::GUNSLINGER_COLOR;
	int size = ddutil::CONSOLEX;
	page.push_back(ColorString(borderString, color));
	page.push_back(ColorString(ddutil::padString("<- Gunslinger Moves ->", size), color));	
	page.push_back(ColorString(borderString, color));
		
	std::unique_ptr<Move> move;
	for (int i = 0; i <= 36; i++)
	{
		switch (i)
		{
		case 0:
			move = std::make_unique<GunslingerMoves::Peashooter>();
			break;
		case 1:
			move = std::make_unique<GunslingerMoves::Dodge>();
			break;
		case 2:
			move = std::make_unique<GunslingerMoves::MakeBullets>();
			break;
		case 3:
			move = std::make_unique<GunslingerMoves::Dropkick>();
			break;
		case 4:
			move = std::make_unique<GunslingerMoves::BottleStrike>();
			break;
		case 5:
			move = std::make_unique<GunslingerMoves::Backflip>();
			break;
		case 6:
			move = std::make_unique<GunslingerMoves::Punch>();
			break;
		case 7:
			move = std::make_unique<GunslingerMoves::DrawSpade>();
			break;
		case 8:
			move = std::make_unique<GunslingerMoves::DrawHeart>();
			break;
		case 9:
			move = std::make_unique<GunslingerMoves::DrawDiamond>();
			break;
		case 10:
			move = std::make_unique<GunslingerMoves::DrawClub>();
			break;
		case 11:
			move = std::make_unique<GunslingerMoves::CardThrow>();
			break;
		case 12:
			move = std::make_unique<GunslingerMoves::Revolver>();
			break;
		case 13:
			move = std::make_unique<GunslingerMoves::ThrowDynamite>();
			break;
		case 14:
			move = std::make_unique<GunslingerMoves::CraftBullets>();
			break;
		case 15:
			move = std::make_unique<GunslingerMoves::DoubleKick>();
			break;
		case 16:
			move = std::make_unique<GunslingerMoves::HeavyRevolver>();
			break;
		case 17:
			move = std::make_unique<GunslingerMoves::ExplosiveShot>();
			break;
		case 18:
			move = std::make_unique<GunslingerMoves::Leap>();
			break;
		case 19:
			move = std::make_unique<GunslingerMoves::Uppercut>();
			break;
		case 20:
			move = std::make_unique<GunslingerMoves::PumpShotgun>();
			break;
		case 21:
			move = std::make_unique<GunslingerMoves::ForgeBullets>();
			break;
		case 22:
			move = std::make_unique<GunslingerMoves::QuickDynamite>();
			break;
		case 23:
			move = std::make_unique<GunslingerMoves::DrinkWhiskey>();
			break;
		case 24:
			move = std::make_unique<GunslingerMoves::CardFlurry>();
			break;
		case 25:
			move = std::make_unique<GunslingerMoves::SupressiveFire>();
			break;
		case 26:
			move = std::make_unique<GunslingerMoves::SuckerPunch>();
			break;
		case 27:
			move = std::make_unique<GunslingerMoves::ConjureBullets>();
			break;
		case 28:
			move = std::make_unique<GunslingerMoves::SharpsRifle>();
			break;
		case 29:
			move = std::make_unique<GunslingerMoves::ColtWalker>();
			break;
		case 30:
			move = std::make_unique<GunslingerMoves::FaceOfSpades>();
			break;
		case 31:
			move = std::make_unique<GunslingerMoves::FaceOfHearts>();
			break;
		case 32:
			move = std::make_unique<GunslingerMoves::FaceOfClubs>();
			break;
		case 33:
			move = std::make_unique<GunslingerMoves::FaceOfDiamonds>();
			break;
		case 34:
			move = std::make_unique<GunslingerMoves::BlackDynamite>();
			break;
		case 35:
			move = std::make_unique<GunslingerMoves::BrassKnuckles>();
			break;
		case 36:
			move = std::make_unique<GunslingerMoves::JesterForm>();
			break;
		}
		page.push_back(move->getFullInformation());
	}

	compendium.push_back(page);
}

void Compendium::makeSorceressList()
{
	Picture page;
	int color = ddutil::SORCERER_COLOR;
	int size = ddutil::CONSOLEX;
	page.push_back(ColorString(borderString, color));
	page.push_back(ColorString(ddutil::padString("<- Sorceress Moves ->", size), color));	
	page.push_back(ColorString(borderString, color));
		
	std::unique_ptr<Move> move;
	for (int i = 0; i <= 35; i++)
	{
		switch (i)
		{
		case 0:
			move = std::make_unique<SorcererMoves::EnergyStrike>();
			break;
		case 1:
			move = std::make_unique<SorcererMoves::Heal>();
			break;
		case 2:
			move = std::make_unique<SorcererMoves::MagicBarrier>();
			break;
		case 3:
			move = std::make_unique<SorcererMoves::ElectricShock>();
			break;
		case 4:
			move = std::make_unique<SorcererMoves::Levitate>();
			break;
		case 5:
			move = std::make_unique<SorcererMoves::BlinkStrike>();
			break;
		case 6:
			move = std::make_unique<SorcererMoves::Fireball>();
			break;
		case 7:
			move = std::make_unique<SorcererMoves::IceOrb>();
			break;
		case 8:
			move = std::make_unique<SorcererMoves::Recover>();
			break;
		case 9:
			move = std::make_unique<SorcererMoves::SummonFireImp>(nullptr);
			break;
		case 10:
			move = std::make_unique<SorcererMoves::SummonIceImp>(nullptr);
			break;
		case 11:
			move = std::make_unique<SorcererMoves::EnergyBarrier>();
			break;
		case 12:
			move = std::make_unique<SorcererMoves::CleansingTouch>();
			break;
		case 13:
			move = std::make_unique<SorcererMoves::EnergySword>();
			break;
		case 14:
			move = std::make_unique<SorcererMoves::PlasmaStrike>();
			break;
		case 15:
			move = std::make_unique<SorcererMoves::MirrorImage>();
			break;
		case 16:
			move = std::make_unique<SorcererMoves::FairySummon>(nullptr);
			break;
		case 17:
			move = std::make_unique<SorcererMoves::SummonFireDragon>(nullptr);
			break;
		case 18:
			move = std::make_unique<SorcererMoves::SummonIceDragon>(nullptr);
			break;
		case 19:
			move = std::make_unique<SorcererMoves::Blizzard>();
			break;
		case 20:
			move = std::make_unique<SorcererMoves::Cataclysm>();
			break;
		case 21:
			move = std::make_unique<SorcererMoves::LightningStrike>();
			break;
		case 22:
			move = std::make_unique<SorcererMoves::PlasmaSword>();
			break;
		case 23:
			move = std::make_unique<SorcererMoves::EtherealBarrier>();
			break;
		case 24:
			move = std::make_unique<SorcererMoves::StarStrike>();
			break;
		case 25:
			move = std::make_unique<SorcererMoves::Combust>();
			break;
		case 26:
			move = std::make_unique<SorcererMoves::Freeze>();
			break;
		case 27:
			move = std::make_unique<SorcererMoves::ChainLightning>();
			break;
		case 28:
			move = std::make_unique<SorcererMoves::CleansingAura>();
			break;
		case 29:
			move = std::make_unique<SorcererMoves::EnergyAura>();
			break;
		case 30:
			move = std::make_unique<SorcererMoves::EtherealBlade>();
			break;
		case 31:
			move = std::make_unique<SorcererMoves::SpiritCall>(nullptr);
			break;
		case 32:
			move = std::make_unique<SorcererMoves::Supernova>();
			break;
		case 33:
			move = std::make_unique<SorcererMoves::CastInvulnerability>();
			break;
		case 34:
			move = std::make_unique<SorcererMoves::ElementalForm>();
			break;
		case 35:
			move = std::make_unique<SorcererMoves::ElementalBarrier>();
			break;
		}
		page.push_back(move->getFullInformation());
	}

	compendium.push_back(page);
}

void Compendium::makeStatusList()
{
	Picture page;
	int color = ddutil::TEXT_COLOR;
	int size = ddutil::CONSOLEX;
	page.push_back(ColorString(borderString, color));
	page.push_back(ColorString(ddutil::padString("<- Status Effect Descriptions ->", size), color));	
	page.push_back(ColorString(borderString, color));
	page.push_back(ColorString());
	page.push_back(ColorString("Be wary of these status effects in your adventures!", ddutil::TEXT_COLOR));
	page.push_back(ColorString());
	std::unique_ptr<Status> status = std::make_unique<BurntStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<ScorchedStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<FrostBurntStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<BleedingStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<PoisonedStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<ZappedStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<StormStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<ZenStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<InvulnerableStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<VulnerableStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<HexedStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<StrangledStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<StunnedStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<SpadeStatus>(-1);
	page.push_back(status->getFullInformation());
	status = std::make_unique<HeartStatus>(-1);
	page.push_back(status->getFullInformation());
	status = std::make_unique<ClubStatus>(-1);
	page.push_back(status->getFullInformation());
	status = std::make_unique<DiamondStatus>(-1);
	page.push_back(status->getFullInformation());
	status = std::make_unique<Dynamite>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<ThornsStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<DragonStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<JesterStatus>();
	page.push_back(status->getFullInformation());
	status = std::make_unique<ElementalStatus>();
	page.push_back(status->getFullInformation());

	compendium.push_back(page);
}
