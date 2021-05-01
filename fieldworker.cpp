#include "fieldworker.h"
#include "display.h"

#include <curses.h>
#include <cstring>

extern Display dis;

// std::string* FieldWorker::FoW;
// int FieldWorker::size;
// int FieldWorker::rSize;
// bool FieldWorker::isTribe {false};
// bool FieldWorker::isGroundTile {false};
// bool FieldWorker::isEmptyField {false};
// std::mutex FieldWorker::glMutex;
// unsigned int FieldWorker::growthRate;
// int FieldWorker::row, FieldWorker::col;

FieldWorker::FieldWorker(std::string* data, char* upCell, char* downCell, int r, int c, int rows)
{
    row = r; col = c; FieldWorker::size = data->length();
    FieldWorker::rSize = rows;

    FieldWorker::FoW = (std::string*)malloc(sizeof(std::string));

    FieldWorker::FoW = data;
    
    if ((char)(*FieldWorker::FoW)[col] == 'X')
    {
        (*FieldWorker::FoW)[col] = '5';
        MarkAsTribe();
        growthRate = 1;
        //worker = new std::thread(&FieldWorker::WorkerJob, this);
    }
    else if ((char)(*FieldWorker::FoW)[col] == 'F' ||
             (char)(*FieldWorker::FoW)[col] == 'W' ||
             (char)(*FieldWorker::FoW)[col] == 'M')
    {
        MarkAsGroundTile();
        growthRate = 0;
    }
    else if ((char)(*FieldWorker::FoW)[col] == '.')
    {
        MarkAsEmptyField();
        growthRate = 0;
        //worker = new std::thread(&FieldWorker::WorkerJob, this);
    }
    else
    {
        MarkAsGroundTile();
        growthRate = 0;
    }
}

void FieldWorker::StartWorkerJob()
{
    if (IsTribe() || IsEmptyField()) {
        //std::cout << "Created: " << FoW;
        worker = new std::thread(&FieldWorker::WorkerJob, this);
    }
}

