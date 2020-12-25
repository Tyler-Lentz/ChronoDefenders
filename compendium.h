#ifndef COMPENDIUM_H
#define COMPENDIUM_H

#include "game.h"
#include "move.h"
#include "gunslinger_moves.h"
#include "sorcerer_moves.h"
#include "samurai_moves.h"
#include "art.h"
#include "creature.h"
#include "virtualwindow.h"

#include <vector>

class Compendium
{
public:
	Compendium(VirtualWindow* vwin);

	void display(); // puts up screen where the user can see all of the information in the compendium and scroll through screens
private:
	std::vector<Picture> compendium;
	VirtualWindow* vwin;
	std::string borderString;
	int lastPage; // stores the last page the player was on so the compendium returns to the page you left off on

	// Private helper functions
	void makeSamuraiList(); // add list of samurai moves
	void makeGunslingerList(); // add list of gunslinger moves
	void makeSorceressList(); // add list of sorceress moves
	void makeStatusList(); // add list of status's and their effects
};
#endif