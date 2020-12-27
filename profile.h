#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include "utilities.h"
#include "colorstring.h"

// connects to "profile.txt";
class Profile
{
public:	
	Profile(); // Get settings from the file
	void incrementNumWins();
	void incrementNumLosses();
	void incrementHighestDistortion();
	void setNewHighScore(int amount);

	int getHighestDistortion();
	int getHighestScore();
	int getNumWins();
	int getNumLosses();

	ColorString getColorString(); // colorstring off all info to display
private:
	void updateFile(); // update the file with the values stored in this class
	const std::string FILENAME;
	int highestDistortion;
	int highestScore;
	int numWins;
	int numLosses;
};

#endif