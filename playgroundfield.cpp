#include "playgroundfield.h"

char** PlaygroundField::field = {};

void PlaygroundField::CreatePlaygroundField()
{
    PlaygroundField::field = (char**) malloc(sizeof(char*) * rawData.RowCount());
	int row = 0;

	PlaygroundField::field[row] = (char*)malloc(sizeof(char) * rawData.ColumnCount());
	while (rawData.GetLine(PlaygroundField::field[row]))
	{
		row++;
		PlaygroundField::field[row] = (char*)malloc(sizeof(char) * rawData.ColumnCount());
	}
}

bool PlaygroundField::IsTribeMark(int row, int col)
{
    return GetCell(row, col) == 'X';
}

PlaygroundField::PlaygroundField(std::string sourceFile)
{
    rawData.PrepareData(sourceFile);
    BOTTOM_FRAME_ROW = rawData.RowCount();
    RIGHT_FRAME_ROW = rawData.ColumnCount();

    CreatePlaygroundField();
}

PlaygroundField::~PlaygroundField()
{
    for (unsigned int i = 0; i < rawData.RowCount(); i++)
    {
        delete field[i];
    }

    delete field;
}