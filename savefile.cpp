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
	distortion = game->currentDistortion;
}

void Savefile::writeToFile(std::string filename)
{
	std::ofstream file(filename, std::ios::trunc);
	file << score << std::endl;
	file << currentZoneIndex << std::endl;
	file << distortion << std::endl;
	Savechunk gameWorldChunk = gameWorldToText();
	for (std::string s : gameWorldChunk)
	{
		file << s << std::endl;
	}
	Savechunk playerPartyChunk = playerPartyToText();
	for (std::string s : playerPartyChunk)
	{
		file << s << std::endl;
	}
	Savechunk deadPlayersChunk = deadPlayersToText();
	for (std::string s : deadPlayersChunk)
	{
		file << s << std::endl;
	}
	file.close();
	ddutil::xorFile(filename);
}

Savefile::Savefile(Game* game, std::string filename)
{
	ddutil::xorFile(filename);
	std::ifstream file(filename);

	if (file.peek() == std::ifstream::traits_type::eof())
	{
		ddutil::errorMessage("No save file found", __LINE__, __FILE__);
	}

	Savechunk gameWorldChunk;
	
    std::string s;
	std::getline(file, s);
	score = std::stoi(s);
	std::getline(file, s);
	currentZoneIndex = std::stoi(s);
	std::getline(file, s);
	distortion = std::stoi(s);
	while (s != "GAME WORLD END")
	{
		std::getline(file, s);
		gameWorldChunk.add(s);
	}

	textToGameWorld(game, gameWorldChunk);

	Savechunk playerPartyChunk;
	while (s != "PLAYER PARTY END")
	{
		std::getline(file, s);
		playerPartyChunk.add(s);
	}

	textToPlayerParty(game, playerPartyChunk);

	Savechunk deadPlayersChunk;
	while (s != "DEAD PLAYERS END")
	{
		std::getline(file, s);
		deadPlayersChunk.add(s);
	}

	textToDeadPlayers(game, deadPlayersChunk);
	file.close();
	ddutil::xorFile(filename);
}

void Savefile::loadIntoGame(Game* game)
{
	game->score = score;
	game->currentZoneIndex = currentZoneIndex;
	game->currentDistortion = distortion;
	if (game->currentDistortion >= 2)
	{
		game->greenMoveCost *= 1 + (ddutil::DIST2_MOVEXP_PERCENT / 100.0);
		game->blueMoveCost*= 1 + (ddutil::DIST2_MOVEXP_PERCENT / 100.0);
		game->redMoveCost *= 1 + (ddutil::DIST2_MOVEXP_PERCENT / 100.0);
	}
	for (Zone* z : game->gameWorld)
	{
		delete z;
	}
	game->gameWorld.clear();
	game->gameWorld = gameWorld;
	for (Player* p : game->playerParty)
	{
		delete p;
	}
	game->playerParty.clear();
	game->playerParty = playerParty;
	for (Player* p : game->deadPlayers)
	{
		delete p;
	}
	game->deadPlayers.clear();
	game->deadPlayers = deadPlayers;
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
	for (unsigned i = 1; i < data.size(); i++)
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
	Savechunk chunk;
	chunk.add("PLAYER PARTY START");
	for (Player* p : playerParty)
	{
		chunk.add(p->makeSaveChunk());
	}
	chunk.add("PLAYER PARTY END");
	return chunk;
}

void Savefile::textToPlayerParty(Game* game, Savechunk data)
{
	if (data.front() != "PLAYER PARTY START" || data.back() != "PLAYER PARTY END")
	{
		ddutil::errorMessage("Invalid Save File Format", __LINE__, __FILE__);
	}
	for (unsigned i = 1; i < data.size(); i++)
	{
		if (data.at(i) == "PLAYER START")
		{
			Savechunk playerChunk;
			i++;
			PlayerId type = static_cast<PlayerId>(std::stoi(data.at(i++)));
			for (; data.at(i) != "PLAYER END"; i++)
			{
				playerChunk.add(data.at(i));
			}
			Player* player = Player::getPlayerFromSavechunk(game, type, playerChunk);
			playerParty.push_back(player);
		}
		else if (data.at(i) != "PLAYER PARTY END")
		{
			ddutil::errorMessage("Invalid Save File Format", __LINE__, __FILE__);
		}
	}
}

Savechunk Savefile::deadPlayersToText()
{
	Savechunk chunk;
	chunk.add("DEAD PLAYERS START");
	for (Player* p : deadPlayers)
	{
		chunk.add(p->makeSaveChunk());
	}
	chunk.add("DEAD PLAYERS END");
	return chunk;
}

void Savefile::textToDeadPlayers(Game* game, Savechunk data)
{
	if (data.front() != "DEAD PLAYERS START" || data.back() != "DEAD PLAYERS END")
	{
		ddutil::errorMessage("Invalid Save File Format", __LINE__, __FILE__);
	}
	for (unsigned i = 1; i < data.size(); i++)
	{
		if (data.at(i) == "PLAYER START")
		{
			Savechunk playerChunk;
			i++;
			PlayerId type = static_cast<PlayerId>(std::stoi(data.at(i++)));
			for (; data.at(i) != "PLAYER END"; i++)
			{
				playerChunk.add(data.at(i));
			}
			Player* player = Player::getPlayerFromSavechunk(game, type, playerChunk);
			deadPlayers.push_back(player);
		}
		else if (data.at(i) != "DEAD PLAYERS END")
		{
			ddutil::errorMessage("Invalid Save File Format", __LINE__, __FILE__);
		}
	}
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
