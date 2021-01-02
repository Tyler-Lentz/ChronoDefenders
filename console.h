#ifndef CONSOLE_H
#define CONSOLE_H

#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include "utilities.h"
#include <stdexcept>

struct Console
{
    Console(unsigned width, unsigned height)
    {
        SMALL_RECT r;
        COORD      c;
        hConOut = GetStdHandle(STD_OUTPUT_HANDLE);

        r.Left =
            r.Top = 0;
        r.Right = width - 1;
        r.Bottom = height - 1;
        SetConsoleWindowInfo(hConOut, TRUE, &r);

        c.X = width;
        c.Y = height;
        SetConsoleScreenBufferSize(hConOut, c);

        CONSOLE_CURSOR_INFO     cursorInfo;

        GetConsoleCursorInfo(hConOut, &cursorInfo);
        cursorInfo.bVisible = false;
        cursorInfo.dwSize = 1;
        SetConsoleCursorInfo(hConOut, &cursorInfo);

        HWND consoleWindow = GetConsoleWindow();
        SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

        setFontSize(ddutil::DEFAULT_CON_SIZE);
    }

    ~Console()
    {
        SetConsoleTextAttribute(hConOut, csbi.wAttributes);
        SetConsoleScreenBufferSize(hConOut, csbi.dwSize);
        SetConsoleWindowInfo(hConOut, TRUE, &csbi.srWindow);
    }

    void setFontSize(int size) const
    {
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof cfi;
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0;
        cfi.dwFontSize.Y = size;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;

        wcscpy_s(cfi.FaceName, L"Lucida Console");
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    }

    void setTitle() const
    {
        SetConsoleTitle(L"Chrono Defenders");
    }

    void setColor(int color)
    {
        SetConsoleTextAttribute(hConOut, color);
    }

    void setCursorPos(int x, int y)
    {
        COORD cord = {
            static_cast<SHORT>(x),
            static_cast<SHORT>(y)
        };
        SetConsoleCursorPosition(hConOut, cord);
    }

    HANDLE                     hConOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
};

#endif




