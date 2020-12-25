#include "colorstring.h"
#include "utilities.h"

#include <string>
#include <vector>

//---------------------------------------------------------------------
// ColorChar functions

ColorChar::ColorChar()
{
    character = ' ';
    color = ddutil::getColor(ddutil::LIGHTGRAY, ddutil::BLACK);
}

ColorChar::ColorChar(char character, int color)
{
    this->character = character;
    this->color = color;
}

bool ColorChar::operator==(const ColorChar& other)
{
    return (character == other.character && color == other.color);
}

bool ColorChar::operator!=(const ColorChar& other)
{
    return !(*this == other);
}

//---------------------------------------------------------------------

//---------------------------------------------------------------------
// ColorString functions

ColorString::ColorString()
{

}

ColorString::ColorString(std::string str, int color)
{
    reserve(str.size());
    for (char i : str)
    {
        push_back(ColorChar(i, color));
    }
}

ColorString::ColorString(std::string str, std::vector<int> colors)
{
    if (str.size() != colors.size())
    {
        ddutil::errorMessage("String size does not match vector size. String is \"" + str + "\".", __LINE__, __FILE__);
    }

    reserve(str.size());
    for (unsigned int i = 0; i < str.size(); i++)
    {
        push_back(ColorChar(str[i], colors[i]));
    }
}

std::string ColorString::getString() const
{
    std::string str;

    str.reserve(size());
    for (auto it = begin(); it != end(); it++)
    {
        str += it->character;
    }

    return str;
}

std::vector<int> ColorString::getColors() const
{
    std::vector<int> colors;
    for (int i = 0; i < this->size(); i++)
    {
        colors.push_back(this->at(i).color);
    }
    return colors;
}

ColorString ColorString::operator+(const ColorString& other)
{
    ColorString colstr(*this);

    colstr.reserve(size() + other.size());
    for (auto i : other)
    {
        colstr.push_back(i);
    }

    return colstr;
}

ColorString ColorString::operator+(const ColorChar& other)
{
    ColorString colstr(*this);
    colstr.push_back(other);
    return colstr;
}


void ColorString::operator+=(const ColorString& other)
{
    reserve(size() + other.size());
    for (auto i : other)
    {
        push_back(i);
    }
}

void ColorString::operator+=(const ColorChar& other)
{
    push_back(other);
}

void ColorString::setColor(int color)
{
    for (auto it = begin(); it != end(); it++)
    {
        it->color = color;
    }
}
//---------------------------------------------------------------------