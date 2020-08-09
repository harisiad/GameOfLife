#include "userInput.h"

UserInput::UserInput()
{
    
}

UserInput::UserInput(std::string filename) :
    sourceFileName(filename)
{
    
}

void UserInput::PrepareData(std::string filename)
{
    sourceFile = std::fstream(filename);
    
    if (sourceFile.fail() || sourceFile.bad())
    {
        throw "Fail or Bad bit set when opened scenario file.";
    }
    
    Exceptions();

    CountColumns();
    CountRows();
}

UserInput::~UserInput()
{

}

void UserInput::Exceptions()
{
    sourceFile.exceptions(std::ios_base::badbit);
}

void UserInput::CountColumns()
{
	char c;
	int col_num = 0;
	while ( (c = sourceFile.get()) )    // extra parenthesis: G++ 9.3.0 compiler suggestion
	{
		if (c == '\n') break;

		col_num++;
	}
	sourceFile.clear(); sourceFile.seekg(0, std::ios_base::beg);

    columnCount = col_num;
}

void UserInput::CountRows()
{
    std::string str;
	int row_num = 0;
    //str->reserve(ColumnCount());
	while (GetLine(str))
	{
		if (sourceFile.eof()) break;

		row_num++;
	}

	sourceFile.clear(); sourceFile.seekg(0, std::ios_base::beg);

    rowCount = row_num;
}

std::istream& UserInput::GetLine(std::string& out)
{
    return std::getline(sourceFile ,out);
}