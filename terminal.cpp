#include "terminal.h"

#include <term.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <curses.h>

Terminal::Terminal()
{
    initscr();
}

Terminal::~Terminal()
{
    endwin();   
}

void Terminal::ClearTerminal()
{
    clear();
}

void Terminal::AddChar(char c)
{
    addch(c);
}

void Terminal::RefreshRateTerminal(int rate)
{
    refresh();
    //usleep(rate);
    std::this_thread::sleep_for(std::chrono::milliseconds(rate));
}