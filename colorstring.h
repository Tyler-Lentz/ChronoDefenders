#ifndef COLORSTRING_H
#define COLORSTRING_H

#include <vector>
#include <string>

struct ColorChar
{
    ColorChar();
    ColorChar(char character, int color);

    bool operator==(const ColorChar& other);
    bool operator!=(const ColorChar& other);

    int color;
    char character;
};

struct ColorString : public std::vector<ColorChar>
{
    // Default constructor, the vector will be empty
    ColorString();

    // Creates a colorstring that contains colorchars of all the same color
    ColorString(std::string, int color);

    // Creates a colorstring that assigns the color of index 0 to index 0 in the string, etc...
    ColorString(std::string, std::vector<int> colors);

    // Returns an std::string of all of the characters stored in the vector
    std::string getString()const;

    // Returns a colorstring that has the right colorstring appended onto this
    ColorString operator+(const ColorString& other);

    // Appends another colorstring to the end of this
    void operator+=(const ColorString& other);

    // Returns a colorstring that has the right colorchar appended onto this
    ColorString operator+(const ColorChar& other);

    // Appends a colorchar to the end of this
    void operator+=(const ColorChar& other);

    // Changes all of the characters color to the specified color
    void setColor(int color);
};

#endif