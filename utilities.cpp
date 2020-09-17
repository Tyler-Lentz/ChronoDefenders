#include "utilities.h"
#include "game.h"
#include "player.h"
#include"soundfile.h"
#include "creature.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <list>
#include <numeric>
#include <chrono>

namespace ddutil
{
    void errorMessage(std::string error, int line, std::string file)
    {
        std::cerr << "Error on line " << line << " in the file " << file << "\n- " << error;
        Sleep(100000);
        exit(1);
    }

    int getColor(int letterColor, int backgroundColor, int numberOfColors)
    {
        return (letterColor + (backgroundColor * numberOfColors));
    }

    int random(int max)
    {
        return random(0, max);
    }

    int random(int min, int max)
    {
        if (max < min)
        {
            std::swap(max, min);
        }
        static std::random_device rd;
        static std::mt19937 generator(rd());
        std::uniform_int_distribution<> distro(min, max);
        return distro(generator);
    }

	std::vector<int> uniqueRandom(int min, int max, unsigned int amount)
	{
        // get a list of possible numbers to choose from
        std::vector<int> numberList(max - min + 1);
        std::iota(numberList.begin(), numberList.end(), min);
        if (amount > numberList.size())
        {
            amount = numberList.size();
        }

        // randomly shuffle the list
        unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
        std::shuffle(numberList.begin(), numberList.end(), std::default_random_engine(seed));
        
        // return however many ints were required (amount), and they will be random because the list was shuffled
        std::vector<int> result;
        for (unsigned int i = 0; i < amount; i++)
        {
            result.push_back(numberList[i]);
        }
        return result;
	}

    // returns in the form "5 damage (2 blocked)"
    ColorString genericDamageAlert(DamageReport damage)
	{
        ColorString s2 = ColorString(std::to_string(damage.getDamageTaken()) + " damage ", ddutil::DAMAGE_COLOR);
        ColorString s3 = ColorString("(" + std::to_string(damage.getDamageBlocked()) + " blocked)", ddutil::BLOCK_COLOR);
        return s2 + s3;
	}

    ColorString genericBlockAlert(int amount)
    {
        return ColorString(std::to_string(amount) + " damage", ddutil::BLOCK_COLOR);
    }

	ColorString genericDamageIntent(int damage, ColorString monsterName, std::string attackName, std::vector<Creature*> targets)
	{
        ColorString intent = ColorString("The ", ddutil::TEXT_COLOR) + monsterName + ColorString(" intends to " + attackName + " ", ddutil::TEXT_COLOR) +
            Game::getPartyNames(targets) + ColorString("for ", ddutil::TEXT_COLOR) + ColorString(std::to_string(damage), ddutil::DAMAGE_COLOR) +
            ColorString(" damage", ddutil::DAMAGE_COLOR);
        ddutil::trimLength(intent);
        return intent;
	}

    ColorString genericBlockIntent(int amount, ColorString monsterName)
    {
        return ColorString("The ", ddutil::TEXT_COLOR) + monsterName + ColorString(" intends to block for ", ddutil::TEXT_COLOR)
            + ddutil::genericBlockAlert(amount);
    }

    ColorString getHealthBar(int hp, int maxHp, int length)
    {
        char empty = '-';
        char full = '=';

        int color = ddutil::GREEN; // assume green, check to see if this needs to be changed

        if (hp < static_cast<int>(0.33 * maxHp))
        {
            color = ddutil::RED;
        }
        else if (hp < static_cast<int>(0.67 * maxHp))
        {
            color = ddutil::YELLOW;
        }

        double percentageOfHp = static_cast<double>(hp) / maxHp;

        // how many characters of 'length' will represent health (=)
        int numberOfFullSpaces = static_cast<int>(percentageOfHp * length);

        // how many characters of 'length' will represent no health (-)
        int numberOfEmptySpaces = length - numberOfFullSpaces;

        // add the appropriate amount of characters to the bar
        std::string barFull(numberOfFullSpaces, full);
        std::string barEmpty(numberOfEmptySpaces, empty);

        return ColorString(barFull + barEmpty, color);
    }

	std::string cardNumToString(int num)
	{
        switch (num)
        {
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            return std::to_string(num);

        case 11:
            return "Jack";
            
        case 12:
            return "Queen";

        case 13:
            return "King";

        case 14:
            return "Ace";
            
        default:
            return "???";
        }
	}

	void trimLength(ColorString& str)
	{
        ColorString trailingText = ColorString("...", ddutil::TEXT_COLOR);
        if (str.getString().length() >= ddutil::CONSOLEX)
        {
            ColorString trimmedResult;
            for (unsigned int i = 0; i < ddutil::CONSOLEX - trailingText.getString().length(); i++)
            {
                trimmedResult += str[i];
            }
            trimmedResult += trailingText;
            str = trimmedResult;
        }
	}

    Creature* getHighestHealthPlayer(std::vector<Creature*> players)
    {
        Creature* highestHealth = nullptr;
        if (!players.empty())
        {
            highestHealth = players[0];
            for (Creature* p : players)
            {
                if (p->getHealth() > highestHealth->getHealth())
                {
                    highestHealth = p;
                }
            }
        }
        return highestHealth;
    }

    Creature* getLowestHealthPlayer(std::vector<Creature*> players)
    {
        Creature* lowestHealth = nullptr;
        if (!players.empty())
        {
            lowestHealth = players[0];
            for (Creature* p : players)
            {
                if (p->getHealth() < lowestHealth->getHealth())
                {
                    lowestHealth = p;
                }
            }
        }
        return lowestHealth;
    }


    bool keypress(int key)
    {
        if (!(GetAsyncKeyState(key) < 0)) // if its not pressed
        {
            return false;
        }

        if (GetConsoleWindow() != GetForegroundWindow()) // if the window is not active (i.e. looking at something else)
        {
            return false;
        }
        return true;
    }

    bool keyrelease(int key)
    {
        if (GetAsyncKeyState(key) < 0) // if the key is down
        {
            return false;
        }
        if (GetConsoleWindow() != GetForegroundWindow())
        {
            return false;
        }
        return true;
    }

    bool keyhold(int key)
    {
        if (!(GetAsyncKeyState(key) < 0)) // if its not pressed
        {
            return false;
        }

        if (GetConsoleWindow() != GetForegroundWindow()) // if the window is not active (i.e. looking at something else)
        {
            return false;
        }
        return true;
    }

    void waitForKeyPress(int key)
    {
        while (!keypress(key))
        {
            // do nothing
        }
        while (keypress(key))
        {
            // do nothing
        }
    }

    bool hasCoords(Coordinate coord, std::vector<Coordinate> coords)
    {
        bool hasCoords = false;
        for (Coordinate c : coords)
        {
            if (c == coord)
            {
                hasCoords = true;
                break;
            }
        }
        return hasCoords;
    }
}

