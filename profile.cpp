#include "profile.h"

#include <fstream>
#include <vector>
#include <string>

Profile::Profile()
	:FILENAME("profile.txt")
{
	ddutil::xorFile(FILENAME);
	std::ifstream file(FILENAME);
	corrupted = false;
	noFileFound = false;

	if (file.peek() == std::ifstream::traits_type::eof())
	{
		// The profile is empty, so put default values
		noFileFound = true;
		highestDistortion = 0;
		highestScore = 0;
		numWins = 0;
		numLosses = 0;
		updateFile();
	}
	else
	{
		// should be data, so fill em up
		try
		{
			std::string s;
			std::getline(file, s);
			highestDistortion = std::stoi(s);
			std::getline(file, s);
			highestScore = std::stoi(s);
			std::getline(file, s);
			numWins = std::stoi(s);
			std::getline(file, s);
			numLosses = std::stoi(s);
			ddutil::xorFile(FILENAME); // don't want to xor if we call updateFile(), so its in this else block not outside
		}
		catch (std::exception& ex)
		{
			// error loading, reset file to default and report that file was corrupted
			corrupted = true;
			highestDistortion = 0;
			highestScore = 0;
			numWins = 0;
			numLosses = 0;
			updateFile();
		}
	}	
	
	file.close();

}

void Profile::incrementNumWins()
{
	numWins++;
	updateFile();
}

void Profile::incrementNumLosses()
{
	numLosses++;
	updateFile();
}

void Profile::incrementHighestDistortion()
{
	highestDistortion++;
	updateFile();
}

void Profile::setNewHighScore(int amount)
{
	highestScore = amount;
	updateFile();
}

int Profile::getHighestDistortion()
{
	return highestDistortion;
}

int Profile::getHighestScore()
{
	return highestScore;
}

int Profile::getNumWins()
{
	return numWins;
}

int Profile::getNumLosses()
{
	return numLosses;
}

bool Profile::wasNoFileFound()
{
	return noFileFound;
}

bool Profile::isCorrupted()
{
	return corrupted;
}

ColorString Profile::getColorString()
{
	ColorString c1("Highest Distortion: " + std::to_string(highestDistortion) + "  ", ddutil::DISTORTION_COLOR);
	ColorString c2("Highest Score: " + std::to_string(highestScore) + "  ", ddutil::SCORE_COLOR);
	ColorString c3("Wins: " + std::to_string(numWins) + "  ", ddutil::WIN_COLOR);
	ColorString c4("Losses: " + std::to_string(numLosses) + "  ", ddutil::LOSS_COLOR);
	return c1 + c2 + c3 + c4;
}

void Profile::updateFile()
{
	std::ofstream file(FILENAME, std::ios::trunc);
	file << highestDistortion << std::endl;
	file << highestScore << std::endl;
	file << numWins << std::endl;
	file << numLosses << std::endl;
	file.close();
	ddutil::xorFile(FILENAME);
}
