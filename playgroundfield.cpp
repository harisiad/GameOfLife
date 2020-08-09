#include "playgroundfield.h"

std::vector<std::string> PlaygroundField::field;

void PlaygroundField::CreatePlaygroundField()
{
    PlaygroundField::field.reserve(rawData.RowCount());
    PlaygroundField::field = std::initializer_list<std::string>();
	int row = 0;

	PlaygroundField::field.push_back(std::string());
    PlaygroundField::field[row].reserve(rawData.ColumnCount());

	while (rawData.GetLine(PlaygroundField::field[row]))
	{
		row++;
    	PlaygroundField::field.push_back(std::string());
		PlaygroundField::field[row].reserve(rawData.ColumnCount());
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
}