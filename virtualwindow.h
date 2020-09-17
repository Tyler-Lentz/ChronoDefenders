#ifndef VIRTUALWINDOW_H
#define VIRTUALWINDOW_H

#include "colorstring.h"
#include "coordinate.h"
#include "console.h"
#include "utilities.h"

#include <vector>
#include <list>
#include <mutex>

typedef std::vector<ColorString> VWIN;

class VirtualWindow
{
public:
    // Sets the VWIN to the size of width,height
    VirtualWindow(unsigned int width, unsigned int height);

    // Outputs the updated information to the console
    void refresh();

    // Puts the char/string to the specified location in the VWIN, for strings
    // it is the starting location
    void put(ColorChar colchar, Coordinate coord);
    void put(ColorString colstr, Coordinate coord, bool scrolling = false);
    // These center the string/char on the specified line
    void putcen(ColorChar colchar, unsigned int line);
    void putcen(ColorString colstr, unsigned int line, bool scrolling = false);
    void putcenSlowScroll(ColorString colstr, unsigned int line, bool slow = false);

    void printArt(std::vector<ColorString> art, Coordinate topLeft, bool centered);
    void printArtFromBottom(std::vector<ColorString> art, Coordinate bottomLeft, bool centered);
    void clearArt(std::vector<ColorString> art, Coordinate bottomLeft);

    // returns the original colorchar that was highlighted
    ColorChar highlight(Coordinate pos, int color);

    // clears the entire screen and sets it to the color specified, default is black
    void clearScreen(int color = ddutil::BLACK);

    void clearLine(int line, int color = ddutil::BLACK);

    const Console& getConsole() const;

    ColorString getLine(unsigned int line) const;

    ColorChar getChar(Coordinate c);// returns the colorchar at the given coordinate
private:
    // Vector that holds the updated information to draw to the console
    VWIN vwin;

    // Handles locking when accessing posToDraw
    std::mutex refreshMut;

    Console console;

    // List that holds all of the updated positions in the virtual window
    std::list<Coordinate> posToDraw;

    // width and height of the virtual window
    unsigned int width;
    unsigned int height;
};

#endif