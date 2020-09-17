#ifndef COORDINATE_H
#define COORDINATE_H

; struct Coordinate
{
    int x;
    int y;

    // Default constructor, leaves the values undefined
    Coordinate();

    // Sets the x and y to the specified amounts
    Coordinate(int x, int y);

    Coordinate operator+(const Coordinate& coord) const;

    bool operator==(const Coordinate& coord) const;

    bool operator<(const Coordinate& coord) const;

    bool operator!=(const Coordinate& coord) const;
};

#endif