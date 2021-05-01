#include "fieldworkers.h"
#include "slacker.h"

FieldWorkers::FieldWorkers(std::string filename) :
    PlaygroundField(filename) 
{
    fieldWorkers = std::vector<std::vector<FieldWorker*>>(GetRows());
	for (std::vector<FieldWorker*> worker : fieldWorkers) {
		worker.reserve(GetColumns());
	}
}

FieldWorkers::~FieldWorkers()
{

}

void FieldWorkers::AssignWorkers()
{
	for (unsigned int i = 0; i < GetRows(); i++)
	{
		for (unsigned int j = 0; j < GetColumns(); j++)
		{
			fieldWorkers[i].push_back(
				new FieldWorker(
					GetC(i,0),
					(j == 0) ? nullptr : GetCell(i,j-1),
					(j == GetColumns() - 1) ? nullptr : GetCell(i,j+1),
					i,
					j,
					GetRows()
				)
			);
		}
	}

	std::cout << "Initialization Completed\n" << "Rows: " << GetRows();
	std::cout << fieldWorkers.size();
	
	for (std::vector<FieldWorker*> workerRow : fieldWorkers) {
		for (FieldWorker* worker : workerRow) {
			//std::cout << *worker->FOW();
			worker->StartWorkerJob();
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