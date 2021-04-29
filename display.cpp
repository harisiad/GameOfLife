#include "display.h"

FieldWorkers* Display::field;
Terminal Display::currTerm;
std::mutex Display::glMutex;

Display::Display(std::string filename)
{
    currTerm = Terminal();
    field = new FieldWorkers(filename);
    displayThrd = new std::thread(&Display::DisplayJob);
}

Display::~Display()
{
    displayThrd->join();
    field->WaitJobsFinish();
}

void Display::Run()
{
    field->AssignWorkers();
}

void Display::DisplayJob()
{
    while (true)
	{
		Display::currTerm.ClearTerminal();
		char* bound = "*"; char* endl = "\n";
		for (unsigned int row = 0; row <= Display::field->GetRows(); row++)
		{
			for (unsigned int col = 0; col <= Display::field->GetColumns(); col++)
			{
				if (row == field->UpperFrame() || row == field->BottomFrame())
				{
					Display::currTerm.AddChar(bound);
					if (col == field->RightFrame())
					{
						Display::currTerm.AddChar(bound);
						Display::currTerm.AddChar(endl);
					}
				}
				else
				{
					if (col == field->LeftFrame() || col == field->RightFrame())
					{
						Display::currTerm.AddChar(bound);
						if (col == field->RightFrame())
						{
							Display::currTerm.AddChar(endl);
						}
						else
						{
							{
								std::lock_guard<std::mutex> lg(glMutex);
								Display::currTerm.AddChar(Display::field->GetCell(row,col));
							}
						}
					}
					else
					{
						{
							std::lock_guard<std::mutex> lg(glMutex);
							Display::currTerm.AddChar(Display::field->GetCell(row,col));
						}
					}
				}
			}
		}
		currTerm.RefreshRateTerminal(500);
	}
}