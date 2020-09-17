#include "menu.h"
#include "virtualwindow.h"
#include "colorstring.h"
#include "coordinate.h"
#include "soundfile.h"

#include <Windows.h>
#include <vector>

Menu::Menu(VirtualWindow* theWin, std::vector<ColorString> theOptions, Coordinate theTopLeft, bool centered)
	:Menu(theWin, theOptions, std::vector<int> {}, theTopLeft, centered)
{
	// passes through an empy other options vector because there arent any	
}

Menu::Menu(VirtualWindow* theWin, std::vector<ColorString> theOptions, std::vector<int> otherInput, Coordinate theTopLeft, bool centered)
{
	vwin = theWin;
	int x = theTopLeft.x;
	int y = theTopLeft.y; // values that will be incremented as it is drawn initially

	// find the longest option, and set the x based of centering that
	if (centered)
	{
		unsigned int longestLength = 0;
		for (auto i : theOptions)
		{
			if (i.size() > longestLength)
			{
				longestLength = i.size();
			}
		}

		//      a          l           a     
		//  __________LongestOption__________
		// a + l + a = X
		// a = (X - l) / 2
		// but a is just the x coordinate so
		x = (ddutil::CONSOLEX - longestLength) / 2;
	}

	for (auto i : theOptions)
	{
		vwin->put(i, Coordinate(x, y++));
	}

	// contains the x coordinate for where to put the [] for each line
	// each element in this vector corresponds to an element in the options vector

	// EXAMPLE:
	// the Options vector contains "Ice Orb"
	// we want to print "[Ice Orb]" when this option is selected
	// we store the x coordinates of both of the brackets using this piece of code in the vector of pairs
	std::vector<std::pair<int, int>> bracketSlots;
	for (auto i : theOptions)
	{
		int selectionLength = i.size();
		bracketSlots.push_back(std::make_pair(x - 1, x + selectionLength));
	}

	char openChar = '[';
	char closeChar = ']';

	// draw the [] around the first option (which starts selected)
	drawIndicators(bracketSlots[0], theTopLeft.y);

	// start with the first one selected
	int currentSelection = 0;

	// how much time to buffer inputs
	const int SLEEP_TIME = 150;

	// input loop, waiting for enter press
	while (true)
	{
		if (ddutil::keypress(VK_UP) && currentSelection != 0)
		{
			playSound(WavFile("menumove", false, true));
			clearIndicators(bracketSlots[currentSelection], theTopLeft.y + currentSelection);
			currentSelection--;
			drawIndicators(bracketSlots[currentSelection], theTopLeft.y + currentSelection);
			Sleep(SLEEP_TIME);
		}
		if (ddutil::keypress(VK_DOWN) && currentSelection != theOptions.size() - 1)
		{
			playSound(WavFile("menumove", false, true));
			clearIndicators(bracketSlots[currentSelection], theTopLeft.y + currentSelection);
			currentSelection++;
			drawIndicators(bracketSlots[currentSelection], theTopLeft.y + currentSelection);
			Sleep(SLEEP_TIME);
		}
		if (ddutil::keypress(VK_RETURN))
		{
			playSound(WavFile("menuselect", false, true));
			response = currentSelection;
			Sleep(SLEEP_TIME);
			return;
		}
		for (int i : otherInput)
		{
			if (ddutil::keypress(i))
			{
				playSound(WavFile("menumove2", false, true));
				response = i;
				Sleep(SLEEP_TIME);
				return;
			}
		}
	}
}

int Menu::getResponse()
{
	return response;
}

void Menu::oneOptionMenu(VirtualWindow* theWin, ColorString text, Coordinate topLeft, bool centered)
{
	std::vector<ColorString> options;
	options.push_back(text);
	Menu(theWin, options, topLeft, centered);
}

void Menu::drawIndicators(std::pair<int, int> xCoords, int y)
{
	vwin->put(ColorChar('[', ddutil::MENU_SELECTION_COLOR), Coordinate(xCoords.first, y));
	vwin->put(ColorChar(']', ddutil::MENU_SELECTION_COLOR), Coordinate(xCoords.second, y));
}

void Menu::clearIndicators(std::pair<int, int> xCoords, int y)
{
	vwin->put(ColorChar(' ', ddutil::MENU_SELECTION_COLOR), Coordinate(xCoords.first, y));
	vwin->put(ColorChar(' ', ddutil::MENU_SELECTION_COLOR), Coordinate(xCoords.second, y));
}
