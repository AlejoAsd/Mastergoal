#include "stdafx.h"
#include "Square.h"
#include <xstring>

namespace Common
{
	Square::Square()
	{
		this->row = -1;
		this->col = -1;
	}

	Square::Square(int row, int col)
	{
		this->row = row;
		this->col = col;
	}

	Square::Square(const std::basic_string<char> squareStr)
	{
		StringToSquare(squareStr);
	}

	Square::Square(const Square& square)
	{
		this->row = square.row;
		this->col = square.col;
	}

	void Square::SetPosition(int row, int col)
	{
		this->row = row;
		this->col = col;
	}

	void Square::StringToRow(const std::basic_string<char> rowStr)
	{
		if (rowStr.compare("6") == 0)
            this->row = 1;
        else if (rowStr.compare("5") == 0)
            this->row = 2;
        else if (rowStr.compare("4") == 0)
            this->row = 3;            
        else if (rowStr.compare("3") == 0)
            this->row = 4;
        else if (rowStr.compare("2") == 0)
            this->row = 5;
        else if (rowStr.compare("1") == 0)
            this->row = 6;
        else if (rowStr.compare("0") == 0)
            this->row = 7;
        else if (rowStr.compare("1.") == 0)
            this->row = 8;
        else if (rowStr.compare("2.") == 0)
            this->row = 9;
        else if (rowStr.compare("3.") == 0)
            this->row = 10;
        else if (rowStr.compare("4.") == 0)
            this->row = 11;
        else if (rowStr.compare("5.") == 0)
            this->row = 12;
        else if (rowStr.compare("6.") == 0)
            this->row = 13;
	}

	void Square::StringToCol(const std::basic_string<char> colStr)
	{
		if (colStr.compare(" ") == 0 || colStr.compare("") == 0 || colStr.compare("_") == 0)
        {
            this->col = 5;
        }
        else if (colStr.compare("0") == 0)
		{
			this->col = 0;
		}
		else if (colStr.compare("1") == 0)
		{
			this->col = 1;
		}
		else if (colStr.compare("2") == 0)
		{
			this->col = 2;
		}
		else if (colStr.compare("3") == 0)
		{
			this->col = 3;
		}
		else if (colStr.compare("4") == 0)
		{
			this->col = 4;
		}
		else if (colStr.compare("5") == 0)
		{
			this->col = 6;
		}
		else if (colStr.compare("6") == 0)
		{
			this->col = 7;
		}
		else if (colStr.compare("7") == 0)
		{
			this->col = 8;
		}
		else if (colStr.compare("8") == 0)
		{
			this->col = 9;
		}
		else if (colStr.compare("9") == 0)
		{
			this->col = 10;
		}
	}

	void Square::StringToSquare(const std::basic_string<char> squareStr)
	{
		std::basic_string<char> colStr = squareStr.substr(0, 1);
		std::basic_string<char> rowStr = squareStr.substr(1, squareStr.length() - 1);
		
		StringToCol(colStr);
		StringToRow(rowStr);
	}
}
