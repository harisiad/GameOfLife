#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <fstream>

#include "display.h"

void Foo(int x)
{
	std::cout << "finished " << x << '\n';
}

int main()
{
	// Terminal currTerm = Terminal();
	// FieldWorkers pField("field1");
	Display dis("field1");

	dis.Run();

	// char** field = (char**) malloc(sizeof(char*) * input.RowCount());
	// int r = 0;

	// field[r] = (char*)malloc(sizeof(char) * input.ColumnCount());
	// while (input.GetLine(field[r]))
	// {
	// 	r++;
	// 	field[r] = (char*)malloc(sizeof(char) * input.ColumnCount());
	// }
	// pField.AssignWorkers();
	// while (true)
	// {
	// 	currTerm.ClearTerminal();
	// 	for (unsigned int row = 0; row <= pField.GetRows(); row++)
	// 	{
	// 		for (unsigned int col = 0; col <= pField.GetColumns(); col++)
	// 		{
	// 			if (row == pField.UpperFrame() || row == pField.BottomFrame())
	// 			{
	// 				currTerm.AddChar('*');
	// 				if (col == pField.RightFrame())
	// 				{
	// 					currTerm.AddChar('*');
	// 					currTerm.AddChar('\n');
	// 				}
	// 			}
	// 			else
	// 			{
	// 				if (col == pField.LeftFrame() || col == pField.RightFrame())
	// 				{
	// 					currTerm.AddChar('*');
	// 					if (col == pField.RightFrame())
	// 					{
	// 						currTerm.AddChar('\n');
	// 					}
	// 					else
	// 					{
	// 						currTerm.AddChar(pField.GetCell(row,col));
	// 					}
	// 				}
	// 				else
	// 				{
	// 					currTerm.AddChar(pField.GetCell(row,col));
	// 				}
	// 			}
	// 		}
	// 	}
	// 	currTerm.RefreshRateTerminal(50);
	// }
	// pField.WaitJobsFinish();

	return 0;
}