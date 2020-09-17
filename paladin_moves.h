#ifndef PALADIN_MOVES_H
#define PALADIN_MOVES_H

#include "move.h"
#include "colorstring.h"

class Creature;

namespace PaladinMoves
{
	class Pound : public Move
	{
	public:
		Pound();
		ColorString doAction(Creature* self, Creature* other);
	};

	class Shield : public Move
	{
	public:
		Shield();
		ColorString doAction(Creature* self, Creature* other);
	};

	class Smite : public Move
	{
	public:
		Smite();
		ColorString doAction(Creature* self, Creature* other);
	};

	class Miracle : public Move
	{
	public:
		Miracle();
		ColorString doAction(Creature* self, Creature* other);
	};
}

#endif