#include "virtualwindow.h"
#include "colorstring.h"
#include "utilities.h"
#include "coordinate.h"
#include "console.h"
#include "soundfile.h"

#include <iostream>
#include <cmath>
#include <string>
#include <mutex>



//-------------------------------------------------------------
// VirtualWindow functions

VirtualWindow::VirtualWindow(unsigned int width, unsigned int height) :
    console(width, height)
{
    vwin.resize(height, ColorString(std::string(width, ' '), ddutil::getColor(ddutil::LIGHTGRAY, ddutil::BLACK)));
    this->width = width;
    this->height = height;
}

void VirtualWindow::put(ColorChar colchar, Coordinate coord)
{
    refreshMut.lock();

    if (vwin[coord.y][coord.x] != colchar)
    {
        vwin[coord.y][coord.x] = colchar;
        posToDraw.push_back(coord);
    }
    refreshMut.unlock();
}

void VirtualWindow::put(ColorString colstr, Coordinate coord, bool scrolling)
{
    if (scrolling)
    {
        playSound(WavFile("LetterSound", true, true));
    }

    for (auto i : colstr)
    {
        put(i, coord);
        if (scrolling)
        {
            Sleep(ddutil::TEXT_SCROLL_TIME);
        }

        coord.x++;
        if (coord.x >= ddutil::CONSOLEX)
        {
            coord.x = 0;
            coord.y++;
        }
    }

    if (scrolling)
    {
        stopSound(SoundType::WAV);
    }
}

void VirtualWindow::putcen(ColorChar colchar, unsigned int line)
{
    put(colchar, Coordinate(static_cast<int>((width - 1) / 2), line));
}

void VirtualWindow::putcen(ColorString colstr, unsigned int line, bool scrolling)
{
    Coordinate coord(static_cast<int>((width - colstr.size()) / 2), line);

    if (scrolling)
    {
        playSound(WavFile("LetterSound", true, true));
    }

    for (auto i : colstr)
    {
        put(i, coord);
        if (scrolling)
        {
            Sleep(ddutil::TEXT_SCROLL_TIME);
        }
        coord.x++;
        if (coord.x >= ddutil::CONSOLEX)
        {
            coord.x = 0;
            coord.y++;
        }
    }

    if (scrolling)
    {
        stopSound(SoundType::WAV);
    }
}

void VirtualWindow::putcenSlowScroll(ColorString colstr, unsigned int line, bool slow)
{
    int WAIT_TIME = ddutil::TEXT_SCROLL_TIME * 2;
    if (slow)
    {
        WAIT_TIME *= 2;
    }

    Coordinate coord(static_cast<int>((width - colstr.size()) / 2), line);

    playSound(WavFile("textsound", true, true));

    for (auto i : colstr)
    {
        put(i, coord);
        if (!ddutil::keypress(VK_RETURN))
        {
            Sleep(WAIT_TIME);
        }
        coord.x++;
        if (coord.x >= ddutil::CONSOLEX)
        {
            coord.x = 0;
            coord.y++;
        }
    }

    while (ddutil::keypress(VK_RETURN));

    stopSound(SoundType::WAV);
}

void VirtualWindow::printArt(std::vector<ColorString> art, Coordinate topLeft, bool centered)
{
    int x = topLeft.x;
    int y = topLeft.y;

    if (!centered)
    {
        for (auto i : art)
        {
            put(i, Coordinate(x, y++));
        }
    }
    else
    {
        for (auto i : art)
        {
            putcen(i, y++);
        }
    }
}

void VirtualWindow::printArtFromBottom(std::vector<ColorString> art, Coordinate bottomLeft, bool centered)
{
    int x = bottomLeft.x;
    int y = bottomLeft.y;

    if (!centered)
    {
        for (auto i = art.rbegin(); i != art.rend(); i++)
        {
            put(*i, Coordinate(x, y--));
        }
    }
    else
    {
        for (auto i = art.rbegin(); i != art.rend(); i++)
        {
            putcen(*i, y--);
        }
    }
}

void VirtualWindow::clearArt(std::vector<ColorString> art, Coordinate bottomLeft)
{
    int x = bottomLeft.x;
    int y = bottomLeft.y;

    for (auto i = art.rbegin(); i != art.rend(); i++)
    {
        put(ColorString(std::string(i->getString().length(), ' '), ddutil::TEXT_COLOR), Coordinate(x, y--));
    }
}

ColorChar VirtualWindow::highlight(Coordinate pos, int backColor)
{
    ColorChar original = vwin[pos.y][pos.x];
    put(ColorChar(original.character, ddutil::getColor(original.color, backColor)), pos);
    return original;
}

ColorString VirtualWindow::getLine(unsigned int line) const
{
    return vwin[line];
}

ColorChar VirtualWindow::getChar(Coordinate c)
{
    return vwin[c.y][c.x];
}

void VirtualWindow::refresh()
{
    refreshMut.lock();

    for (auto i : posToDraw)
    {
        ColorChar charToDraw = vwin[i.y][i.x];
        console.setCursorPos(i.x, i.y);
        console.setColor(charToDraw.color);
        std::cout << charToDraw.character;
    }

    posToDraw.clear();
    console.setColor(ddutil::LIGHTGRAY);

    refreshMut.unlock();
}

const Console& VirtualWindow::getConsole() const
{
    return console;
}

void VirtualWindow::clearScreen(int color)
{
    ColorString line(std::string(ddutil::CONSOLEX, ' '), ddutil::getColor(color, color));

    for (int i = 0; i < ddutil::CONSOLEY; i++)
    {
        putcen(line, i);
    }
}

void VirtualWindow::clearLine(int lineNum, int color)
{
    ColorString line(std::string(ddutil::CONSOLEX, ' '), color);

    putcen(line, lineNum);
}

//-------------------------------------------------------------