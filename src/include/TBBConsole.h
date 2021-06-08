#pragma once
#include <string>

enum ConsoleColors
{
    RESETCOLOR          = 0,
    Black_TXT           = 30,
    Red_TXT             = 31,
    Green_TXT           = 32,
    Yellow_TXT          = 33,
    Blue_TXT            = 34,
    Magenta_TXT         = 35,
    Cyan_TXT            = 36,
    White_TXT           = 37,
    BrightBlack_TXT     = 90,
    BrightRed_TXT       = 91,
    BrightGreen_TXT     = 92,
    BrightYellow_TXT    = 93,
    BrightBlue_TXT      = 94,
    BrightMagenta_TXT   = 95,
    BrightCyan_TXT      = 96,
    BrightWhite_TXT     = 97,
    Black_BKG           = 40,
    Red_BKG             = 41,
    Green_BKG           = 42,
    Yellow_BKG          = 43,
    Blue_BKG            = 44,
    Magenta_BKG         = 45,
    Cyan_BKG            = 46,
    White_BKG           = 47,
    BrightBlack_BKG     = 100,
    BrightRed_BKG       = 101,
    BrightGreen_BKG     = 102,
    BrightYellow_BKG    = 103,
    BrightBlue_BKG      = 104,
    BrightMagenta_BKG   = 105,
    BrightCyan_BKG      = 106,
    BrightWhite_BKG     = 107,
};

enum ClearCodes 
{
    CLEAR_FROM_CURSOR_TO_END,
    CLEAR_FROM_CURSOR_TO_BEGIN,
    CLEAR_ALL
};

class TBBConsole
{
    public:
        static void setupConsole();
        static void restoreConsole();
        static void resetColor();
        static void setTextColor(ConsoleColors color);
        static void setTextColorBright(ConsoleColors color);        
        static void setBackgroundColor(ConsoleColors color);
        static void setBackgroundColorBright(ConsoleColors color);
        static void clearScreen();
        static void clearScreenToBottom();
        static void clearScreenToTop();
        static void clearLine();
        static void clearLineToRight();
        static void clearLineToLeft();
        static void saveCursorPosition();
        static void restoreCursorPosition();
};