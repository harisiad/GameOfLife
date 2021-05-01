#ifndef __TERMINAL_H_
#define __TERMINAL_H_

class Terminal
{
public:
    Terminal();
    ~Terminal();

    void ClearTerminal();
    void RefreshRateTerminal(int);
    void AddChar(char*);
};

#endif