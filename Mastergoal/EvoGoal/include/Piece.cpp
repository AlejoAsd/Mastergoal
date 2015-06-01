#include "stdafx.h"
#include "Piece.h"
#include "Square.h"

namespace Common
{
	Piece::Piece(TYPEPIECE type, const Square& position)
	{
		this->type = type;
		this->position = new Square(position);
	}

	Piece::Piece(TYPEPIECE type, int row, int col)
	{
		this->type = type;
		this->position = new Square(row, col);
	}

	Piece::~Piece()
	{
		if (position)
		{
			delete position;
			position = NULL;
		}
	}

	void Piece::SetPosition(const Square& position)
	{ 
		this->position->SetRow(position.GetRow());
		this->position->SetCol(position.GetCol()); 
	}

	void Piece::SetPosition(int row, int col)
	{
		this->position->SetRow(row);
		this->position->SetCol(col);
	}
}
