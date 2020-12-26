#ifndef GAME_H
#define GAME_H

#include "coordinate.h"
#include "utilities.h"
#include "art.h"
#include "savefile.h"

#include <vector>


class Player;
class VirtualWindow;
class Enemy;
class Zone;
class Creature;
class Artifact;
class ZoneEnvironment;
class Compendium;

class Game
{
public:
	Game(VirtualWindow* virWin);
	~Game();

	ddutil::GameStatus run(); // main game loop

	// Getting information
	static ColorString getPartyNames(std::vector<Creature*> party);
	static ColorString getPartyNames(std::vector<Player*> party);

	// Major Functions
	void battle(Enemy* enemy);

	// add a player to the player party
	void addPlayer(Player* newPlayer);

	std::vector<Player*> getPlayerParty();
	Enemy* getCurrentEnemy();

	VirtualWindow* getVWin();
	void clearBottomDivider();
	void clearCenterScreen();
	void displayInfo();

	bool isGameOver();

	void playersGetExperience(int amount);

	Player* selectPlayer(int startingLine);

	void artifactSelectionMenu(int startingLine, Artifact* theArtifact);

	Zone* getActiveZone();

	// heals everybody and returns a color string showing the results
	ColorString healAllPlayers(int amount);

	bool checkForPlayerDeaths(); // returns true if all the players are dead and prints info to screen
	bool checkForEnemyDeath(Enemy* enemy); // returns true if the enemy is dead and prints info to screen

	void setGameWin();
	bool getGameWin();
	void setSkipToNextChapter();
	bool shouldSkipChapter();

	void endingScene();
	void changeScore(int amount);
	int getScore();

	void viewCompendium();
private:
	bool loadedFromFile;
	Compendium* compendium;

	int score;

	// Major Functions
	void titleScreen(); // display the titlescreen
	void intro();

	//flags
	bool skipToNextChapter;
	bool gameOver;
	bool gameWin;
	ddutil::GameStatus status;

	// Displaying information to the screen
	void drawPlayerParty();
	
	void roomEntranceAnimation();
	void displayZoneEntrance();
	void displayHelpScreen();
	void printCreatureBattleInfo(Creature* creature, Coordinate bottomLeft, int textLine);
	void doStatusDecrementionAndPrintInfo(Creature* creature);
	void displayDividerString();

	
	void removeMinionsFromParty();

	std::vector<Zone*> gameWorld;
	unsigned int currentZoneIndex;

	VirtualWindow* vwin;

	std::vector<Player*> deadPlayers;
    std::vector<Player*> playerParty;
	Enemy* currentEnemy;


	friend class Savefile;
};

#endif GAME_H