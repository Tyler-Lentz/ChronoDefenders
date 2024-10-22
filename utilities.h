#ifndef UTILITIES_H
#define UTILITIES_H

#include "coordinate.h"
#include "colorstring.h"

#include <string>
#include <fstream>
#include <list>
#include <vector>

class Player;
class Creature;

namespace ddutil
{
    enum colors
    {
        BLACK,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHTGRAY,
        DARKGRAY,
        LIGHTBLUE,
        LIGHTGREEN,
        LIGHTCYAN,
        LIGHTRED,
        LIGHTMAGENTA,
        YELLOW,
        WHITE
    };

    // tells the program whether to restart or close out of the game
    enum class GameStatus
    {
        RESTART,
        EXIT,
        CONTINUE
    };

    // percentage chance of getting artifacts of specific rarity per zone
    const int CATACOMBS_MOD_CHANCE = 75;
    const int CATACOMBS_POW_CHANCE = 25;
    const int CATACOMBS_MYTH_CHANCE = 0;

    const int ABYSS_MOD_CHANCE = 45;
    const int ABYSS_POW_CHANCE = 50;
    const int ABYSS_MYTH_CHANCE = 5;
    
    const int VOID_MOD_CHANCE = 0;
    const int VOID_POW_CHANCE = 75;
    const int VOID_MYTH_CHANCE = 25;

    const int PALACE_MOD_CHANCE = 0;
    const int PALACE_POW_CHANCE = 0;
    const int PALACE_MYTH_CHANCE = 100;

    const int NORM_ENEMY_SCORE = 10;
    const int STRONG_ENEMY_SCORE = 30;
    const int BOSS_ENEMY_SCORE = 100;
    const int MOD_ARTIFACT_SCORE = 10;
    const int POW_ARTIFACT_SCORE = 30;
    const int MYTH_ARTIFACT_SCORE = 50;
    const int MOD_MOVE_SCORE = 10;
    const int POW_MOVE_SCORE = 30;
    const int MYTH_MOVE_SCORE = 50;
    const int VICTORY_SCORE = 300;
    const int SCORE_PER_ALIVE_PLAYER = 100;

    const int BUFFER_TIME = 225;

    const char MAP_BOSS = 'B'; // 234
    const char MAP_EVENT = '?';
    const int EVENT_COLOR = ddutil::WHITE;
    const char MAP_NENEMY = 'E'; //233
    const char MAP_SENEMY = 'S'; //232
    const char MAP_FIRE = '~';
    const char MAP_REVIVE = '+';
    const int REVIVE_COLOR = ddutil::LIGHTGREEN;
    const int FIRE_COLOR = ddutil::LIGHTRED;
    const int MAP_HIGHLIGHT_COLOR = ddutil::LIGHTCYAN;


    const int TEXT_COLOR = WHITE;
    const int BLOCK_COLOR = LIGHTBLUE;
    const int DAMAGE_COLOR = RED;
    const int STATUS_COLOR = MAGENTA;
    const int VITALITY_COLOR = LIGHTRED;
    const int HEAL_COLOR = GREEN;
    const int EXPERIENCE_COLOR = ddutil::LIGHTMAGENTA;

    const int MENU_SELECTION_COLOR = ddutil::GREEN;

    const int SAMURAI_COLOR = ddutil::RED;
    const int PALADIN_COLOR = ddutil::LIGHTGRAY;
    const int GUNSLINGER_COLOR = ddutil::BROWN;
    const int SORCERER_COLOR = ddutil::LIGHTBLUE;
    
    const int AURA_COLOR = ddutil::LIGHTCYAN;

    const int BESERKER_COLOR = ddutil::SAMURAI_COLOR;
    const int NOMAD_COLOR = ddutil::SAMURAI_COLOR;
    const int NINJA_COLOR = ddutil::SAMURAI_COLOR;

    const int CLERIC_COLOR = ddutil::SORCERER_COLOR;
    const int SUMMONER_COLOR = ddutil::SORCERER_COLOR;
    const int TEMPEST_COLOR = ddutil::SORCERER_COLOR;

    const int BRAWLER_COLOR = ddutil::BROWN;
    const int GAMBLER_COLOR = ddutil::LIGHTBLUE;
    const int SHARPSHOOTER_COLOR = ddutil::LIGHTMAGENTA;

    const int COMPENDIUM_COLOR = ddutil::CYAN;
    const int DISTORTION_COLOR = ddutil::MAGENTA;
    const int SCORE_COLOR = ddutil::YELLOW;
    const int WIN_COLOR = ddutil::GREEN;
    const int LOSS_COLOR = ddutil::RED;

    const int WEAK_COLOR = ddutil::LIGHTGRAY;
    const int MODERATE_COLOR = ddutil::GREEN;
    const int POWERFUL_COLOR = ddutil::CYAN;
    const int MYTHICAL_COLOR = ddutil::RED;
    const int COSMIC_COLOR = ddutil::YELLOW;

    const unsigned int BOSS_ARTIFACT_CHOICES = 3;

    const int MODERATE_COST = 20;
    const int POWERFUL_COST = 100;
    const int MYTHICAL_COST = 250;

    const int ARTIFACT_COLOR = ddutil::YELLOW;

    const int BLACK_CARD_COLOR = ddutil::DARKGRAY;
    const int RED_CARD_COLOR = ddutil::LIGHTRED;

    const int PLAYER_HEALTHBAR_LENGTH = 20;
    const int BOSS_HEALTHBAR_LENGTH = 60;

