#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include "colorstring.h"

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

	static const int EXPERIENCE_GAIN = 20;
	static const int HEALTH_GAIN = 8;
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