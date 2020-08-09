#ifndef __FIELDWORKER_H__
#define __FIELDWORKER_H__

#include <thread>
#include <mutex>
#include <iostream>
#include <memory>

class FieldWorker
{
private:
    static std::string* FoW;
    
    static int size;
    static int rSize;
    static int row, col;

    static unsigned int growthRate;
    
    static std::mutex glMutex;
    
    static bool isTribe;
    static bool isGroundTile;
    static bool isEmptyField;

    std::thread* worker;
    // Add team separation implementation

protected:
    static void GrowthRate();
    static bool IsForest(int);
    static bool IsWater(int);
    static bool IsMountain(int);
    static bool IsNeighbourEmptyField(int);
    
    static bool CanMigrate();
    
    static bool CheckCells(int,char);
    
    static bool IsRightCellTribe();
    static bool IsLeftCellTribe();
    static bool IsUpCellTribe();
    static bool IsDownCellTribe();
public:
    inline FieldWorker() {};
    inline FieldWorker(const FieldWorker&) {};
    FieldWorker(std::string*, int, int, int);
    ~FieldWorker();

    static void WorkerJob();
    void WorkerJobFinish();

    inline static void MarkAsTribe() { FieldWorker::isTribe = true; }
    inline static bool IsTribe() { return FieldWorker::isTribe; }
    inline static void MarkAsGroundTile() { FieldWorker::isGroundTile = true; }
    inline static bool IsGroundTile() { return FieldWorker::isGroundTile; }
    inline static void MarkAsEmptyField() { FieldWorker::isEmptyField = true; }
    inline static bool IsEmptyField() { return FieldWorker::isEmptyField; }
    inline static std::string* FOW() { return &FoW[col]; }
    inline static void FOW(char c) { FoW[col] = c; }

    inline int GetRow() { return row; }
    inline int GetColumn() { return col; }

    inline void AssignFOW(std::string* fow) { FoW = fow; }
};

#endif