#ifndef __PLAYGROUNDFIELD_H_
#define __PLAYGROUNDFIELD_H_

#include "userInput.h"

#include <vector>

class PlaygroundField
{
private:
    UserInput rawData;

    static std::vector<std::string> field;

    unsigned int UPPER_FRAME_ROW = 0;
    unsigned int BOTTOM_FRAME_ROW;

    unsigned int LEFT_FRAME_ROW = 0;
    unsigned int RIGHT_FRAME_ROW;

    void CreatePlaygroundField();

public:
    PlaygroundField(std::string);
    ~PlaygroundField();

    inline unsigned int UpperFrame() { return UPPER_FRAME_ROW; }
    inline unsigned int BottomFrame() { return BOTTOM_FRAME_ROW; }
    inline unsigned int LeftFrame() { return LEFT_FRAME_ROW; }
    inline unsigned int RightFrame() { return RIGHT_FRAME_ROW; }

    inline unsigned int GetRows() { return rawData.RowCount(); }
    inline unsigned int GetColumns() { return rawData.ColumnCount(); }

    inline static std::vector<std::string> GetField() { return field; }

    inline static char GetCell(int row, int col) { return PlaygroundField::field[row][col]; }
    inline static std::string* GetC(int row, int col) { return &PlaygroundField::field[row]; }
    bool IsTribeMark(int ,int);
};

#endif