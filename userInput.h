#ifndef __USERINPUT_H_
#define __USERINPUT_H_

#include <iostream>
#include <fstream>
#include <string>

class UserInput
{
private:
    std::string sourceFileName;
    std::fstream sourceFile;

    unsigned int columnCount;
    unsigned int rowCount;

    const std::streamsize MAX_BUFFER_SIZE = 200;
    
    void Exceptions();

public:
    UserInput();
    UserInput(std::string);
    ~UserInput();

    UserInput(UserInput&)=delete;
    UserInput(UserInput&&)=delete;
    UserInput operator=(const UserInput&)=delete;

    void PrepareData(std::string);

    inline unsigned int ColumnCount() { return columnCount; }
    inline unsigned int RowCount() { return rowCount; }

    void CountColumns();
    void CountRows();

    std::istream& GetLine(std::string&);
    
};

#endif