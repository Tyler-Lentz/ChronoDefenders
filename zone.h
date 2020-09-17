#ifndef ZONE_H
#define ZONE_H

#include "colorstring.h"
#include "utilities.h"
#include "move.h"

#include <vector>

class Artifact;
class ModerateArtifact;
class PowerfulArtifact;
class MythicalArtifact;
class BossArtifact;
class Game;
class Enemy;
class Room;
class EventRoom;

class ZoneMap : public std::vector<std::vector<Room*>>
{
public:
	ZoneMap() {};
	ZoneMap(int numPaths, int numRows, int numCols);

	int getNumRows();
	int getNumCols();
	std::vector<Coordinate> getRoomCoords();
private:
	std::vector<Coordinate> roomCoords;
};

class ZoneEnvironment
{
public:
	ZoneEnvironment(Game* theGame, ColorString zoneString, int modChance, int powChance, int mythChance);

	ColorString getZoneString();

	virtual ZoneMap generateRooms() = 0; // makes a list of all the rooms for the zone
	Artifact* getRandomArtifact(); // get a random artifact specific to the floor
	Artifact* getRandomArtifact(Strength str);
	void chooseBossArtifact();

private:
	virtual Enemy* generateEnemy(ddutil::EnemyType type) = 0;// generates an enemy based on this zones type. the type of enemy depends on the parameter
	virtual Room* generateEventRoom() = 0; // makes a random event room for the zone

	ModerateArtifact* getModArtifact();
	PowerfulArtifact* getPowArtifact();
	MythicalArtifact* getMythArtifact();

	ColorString zoneString;

	int moderateChance;
	int powerfulChance;
	int mythicalChance;


protected:
	Game* game;
};

class CatacombsEnvironment : public ZoneEnvironment
{
public:
	CatacombsEnvironment(Game* theGame);
	ZoneMap generateRooms() override;
private:
	Enemy* generateEnemy(ddutil::EnemyType type);
	Room* generateEventRoom() override;
};

class AbyssEnvironment : public ZoneEnvironment
{
public:
	AbyssEnvironment(Game* theGame);
	ZoneMap generateRooms() override;
private:
	Enemy* generateEnemy(ddutil::EnemyType type);
	Room* generateEventRoom() override;
};

class VoidEnvironment : public ZoneEnvironment
{
public:
	VoidEnvironment(Game* theGame);
	ZoneMap generateRooms() override;
private:
	Enemy* generateEnemy(ddutil::EnemyType type);
	Room* generateEventRoom() override;
};

class PalaceEnvironment : public ZoneEnvironment
{
public:
	PalaceEnvironment(Game* theGame);
	ZoneMap generateRooms() override;
private:
	Enemy* generateEnemy(ddutil::EnemyType type);
	Room* generateEventRoom() override;
};




class Zone
{
public:
	Zone(Game* theGame, int num);
	~Zone();

	int getZoneNumber();
	
	ColorString getZoneString();

	ZoneMap getMap();

	const static int LAST_ZONE = 4;

	Artifact* getRandomArtifact() const;
	Artifact* getRandomArtifact(Strength str) const;
	void chooseBossArtifact() const;

	bool hasMoreRooms();
	Room* chooseRoom();
private:
	int currentCol;
	int currentRow;

	ZoneMap map; // contains all the rooms

	ZoneEnvironment* environment; // information about the zone (enemies to spawn, rooms to generate, name + color)

	int zoneNumber;

	Game* game;
};



#endif