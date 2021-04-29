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
    char* upCell;
    char* downCell;
    
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

    inline void MarkAsTribe() { FieldWorker::isTribe = true; FieldWorker::isGroundTile = false; FieldWorker::isEmptyField = false; }
    inline bool IsTribe() { return FieldWorker::isTribe; }
    inline void MarkAsGroundTile() { FieldWorker::isGroundTile = true; FieldWorker::isTribe = false; FieldWorker::isEmptyField = false; }
    inline bool IsGroundTile() { return FieldWorker::isGroundTile; }
    inline void MarkAsEmptyField() { FieldWorker::isEmptyField = true; FieldWorker::isGroundTile = false; FieldWorker::isTribe = false; }
    inline bool IsEmptyField() { return FieldWorker::isEmptyField; }
    inline std::string* FOW() { return &FoW[col]; }
    inline void FOW(char c) { FoW[col] = c; }

    bool IsNeighbourATribe(int);
    inline char GetRightNeighbour(int pos) { return (col + pos < FieldWorker::size) ? (*FieldWorker::FoW)[col + pos] : 'Z'; }
    inline char GetLeftNeighbour(int pos) { return (col - pos >= 0) ? (*FieldWorker::FoW)[col - pos] : 'Z'; }
    inline char GetUpNeighbour(int pos) { std::string* tmp = FieldWorker::FoW; return (row - pos >= 0) ? (*(tmp - pos)).at(pos) : 'Z'; }
    inline char GetDownNeighbour(int pos) { std::string* tmp = FieldWorker::FoW; return (row + pos < FieldWorker::rSize) ? (*(tmp + pos)).at(pos) : 'Z'; }

    inline int GetRow() { return row; }
    inline int GetColumn() { return col; }

    inline void AssignFOW(std::string* fow) { FoW = fow; }
};

#endif