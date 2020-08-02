#ifndef __SLACKER_H__
#define __SLACKER_H__

#include "fieldworker.h"

class Slacker : public FieldWorker
{
public:
    inline Slacker() {};

    inline std::string GetClassName() { return "Slacker"; };
};

#endif