#include <string>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include "TBBConsole.h"

static struct termios orig_term;
static struct termios new_term;

void TBBConsole::setupConsole() {
    tcgetattr(STDIN_FILENO, &orig_term);
    new_term = orig_term;

    new_term.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

void TBBConsole::restoreConsole() {
    // Reset colors
    printf("\x1b[0m");

    // Reset console mode
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
}

void TBBConsole::resetColor() {
    printf("\x1b[%dm", ConsoleColors::RESETCOLOR);
}

void TBBConsole::setTextColor(ConsoleColors color)
{
    printf("\x1b[%dm", color);
}

void TBBConsole::setTextColorBright(ConsoleColors color)
{
    printf("\x1b[%d;1m", color);
}

void TBBConsole::setBackgroundColor(ConsoleColors color)
{
    printf("\x1b[%dm", color);
}

void TBBConsole::setBackgroundColorBright(ConsoleColors color)
{
    printf("\x1b[%d;1m", color);
}

void TBBConsole::clearScreen()
{
    printf("\x1b[%dJ", CLEAR_ALL);
}

void TBBConsole::clearScreenToBottom()
{
    printf("\x1b[%dJ", CLEAR_FROM_CURSOR_TO_END);
}

void TBBConsole::clearScreenToTop()
{
    printf("\x1b[%dJ", CLEAR_FROM_CURSOR_TO_BEGIN);
}

void TBBConsole::clearLine()
{
    printf("\x1b[%dK", CLEAR_ALL);
}

void TBBConsole::clearLineToRight()
{
    printf("\x1b[%dK", CLEAR_FROM_CURSOR_TO_END);
}

void TBBConsole::clearLineToLeft()
{
    printf("\x1b[%dK", CLEAR_FROM_CURSOR_TO_BEGIN);
}

void TBBConsole::saveCursorPosition() {
    printf("\x1b%d", 7);
}

void TBBConsole::restoreCursorPosition() {
    printf("\x1b%d", 8);
}