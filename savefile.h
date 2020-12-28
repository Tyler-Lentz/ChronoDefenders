#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <string>
#include <vector>

class Game;
class Zone;
class Player;


class Savechunk : public std::vector<std::string>
{
public:
	void add(Savechunk other);
	void add(std::string str);
	void add(int i);
};

class Savefile
{
public:
	Savefile(Game* game); // constructor to make a save file from game's state
	void writeToFile(std::string filename); // write this save file to file

	Savefile(Game* game, std::string filename); // constructor to make a save file from a file
	void loadIntoGame(Game* game); // take this save file object and set the game up correctly

	~Savefile();

	static void eraseFile(std::string filename);
private:
	// easy integer values and the like
	int score;
	unsigned int currentZoneIndex;
	int distortion;

	// complicated containers
	std::vector<Zone*> gameWorld;
	Savechunk gameWorldToText(); // helper function which puts this savefile's gameWorld object into text format
	void textToGameWorld(Game* game, Savechunk data);// puts text format into this objects gameWorld object

	std::vector<Player*> playerParty;
	Savechunk playerPartyToText(); // helper function which puts this savefile's playerParty into text format
	void textToPlayerParty(Game* game, Savechunk data); // puts text format into this object's playerParty object

	std::vector<Player*> deadPlayers;
	Savechunk deadPlayersToText(); // puts this savefile's playerParty into text format
	void textToDeadPlayers(Game* game, Savechunk data); // puts text format into this object's playerParty object
};

#endif
