#ifndef __FIELDWORKER_H__
#define __FIELDWORKER_H__

#include <thread>
#include <mutex>
#include <iostream>
#include <memory>

class FieldWorker
{
private:
    static char* FoW;
    static int size;
    static bool isTribe;
    static std::mutex glMutex;
    static unsigned int growthRate;

    std::thread* worker;
    int row, col;
    // Add team separation implementation

protected:
    static void GrowthRate();
    static bool IsForest(int);
    static bool IsWater(int);
    static bool IsMountain(int);
    static bool IsNeighbourEmptyField(int);
    static bool CanMigrate();
public:
    inline FieldWorker() {};
    inline FieldWorker(const FieldWorker&) {};
    FieldWorker(char*, int, int, int);
    ~FieldWorker();

    static void WorkerJob();
    void WorkerJobFinish();

    inline static void MarkAsTribe() { isTribe = true; }
    inline static bool IsTribe() { return isTribe; }
    inline static bool IsEmptyField() { return (int)(*FoW) == 0; }
    
    inline int GetRow() { return row; }
    inline int GetColumn() { return col; }

    inline void AssignFOW(char* fow) { FoW = fow; }
    inline char* FOW() { return FoW; }
};

#endif