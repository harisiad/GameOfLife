#ifndef __PLAYGROUNDFIELD_H_
#define __PLAYGROUNDFIELD_H_

#include "userInput.h"

class PlaygroundField
{
private:
    UserInput rawData;

    static char** field;

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

    inline static char** GetField() { return field; }

    inline static char GetCell(int row, int col) { return PlaygroundField::field[row][col]; }
    bool IsTribeMark(int ,int);
};

#endif