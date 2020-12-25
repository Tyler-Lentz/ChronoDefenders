#include "savefile.h"
#include "game.h"
#include "player.h"
#include "creature.h"
#include "zone.h"

#include <fstream>

Savefile::Savefile(Game* game)
{
	score = game->score;
	currentZoneIndex = game->currentZoneIndex;
	gameWorld = game->gameWorld;
	playerParty = game->playerParty;
	deadPlayers = game->deadPlayers;
}

void Savefile::writeToFile(std::string filename)
{
	std::ofstream file(filename, std::ios::trunc);
	Savechunk gameWorldChunk = gameWorldToText();
	for (std::string s : gameWorldChunk)
	{
		file << s << std::endl;
	}
	file.close();
}

Savefile::Savefile(Game* game, std::string filename)
{
	score = 0;
	currentZoneIndex = 0;//TODO FIX

	std::ifstream file(filename);

	if (file.peek() == std::ifstream::traits_type::eof())
	{
		ddutil::errorMessage("No save file found", __LINE__, __FILE__);
	}

	Savechunk gameWorldChunk;
	
    std::string s;
	while (s != "GAME WORLD END")
	{
		std::getline(file, s);
		gameWorldChunk.add(s);
	}

	textToGameWorld(game, gameWorldChunk);
}

void Savefile::loadFromFile(Game* game)
{
	for (Zone* z : game->gameWorld)
	{
		delete z;
	}
	game->gameWorld.clear();
	game->gameWorld = gameWorld;
}

Savefile::~Savefile()
{
	// TODO: figure out if need to delete objects here, because there might be situation where save file is loaded but never 
	// put into game
}

Savechunk Savefile::gameWorldToText()
{
	Savechunk chunk;
	chunk.add("GAME WORLD START");
	for (Zone* z : gameWorld)
	{
		chunk.add(z->makeSaveChunk());
	}
	chunk.add("GAME WORLD END");
	return chunk;
}

void Savefile::textToGameWorld(Game* game, Savechunk data)
{
	if (data.front() != "GAME WORLD START" || data.back() != "GAME WORLD END")
	{
		ddutil::errorMessage("Invalid Save File Format", __LINE__, __FILE__);
	}
	for (int i = 1; i < data.size(); i++)
	{
		if (data.at(i) == "ZONE START")
		{
			Savechunk zoneChunk;
			i++;
			for (; data.at(i) != "ZONE END"; i++)
			{
				zoneChunk.add(data.at(i));
			}
			Zone* zone = new Zone(game, zoneChunk);
			gameWorld.push_back(zone);
		}
		else if (data.at(i) != "GAME WORLD END")
		{
			ddutil::errorMessage("Invalid Save File Format", __LINE__, __FILE__);
		}
	}
}

Savechunk Savefile::playerPartyToText()
{
	return Savechunk();
}

void Savefile::textToPlayerParty(Savechunk data)
{
}

Savechunk Savefile::deadPlayersToText()
{
	return Savechunk();
}

void Savefile::textToDeadPlayers(Savechunk data)
{
}

void Savechunk::add(Savechunk other)
{
	for (std::string s : other)
	{
		this->push_back(s);
	}
}

void Savechunk::add(std::string str)
{
	this->push_back(str);
}

void Savechunk::add(int i)
{
	this->push_back(std::to_string(i));
}