    const int CONSOLEX = 100;
    const int CONSOLEY = 40;

    const int TEXT_SCROLL_TIME = 20;

    const int DIVIDER_LINE3 = 28;
    const int BOTTOM_TEXT_LINE = 29;
    const int CENTER_TEXT_LINE = 26;
    const int DIVIDER_LINE4 = 39;
    const char DIVIDER_CHARACTER = '#';

    const int EVENT_PICTURE_LINE = DIVIDER_LINE3 - 7;

    const bool SF_LOOP = false;
    const bool SF_ASYNC = false;

    const int ABSOLUTE_MAX_MOVES = 7;

    const int DIST1_HP_PERCENT = 75;
    const int DIST2_MOVEXP_PERCENT = 33;
    const int DIST3_MAXHP_MINUS = 10;
    const int DIST4_PERC_ENEMY_HP_INC = 20;
    const int NORMAL_REVIVAL_PERCENT = 67;

    const int DEFAULT_CON_SIZE = 16;

    const int START_MOVES_TO_CHOOSE_FROM = 4;

    // sorry
    const std::string NOT_ENOUGH_BULLETS = "Not enough bullets!";
    const std::string NOT_ENOUGH_AURA = "Not enough Aura!";
    const std::string CANT_USE_MOVE = "Can't use this move!";

    void errorMessage(std::string error, int line, std::string file);

    int getColor(int letterColor, int backgroundColor, int numberOfColors = 16);
    const ColorString PATRIARCH_STRING = ColorString("The Patriarch", getColor(RED, YELLOW));
    const ColorString WATCHER_STRING = ColorString("The Watcher", getColor(LIGHTMAGENTA, YELLOW));

    bool keypress(int key);

    bool keyrelease(int key);

    bool keyhold(int key); // like keypress but doesnt wait time after confirming the keypress to prevent double presses

    void waitForKeyPress(int key);

    int random(int max);

    int random(int min, int max);

    // returns amount unique random numbers from min to max
    std::vector<int> uniqueRandom(int min, int max, unsigned int amount);

    // class that helps transfer information about how much damage is taken and blocked
    class DamageReport
    {
    public:
        DamageReport(int dBlocked, int dTaken);
        int getDamageBlocked();
        int getDamageTaken();

    private:
        int damageBlocked;
        int damageTaken;
    };

    // the message returned depends on whether or not its from the perspective of the player
    // true = player is doing damage / applying block / applying status
    // false = enemy is doing damage / applying block / applying status
    ColorString genericDamageAlert(DamageReport damage);
    ColorString genericBlockAlert(int amount);

    // message displaying enemy intent
    ColorString genericDamageIntent(int damage, ColorString monsterName, std::string attackName, std::vector<Creature*> targets);
    ColorString genericBlockIntent(int amount, ColorString monsterName);


    // used to tell what type of enemy to be generated in zone.cpp
    enum class EnemyType
    {
        Normal,
        Strong,
        Boss
    };
    
    // converts the num to a string, and if it is 11, 12, 13, or 14 convert to corresponding card
    std::string cardNumToString(int num);

    // Trims length to make sure the game does not crash
    void trimLength(ColorString& str);

    // returns the player with the most/least Health
    // returns nullptr if the vector is empty
    Creature* getHighestHealthPlayer(std::vector<Creature*> players);
    Creature* getLowestHealthPlayer(std::vector<Creature*> players);



    // utility function used in generating the map
    // returns true if the coordinate is already in the vector of coordinates
    bool hasCoords(Coordinate coord, std::vector<Coordinate> coords);

    // makes the string the desired length by adding an equal amount of spaces before and after the string
    std::string padString(std::string str, int size);
    ColorString padString(ColorString str, int size);


    const std::vector<std::string> minionNames = {
        "Harold",
        "Jerry",
        "Justin",
        "Cameron",
        "Collin",
        "Alec",
        "Brandon",
        "Aidan",
        "Tyler",
        "Zach",
        "Nick",
        "Julia",
        "Kaela",
        "Hailey",
        "Cassidy",
        "Jodi",
        "Jim",
        "Margaret",
        "Casio",
        "Stephen",
        "Tybalt",
        "John",
        "Kyle",
        "Garret",
        "Kristian",
        "Thomas",
        "Chris",
        "Joseph",
        "Pauline",
        "Gabi",
        "Eren",
        "Deku",
        "Kade",
        "Andrew",
        "Jeff",
        "Dana",
        "Julie",
        "Bradley",
        "Daniel",
        "Wesley",
        "Cole",
        "Jacqueline",
        "Beeg",
        "Yoshi",
        "Timothy",
        "Xavier",
        "Billy",
        "Jessica",
        "Shayla",
        "Linguini",
        "Groob",
        "Zerb",
        "Sean",
        "Emily",
        "Talia",
        "Isabella",
        "Abigail",
        "Chloe",
        "Hannah",
        "Lucy",
        "Korra",
        "Aang",
        "Sokka",
        "Toph",
        "Katara",
        "Kya",
        "Suki",
        "Tenzin",
        "Gyatso",
        "Sammy",
        "Cassandra",
        "Troy",
        "Bertolt",
        "Apollo",
        "Axel",
        "Barney",
        "Appa",
        "Barry",
        "Kash",
        "Ivy",
        "Palindrome",
        "Cookie",
        "Lucas",
        "Erika",
        "Sally",
        "Bumi",
        "Chad",
        "Flameo"
    };
    std::string getCuteMinionName();

    ColorString getDistortionDescription(int level);

    void xorFile(std::string filename);
}
#endif