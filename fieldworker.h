#ifndef __FIELDWORKER_H__
#define __FIELDWORKER_H__

#include <thread>
#include <mutex>
#include <iostream>
#include <memory>

class FieldWorker
{
private:
    std::string* FoW;
    char* upFoW;
    char* downFoW;
    
    int size;
    int rSize;
    int row, col;

    unsigned int growthRate;
    
    std::mutex glMutex;
    
    bool isTribe;
    bool isGroundTile;
    bool isEmptyField;

    std::thread* worker;
    // Add team separation implementation

    enum NEIGHBOUR
    {
        RIGHT = 1,
        LEFT  = 2,
        UP    = 3,
        DOWN  = 4
    };

    protected : void
    GrowthRate();
    bool IsForest(int);
    bool IsWater(int);
    bool IsMountain(int);
    bool IsNeighbourEmptyField(int);
    
    bool CanMigrate();
    
    bool CheckCells(int,char);
    
    bool IsRightCellTribe();
    bool IsLeftCellTribe();
    bool IsUpCellTribe();
    bool IsDownCellTribe();
public:
    inline FieldWorker() {};
    inline FieldWorker(const FieldWorker&) {};
    FieldWorker(std::string*, char*, char*, int, int, int);
    ~FieldWorker();

    void StartWorkerJob();
    void WorkerJob();
    void WorkerJobFinish();

    inline void MarkAsTribe() { isTribe = true; isGroundTile = false; isEmptyField = false; }
    inline bool IsTribe() { return isTribe; }
    inline void MarkAsGroundTile() { isGroundTile = true; isTribe = false; isEmptyField = false; }
    inline bool IsGroundTile() { return isGroundTile; }
    inline void MarkAsEmptyField() { isEmptyField = true; isGroundTile = false; isTribe = false; }
    inline bool IsEmptyField() { return isEmptyField; }
    inline std::string* FOW() { return FoW; }
    inline void FOW(char c) { FoW[col] = c; }

    bool IsNeighbourATribe(int);
    inline char GetRightNeighbour(int pos) { return (col + pos < size) ? (*FoW)[col + pos] : 'Z'; }
    inline char GetLeftNeighbour(int pos) { return (col - pos >= 0) ? (*FoW)[col - pos] : 'Z'; }
    inline char GetUpNeighbour(int pos) { std::string* tmp = FoW; return (row - pos >= 0) ? (*(tmp - pos)).at(col) : 'Z'; }
    inline char GetDownNeighbour(int pos) { std::string* tmp = FoW; return (row + pos < rSize) ? (*(tmp + pos)).at(col) : 'Z'; }

    inline int GetRow() { return row; }
    inline int GetColumn() { return col; }

    inline void AssignFOW(std::string* fow) { FoW = fow; }
};

#endif