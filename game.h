#ifndef GAME_H
#define GAME_H

#include "coordinate.h"
#include "utilities.h"
#include "art.h"
#include "savefile.h"
#include "profile.h"

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
	std::vector<Player*> getDeadPlayers();
	Player* revivePlayer(int index); // pass through the index of the dead player in the dead player list
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

	int getGreenMoveCost();
	int getBlueMoveCost();
	int getRedMoveCost();
	int getCurrentDistortion();

	// drawing to screen
	void displayDividerString();
private:

	bool loadedFromFile;
	Compendium* compendium;

	int greenMoveCost;
	int blueMoveCost;
	int redMoveCost;
	int score;
	Profile profile;
	int currentDistortion; // the actual distortion the current run is on
	void setDistortionModifers(); // once the player has selected their modifier, then change the values appropriately
	// Distortion is the unlockable difficulty meter for the game
	// distortion 0 is the default
	// each time you win, you unlock a new distortion
	// distortion 1: Start with 75% of max HP
	// distortion 2: Moves cost 33% more experience
	// distortion 3: -10 Max Health
	// distortion 4: Receive no starting bonus
	// distortion 5: True Final Boss

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