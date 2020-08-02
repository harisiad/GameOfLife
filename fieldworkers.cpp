#include "fieldworkers.h"
#include "slacker.h"

FieldWorkers::FieldWorkers(std::string filename) :
    PlaygroundField(filename) 
{
    fieldWorkers = std::vector<std::vector<FieldWorker*>>(GetRows());
}

FieldWorkers::~FieldWorkers()
{

}

void FieldWorkers::AssignWorkers()
{
	for (unsigned int i = 0; i < GetRows(); i++)
	{
		fieldWorkers.push_back(std::vector<FieldWorker*>(GetColumns()));
		for (unsigned int j = 0; j < GetColumns(); j++)
		{
			fieldWorkers[i].push_back(new FieldWorker(&GetField()[i][j],i,j,GetColumns()));
		}
	}
}

void FieldWorkers::WaitJobsFinish()
{
    for (unsigned int i = 0; i < GetRows(); i++)
	{
		for(unsigned int j = 0; j < GetColumns(); j++)
		{
			fieldWorkers[i][j]->WorkerJobFinish();
		}
	}
}