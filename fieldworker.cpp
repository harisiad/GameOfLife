#include "fieldworker.h"
#include "display.h"

#include <curses.h>
#include <cstring>

extern Display dis;

std::string* FieldWorker::FoW;
int FieldWorker::size;
int FieldWorker::rSize;
bool FieldWorker::isTribe {false};
bool FieldWorker::isGroundTile {false};
bool FieldWorker::isEmptyField {false};
std::mutex FieldWorker::glMutex;
unsigned int FieldWorker::growthRate;
int FieldWorker::row, FieldWorker::col;

FieldWorker::FieldWorker(std::string* data, int r, int c, int rows)
{
    row = r; col = c; FieldWorker::size = data->length();
    FieldWorker::rSize = rows;

    FieldWorker::FoW = (std::string*)malloc(sizeof(std::string));

    //std::memcpy(FieldWorker::FoW, data, size);

    FieldWorker::FoW = data;
    //FieldWorker::FoW += FieldWorker::col;
    if ((*FieldWorker::FoW)[col] == 'X')
    {
        (*FieldWorker::FoW)[col] = '5';
        MarkAsTribe();
        growthRate = 1;
    }
    else if ((*FieldWorker::FoW)[col] == 'F' ||
            (*FieldWorker::FoW)[col] == 'W' ||
            (*FieldWorker::FoW)[col] == 'M')
    {
        MarkAsGroundTile();
        growthRate = 0;
    }
    else if ((*FieldWorker::FoW)[col] == '.')
    {
        MarkAsEmptyField();
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
            {
                std::lock_guard<std::mutex> lg(glMutex);
                GrowthRate();

                if ((*FieldWorker::FoW)[col] >= '9' && (*FieldWorker::FoW)[col] < 'A')
                {
                    (*FieldWorker::FoW)[col] = '9';
                    growthRate = 0;
                }
                else if ((*FieldWorker::FoW)[col] < '9')
                {
                    char cell = (*FieldWorker::FoW)[col];
                    
                    if (char(cell + growthRate) >= '9')
                    {
                        (*FieldWorker::FoW)[col] = char('9');
                    }
                    else
                    {
                        (*FieldWorker::FoW)[col] = char(cell + growthRate);
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        else if (IsEmptyField())
        {
            {
                std::lock_guard<std::mutex> lg(glMutex);
                char newVal = '1';
                if(IsRightCellTribe())
                {
                    char right;

                    // Move pointers to correct position
                    // Left | Right neighbour
                    // Up   | Down neighbour
                    right = (*FieldWorker::FoW)[col+1];

                    if (char(right - 1) <= '1') { continue; }

                    (*FieldWorker::FoW)[col+1] = char(right - 1);
                    (*FieldWorker::FoW)[col] = newVal;

                    MarkAsTribe();
                    FieldWorker::isEmptyField = false;
                }
                
                if (IsLeftCellTribe())
                {
                    char left;

                    // Move pointers to correct position
                    // Left | Right neighbour
                    // Up   | Down neighbour
                    left = (*FieldWorker::FoW)[col-1];
                    
                    if (char(left - 1) <= '1') { continue; }
                    
                    (*FieldWorker::FoW)[col-1] = char(left - 1);
                    (*FieldWorker::FoW)[col] = newVal;
                    
                    MarkAsTribe();
                    FieldWorker::isEmptyField = false;
                }
                
                if (IsUpCellTribe())
                {
                    std::string* tmp;
                    char up;

                    // Move pointers to correct position
                    // Left | Right neighbour
                    // Up   | Down neighbour
                    tmp = FieldWorker::FoW;

                    up = (*(tmp - 1))[col];
                    
                    if (char(up - 1) <= '1') { continue; }

                    (*(FieldWorker::FoW - 1))[col] = char(up - 1);
                    (*FieldWorker::FoW)[col] = newVal;
                    MarkAsTribe();
                    FieldWorker::isEmptyField = false;

                    delete tmp;
                }
                
                if (IsDownCellTribe())
                {
                    std::string* tmp;
                    char down;

                    // Move pointers to correct position
                    // Left | Right neighbour
                    // Up   | Down neighbour
                    tmp = FieldWorker::FoW;

                    down = (*(tmp + 1))[col];
                    if (char(down - 1) <= '1') { continue; }

                    (*FieldWorker::FoW)[col] = newVal;
                    MarkAsTribe();
                    FieldWorker::isEmptyField = false;

                    (*(FieldWorker::FoW + 1))[col] = char(down - 1);
                    delete tmp;
                }
            }
            //glMutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        else if(IsGroundTile())
        {
            break;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        
        std::this_thread::yield();
    }
    std::cout << "Thread with field: " << (*FieldWorker::FoW)[col] << " completed\n";
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
    if (col == FieldWorker::size - 1)
    {
        return false;
    }

    if ((*FieldWorker::FoW)[col + 1] == 'F' ||
        (*FieldWorker::FoW)[col + 1] == 'W' ||
        (*FieldWorker::FoW)[col + 1] == 'M' ||
        (*FieldWorker::FoW)[col + 1] == '.')
    {
        return false;
    }

    return (*FieldWorker::FoW)[col + 1] > '1';
}

bool FieldWorker::IsLeftCellTribe()
{
    if (col == 0)
    {
        return false;
    }
    
    if ((*FieldWorker::FoW)[col - 1] == 'F' ||
        (*FieldWorker::FoW)[col - 1] == 'W' ||
        (*FieldWorker::FoW)[col - 1] == 'M' ||
        (*FieldWorker::FoW)[col - 1] == '.')
    {
        return false;
    }

    return (*FieldWorker::FoW)[col - 1] > '1';
}

bool FieldWorker::IsUpCellTribe()
{
    std::string* tmp(FieldWorker::FoW);
    
    if (row == 0)
    {
        return false;
    }

    if ((*tmp)[col] == 'F' ||
        (*tmp)[col] == 'W' ||
        (*tmp)[col] == 'M' ||
        (*tmp)[col] == '.')
    {
        return false;
    }

    return (*(tmp - 1))[col] > '1';
}

bool FieldWorker::IsDownCellTribe()
{
    std::string* tmp(FieldWorker::FoW);

    if (row == FieldWorker::rSize - 1)
    {
        return false;
    }
    
    if ((*tmp)[col] == 'F' ||
        (*tmp)[col] == 'W' ||
        (*tmp)[col] == 'M' ||
        (*tmp)[col] == '.')
    {
        return false;
    }
    
    return (*(tmp + 1))[col] > '1';
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
    std::string* tmpU = FieldWorker::FoW, *tmpB = FieldWorker::FoW; 
    // Move pointers to correct position
    // Left | Right neighbour
    // Up   | Down neighbour
    right = (col + pos < FieldWorker::size - 1) ? (*FieldWorker::FoW)[col + pos] : 'Z';
    left = (col - pos > 0) ? (*FieldWorker::FoW)[col - pos] : 'Z';
    up = (row - pos > 0) ? (*(tmpU - pos))[col] : 'Z';
    down = (row + pos < FieldWorker::rSize - 1) ? (*(tmpB + pos))[col] : 'Z';

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

    return (left == type || right == type ||
              up == type || down == type);
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