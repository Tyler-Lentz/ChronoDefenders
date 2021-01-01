#ifndef MENU_H
#define MENU_H

#include "colorstring.h"
#include "coordinate.h"

#include <vector>

class VirtualWindow;

// Creates a menu and draws it to the screen, waiting for an input
class Menu
{
public:
	Menu(VirtualWindow* theWin, std::vector<ColorString> theOptions, Coordinate theTopLeft, bool centered, int startPos = 0);
	// the constructor draws the menu to screen and waits for valid input through enter, which is then stored in response
	// and can be retrieved from getResponse();
	Menu(VirtualWindow* theWin, std::vector<ColorString> theOptions, std::vector<int> otherInput, Coordinate theTopLeft, bool centered, int startPos = 0);
	// other input is a vector which should contain values like VK_LEFT or VK_ESCAPE which are other keys that can trigger a response from the menu

	int getResponse(); // returns integer coresponding to the index of the option from the option vector, or the VK code from the other input
	int getOtherInputResponse(); // always returns the position in the menu the cursor was at

	static void oneOptionMenu(VirtualWindow*, ColorString text, Coordinate topLeft, bool centered);
private:
	// helper functions
	void drawIndicators(std::pair<int, int> xCoords, int y);
	void clearIndicators(std::pair<int, int> xCoords, int y);

	VirtualWindow* vwin;

	int response;

	int otherInputResponse; // if a keypress from the other inputs is selected, the pos of the menu when the button was selected is stored here
};

#endif