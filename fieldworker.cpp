#include "fieldworker.h"

char* FieldWorker::FoW;
int FieldWorker::size;
bool FieldWorker::isTribe;
std::mutex FieldWorker::glMutex;
unsigned int FieldWorker::growthRate;

FieldWorker::FieldWorker(char* data, int r, int c, int size)
{
    row = r; col = c; FieldWorker::size = size;

    FieldWorker::FoW = (char*) malloc(sizeof(char));

    if ((*data) == 'X')
    {
        FieldWorker::FoW = data;
        (*FieldWorker::FoW) = '5';
        MarkAsTribe();
        growthRate = 1;
    }
    else
    {
        FieldWorker::FoW = data;
        FieldWorker::isTribe = false;
        growthRate = 0;
    }
    worker = new std::thread(&FieldWorker::WorkerJob);
}

void FieldWorker::WorkerJob()
{
    while (true)
    {
        if (IsTribe())
        {
            //std::lock_guard<std::mutex> lg(glMutex);
            glMutex.lock();
            if ((*FieldWorker::FoW) == '9')
            {
                growthRate = 0;
            }
            else if ((*FieldWorker::FoW) < '9')
            {
                GrowthRate();
                (*FieldWorker::FoW) += growthRate;
            }
            glMutex.unlock();
        }
        else if ((*FieldWorker::FoW) == 'F' || 
            (*FieldWorker::FoW) == 'W' || 
            (*FieldWorker::FoW) == 'M')
        {
            break;
        }
        else if (IsEmptyField())
        {
            if(CanMigrate())
            {
                glMutex.lock();
                if (((int)(*(FieldWorker::FoW - 1)) > 1))
                {
                    (*(FieldWorker::FoW - 1))--;
                    (*FieldWorker::FoW)++;
                }
                else if (((int)(*(FieldWorker::FoW + 1)) > 1))
                {
                    (*(FieldWorker::FoW + 1))--;
                    (*FieldWorker::FoW)++;
                }
                else if (((int)(*(FieldWorker::FoW - FieldWorker::size)) > 1))
                {
                    (*(FieldWorker::FoW - FieldWorker::size))--;
                    (*FieldWorker::FoW)++;
                }
                else if (((int)(*(FieldWorker::FoW + FieldWorker::size)) > 1))
                {
                    (*(FieldWorker::FoW + FieldWorker::size))--;
                    (*FieldWorker::FoW)++;
                }
                glMutex.unlock();
                MarkAsTribe();
            }
        }
    }
}

bool FieldWorker::CanMigrate()
{
    if (IsForest(1) ||
        IsWater(1) ||
        IsMountain(1) ||
        IsNeighbourEmptyField(1))
    {
        return false;
    }

    return true;
}

void FieldWorker::GrowthRate()
{
    if((IsForest(1) || IsForest(2)) &&
        (IsWater(1) || IsWater(2)))
    {
        growthRate = 2;
    }
    else if (IsForest(1) || IsForest(2))
    {
        growthRate = 1;
    }
    else if (IsWater(1) || IsWater(2))
    {
        growthRate = 1;
    }
}

bool FieldWorker::IsForest(int size)
{
    return (((*(FieldWorker::FoW - size)) == 'F') ||
            ((*(FieldWorker::FoW + size)) == 'F') ||
            ((*(FieldWorker::FoW - FieldWorker::size)) == 'F') ||
            ((*(FieldWorker::FoW + FieldWorker::size)) == 'F'));
}

bool FieldWorker::IsWater(int size)
{
    return (((*(FieldWorker::FoW - size)) == 'W') ||
            ((*(FieldWorker::FoW + size)) == 'W') ||
            ((*(FieldWorker::FoW - FieldWorker::size)) == 'W') ||
            ((*(FieldWorker::FoW + FieldWorker::size)) == 'W'));
}

bool FieldWorker::IsMountain(int size)
{
    return (((*(FieldWorker::FoW - size)) == 'M') ||
            ((*(FieldWorker::FoW + size)) == 'M') ||
            ((*(FieldWorker::FoW - FieldWorker::size)) == 'M') ||
            ((*(FieldWorker::FoW + FieldWorker::size)) == 'M'));
}

bool FieldWorker::IsNeighbourEmptyField(int size)
{
    return (((*(FieldWorker::FoW - size)) == '0') ||
            ((*(FieldWorker::FoW + size)) == '0') ||
            ((*(FieldWorker::FoW - FieldWorker::size)) == '0') ||
            ((*(FieldWorker::FoW + FieldWorker::size)) == '0'));
}

void FieldWorker::WorkerJobFinish()
{
    if (worker != NULL && 
        worker->joinable())
    {
        worker->join();
    }
}

FieldWorker::~FieldWorker() 
{
    delete worker;
}