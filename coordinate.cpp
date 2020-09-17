#include "coordinate.h"

// Default constructor, leaves the values undefined
Coordinate::Coordinate() 
{
    x = -1; y = -1; 
}

// Sets the x and y to the specified amounts
Coordinate::Coordinate(int x, int y)
{
    this->x = x;
    this->y = y;
}

Coordinate Coordinate::operator+(const Coordinate& coord) const
{
    return Coordinate(x + coord.x, y + coord.y);
}

bool Coordinate::operator==(const Coordinate& coord) const
{
    return ((x == coord.x) && (y == coord.y));
}

bool Coordinate::operator<(const Coordinate& coord) const
{
    if (x == coord.x)
    {
        return (y < coord.y);
    }
    else
    {
        return (x < coord.x);
    }
}

bool Coordinate::operator!=(const Coordinate& coord) const
{
    return (x != coord.x || y != coord.y);
}