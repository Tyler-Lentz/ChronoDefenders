#include "virtualwindow.h"
#include "utilities.h"
#include "coordinate.h"
#include "game.h"
#include "soundfile.h"
#include "menu.h"

#include <thread>
#include <Windows.h>

int main()
{
	VirtualWindow* vwin = new VirtualWindow(ddutil::CONSOLEX, ddutil::CONSOLEY);
	vwin->getConsole().setTitle();

	bool exit = false;
	std::thread framerate([&vwin, &exit]()
		{
			while (!exit)
			{
				vwin->refresh();
				Sleep(50);
			}
		});

	Game* game = nullptr;
	while (!exit)
	{
		game = nullptr;
		try
		{
			game = new Game(vwin);
			while (!exit)
			{
				switch (game->run())
				{
				case ddutil::GameStatus::EXIT:
					exit = true;
					break;
				case ddutil::GameStatus::RESTART:
					delete game;
					game = new Game(vwin);
					break;
				}
			}
		}
		catch (std::exception& ex)
		{
			stopSound(SoundType::MP3);
			playSound(WavFile("error", false, true));
			playSound(Mp3File("gameover"));
			for (int i = 0; i < ddutil::CONSOLEY; i++)
			{
				vwin->clearLine(i, ddutil::getColor(ddutil::RED, ddutil::RED));
				Sleep(1);
			}
			for (int i = 0; i < ddutil::CONSOLEY; i++)
			{
				vwin->clearLine(i, ddutil::BLACK);
				Sleep(1);
			}
			vwin->printArt(Art::getBug(), Coordinate(0, 5), true);
			vwin->putcen(ColorString("Fatal Error Encountered :(", ddutil::RED), 18);	
			vwin->putcen(ColorString("Please report this bug", ddutil::RED), 19);
			ColorString error = ColorString(ex.what(), ddutil::YELLOW);
			ddutil::trimLength(error);
			vwin->putcen(error, 21);
			std::vector<ColorString> options = {
				ColorString("Reload", ddutil::GREEN),
				ColorString("Exit", ddutil::RED)
			};

			Menu menu(vwin, options, Coordinate(0, 25), true);
			if (menu.getResponse() == 1)
			{
				exit = true;
			}
			stopSound(SoundType::MP3);
		}
	}

	framerate.join();

	delete vwin;
	delete game;
	return 0;
}