void FieldWorker::WorkerJob()
{
    std::this_thread::sleep_for((IsTribe() ? std::chrono::milliseconds(10) : std::chrono::milliseconds(20)));

    while (true)
    {
        if (IsTribe())
        {
            {
                std::lock_guard<std::mutex> lg(glMutex, std::adopt_lock);
                GrowthRate();

                if ((*FieldWorker::FoW)[col] >= '9')
                {
                    (*FieldWorker::FoW)[col] = '9';
                    growthRate = 0;
                }
                else if ((*FieldWorker::FoW)[col] >= '1' || (*FieldWorker::FoW)[col] < '9')
                {
                    char cell = (*FieldWorker::FoW)[col];
                    
                    if (char(cell + growthRate) >= '9')
                    {
                        (*FieldWorker::FoW)[col] = '9';
                        growthRate = 0;
                    }
                    else
                    {
                        (*FieldWorker::FoW)[col] = cell + growthRate;
                    }
                }
                else if ((*FieldWorker::FoW)[col] < '1')
                {
                    (*FieldWorker::FoW)[col] = '1' + growthRate;
                }
            }
        }
        else if (IsEmptyField())
        {
            {
                std::lock_guard<std::mutex> lg(glMutex, std::adopt_lock);
                char newVal = '1';
                if(IsRightCellTribe())
                {
                    // Move pointers to correct position
                    // Left | Right neighbour
                    // Up   | Down neighbour
                    char right = GetRightNeighbour(1);

                    if (right != 'X' && 
                       (right > '2'  && right <= '9')) {
                        {
                            std::lock_guard<std::mutex> tranformLock(glMutex, std::adopt_lock);
                            FoW->replace(col, 1, 1, newVal);
                            FoW->replace(col + 1, 1, 1, right - 1);
                            
                            MarkAsTribe();
                            GrowthRate();
                        }
                    }
                }
                if (IsLeftCellTribe())
                {
                    // Move pointers to correct position
                    // Left | Right neighbour
                    // Up   | Down neighbour
                    char left = GetLeftNeighbour(1);
                    
                    if (left != 'X' && 
                       (left > '2' && left <= '9')) {
                        {
                            std::lock_guard<std::mutex> tranformLock(glMutex, std::adopt_lock);
                            FoW->replace(col, 1, 1, newVal);
                            FoW->replace(col - 1, 1, 1, left - 1);
                            
                            MarkAsTribe();
                            GrowthRate();
                        }
                    }
                }
                if (IsUpCellTribe())
                {
                    // Move pointers to correct position
                    // Left | Right neighbour
                    // Up   | Down neighbour
                    char up = GetUpNeighbour(1);
                    
                    if (up != 'X' &&
                       (up > '2' && up <= '9')) {
                        {
                            std::lock_guard<std::mutex> tranformLock(glMutex, std::adopt_lock);
                            FoW->replace(col, 1, 1, newVal);
                            (FoW - 1)->replace(col, 1, 1, up - 1);
                            
                            MarkAsTribe();
                            GrowthRate();
                        }
                    }
                }
                if (IsDownCellTribe())
                {
                    // Move pointers to correct position
                    // Left | Right neighbour
                    // Up   | Down neighbour
                    char down = GetDownNeighbour(1);

                    if (down != 'X' &&
                       (down > '2' && down <= '9')) {
                        {
                            std::lock_guard<std::mutex> tranformLock(glMutex, std::adopt_lock);
                            FoW->replace(col, 1, 1, newVal);
                            (FoW + 1)->replace(col, 1, 1, down - 1);
                            
                            MarkAsTribe();
                            GrowthRate();
                        }
                    }
                }

                if (IsTribe() && (*FieldWorker::FoW)[col] >= '9') {
                    (*FieldWorker::FoW)[col] = '9';
                    growthRate = 0;
                }
            }
            //std::this_thread::yield();
            //std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        else if(IsGroundTile())
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::this_thread::yield();
    }
}

bool FieldWorker::CanMigrate()
{
    if (IsRightCellTribe())
    {
        return true;
    }

    return true;
}

bool FieldWorker::IsRightCellTribe()
{
    return IsNeighbourATribe(NEIGHBOUR::RIGHT);
}

bool FieldWorker::IsLeftCellTribe()
{
    return IsNeighbourATribe(NEIGHBOUR::LEFT);
}

bool FieldWorker::IsUpCellTribe()
{
    return IsNeighbourATribe(NEIGHBOUR::UP);
}

bool FieldWorker::IsDownCellTribe()
{
    return IsNeighbourATribe(NEIGHBOUR::DOWN);
}

bool FieldWorker::IsNeighbourATribe(int side)
{
    char neighbour;
    if ((col == FieldWorker::size - 1) ||
        (col == 0) ||
        (row == 0) ||
        (row == FieldWorker::rSize - 1)) {
        return false;
    }

    switch (side) {
        case NEIGHBOUR::RIGHT: {
            neighbour = GetRightNeighbour(1);
            break;
        }
        case NEIGHBOUR::LEFT: {
            neighbour = GetLeftNeighbour(1); 
            break;
        }
        case NEIGHBOUR::UP: {
            neighbour = GetUpNeighbour(1);
            break;
        }
        case NEIGHBOUR::DOWN: {
            neighbour = GetDownNeighbour(1); 
            break;
        }
    }

    if (neighbour == 'F' ||
        neighbour == 'W' ||
        neighbour == 'M' ||
        neighbour == '.')
    {
        return false;
    }

    return neighbour > '1' || neighbour <= '9';
}

void FieldWorker::GrowthRate()
{
    if((IsForest(1) || IsForest(2)) &&
        (IsWater(1) || IsWater(2)))
    {
        growthRate = 2;
    }
    else if ((IsForest(1) || IsForest(2)) ||
             (IsWater(1) || IsWater(2)))
    {
        growthRate = 1;
    }
    else
    {
        growthRate = 0;
    }
}

bool FieldWorker::IsForest(int pos)
{
    return CheckCells(pos, 'F');
}

bool FieldWorker::IsWater(int pos)
{
    return CheckCells(pos, 'W');
}

bool FieldWorker::IsMountain(int pos)
{
    return CheckCells(pos, 'M');
}

bool FieldWorker::IsNeighbourEmptyField(int pos)
{
    return CheckCells(pos, '.');
}

bool FieldWorker::CheckCells(int pos, char type)
{
    char left,right, up, down;
    // Move pointers to correct position
    // Left | Right neighbour
    // Up   | Down neighbour
    right = GetRightNeighbour(pos);
    left = GetLeftNeighbour(pos);
    up = GetUpNeighbour(pos);
    down = GetDownNeighbour(pos);

    if (row == 0 && col == 0)
    {
        return ((right == type) || (down == type));
    }
    else if (row == 0 && col == FieldWorker::size - 1)
    {
        return ((left == type) || (down == type));
    }
    else if (row == FieldWorker::rSize - 1 && col == 0)
    {
        return ((right == type) || (up == type));
    }
    else if (row == FieldWorker::rSize - 1 && col == FieldWorker::size - 1)
    {
        return ((left == type) || (up == type));
    }
    
    if (row == 0)
    {
        return ((left == type) || (right == type) || (down == type));
    }
    else if (row == FieldWorker::rSize - 1)
    {
        return ((left == type) || (right == type) || (up == type));
    }
    
    if (col == 0)
    {
        return ((right == type) || (up == type) || (down == type));
    }
    else if (col == FieldWorker::size - 1)
    {
        return ((left == type) || (up == type) || (down == type));
    }

    return (left == type  || 
            right == type ||
            up == type    || 
            down == type);
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