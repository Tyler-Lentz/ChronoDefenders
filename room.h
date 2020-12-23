#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include "colorstring.h"
#include "utilities.h"

class Game;
class Enemy;

class Room
{
public:
	Room(Game* theGame, ColorChar theMapChar);

	virtual void playRoom() = 0;

	ColorChar getMapChar();
	void setCharEmpty(); // // set the map char to a blank character so the room isnt drawn anymore on the map

protected:
	Game* game;
	ColorChar mapChar;
};

class EnemyRoom : public Room
{
public:
	EnemyRoom(Game* game, Enemy* theEnemy);
	~EnemyRoom();
	void playRoom();

private:
	Enemy* enemy;
};

class FireRoom : public Room
{
public:
	FireRoom(Game* game);

	void playRoom();

	const static int FIRE_HEAL = 20;
private:

};

class EventRoom : public Room
{
public:
	EventRoom(Game* game);
private:
};

class DeadAdventurerEvent : public EventRoom
{
public:
	DeadAdventurerEvent(Game* game);

	void playRoom() override;

	static const int EXPERIENCE_GAIN = ddutil::MODERATE_COST * 2;
	static const int HEALTH_GAIN = FireRoom::FIRE_HEAL;
};

class BrokenMirrorEvent : public EventRoom
{
public:
	BrokenMirrorEvent(Game* game);

	void playRoom() override;
};

class MysteriousKnightEvent : public EventRoom
{
public:
	MysteriousKnightEvent(Game* game);

	void playRoom() override;
};

class DynamiteEvent : public EventRoom
{
public:
	DynamiteEvent(Game* game);

	void playRoom() override;

	const static int DAMAGE = 10;
	const static int EXPERIENCE_GAIN = 40;
};

// Enter a room, become surrounded by giant spiders, either can attempt escape
// for a 50% chance to leave, 50% chance everyone takes 5 damage then fight. Or 
// can just choose to fight from the begining
class SpiderEvent : public EventRoom
{
public:
	SpiderEvent(Game* game);

	void playRoom() override;
	const static int INITIAL_DAMAGE = 5;
	const static int ESCAPE_CHANCE = 50;
};

// For a blood price, one character can gain additional max health
class BloodAltarEvent : public EventRoom
{
public:
	BloodAltarEvent(Game* game);

	void playRoom() override;
	const static int NET_HEALTH_LOSS = 20;
	const static int MAX_HEALTH_GAIN = 10;
};

class TreasureEvent : public EventRoom
{
public:
	TreasureEvent(Game* game);

	void playRoom() override;
};

class LavaBeastEvent : public EventRoom
{
public:
	LavaBeastEvent(Game* game);

	void playRoom() override;

	const static int FULL_HEAL = 999;
	const static int MAX_HEALTH_DECREASE = 10;

	const static int VIT_PER_TURN_INCREASE = 1;
	const static int HEALING_PERCENT_DECREASE = 50;

	const static int NUM_ARTIFACTS = 3;
	const static int DAMAGE = 25;
};

class GoblinEvent : public EventRoom
{
public:
	GoblinEvent(Game* game);

	void playRoom() override;

	const static int HEALTH_DECREASE = 7;
};

class MaskEvent : public EventRoom
{
public:
	MaskEvent(Game* game);

	void playRoom() override;
};

class MirrorEvent : public EventRoom
{
public:
	MirrorEvent(Game* game);

	void playRoom() override;
};

class PortalEvent : public EventRoom
{
public:
	PortalEvent(Game* game);

	void playRoom() override;
};

#endif