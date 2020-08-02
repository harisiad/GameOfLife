#ifndef __FIELDWORKERS_H__
#define __FIELDWORKERS_H__

#include "playgroundfield.h"
#include "fieldworker.h"

#include <thread>
#include <vector>

class FieldWorkers : public PlaygroundField
{
private:
    std::vector<std::vector<FieldWorker*>> fieldWorkers;
public:
    FieldWorkers(std::string);
    ~FieldWorkers();

    void AssignWorkers();
    void WaitJobsFinish();
};

#endif