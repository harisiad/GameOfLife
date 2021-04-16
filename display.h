#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "terminal.h"
#include "fieldworkers.h"

class Display
{
private:
    static FieldWorkers* field;
    static Terminal currTerm;
    static std::mutex glMutex;

    std::thread* displayThrd;
public:
    Display(std::string);
    ~Display();

    static void DisplayJob();
    void Run();
};

#endif