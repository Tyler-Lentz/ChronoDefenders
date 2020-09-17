#include "virtualwindow.h"
#include "utilities.h"
#include "coordinate.h"
#include "game.h"
#include "soundfile.h"

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

	Game* game = new Game(vwin);

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


	framerate.join();

	delete vwin;
	delete game;
	
	return 0;
}