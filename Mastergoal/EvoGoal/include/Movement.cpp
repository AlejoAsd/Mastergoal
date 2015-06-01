#include "stdafx.h"
#include "Movement.h"
#include "Pieces.h"
#include "Square.h"

namespace Common
{
	Movement::Movement(const Common::BallPiece &ball, int rowAfter, int colAfter)
	{
		this->piece = new BallPiece(ball);
		this->squareAfter = new Square(rowAfter, colAfter);
	}

	Movement::Movement(const Common::PlayerPiece &player, int rowAfter, int colAfter)
	{
		this->piece = new PlayerPiece(player);
		this->squareAfter = new Square(rowAfter, colAfter);
	}

	//Movement::Movement(const Movement& movement)
	//{
	//	// TODO: controlar si copia bien
	//	if (movement.piece->GetType() == TYPE_BALL)
	//	{
	//		this->piece = new BallPiece(*((BallPiece*)movement.piece));
	//	}
	//	else // movement.piece->GetType() == TYPE_PLAYER
	//	{
	//		this->piece = new PlayerPiece(*((PlayerPiece*)movement.piece));
	//	}
	//	this->squareAfter = new Square(movement.squareAfter->GetRow(), movement.squareAfter->GetCol());
	//}

	Movement::~Movement()
	{
		// TODO: controlar si le llama al destructor de Piece
		// o de su subclase
		if (piece)
		{
			delete piece;
			piece = NULL;
		}

		if (squareAfter)
		{
			delete squareAfter;
			squareAfter = NULL;
		}
	}

	void Movement::SetSquareAfter(int row, int col)
	{
		this->squareAfter->SetRow(row);
		this->squareAfter->SetCol(col);
	}

	void Movement::SetSquareAfter(const Square& square)
	{
		this->squareAfter->SetRow(square.GetRow());
		this->squareAfter->SetCol(square.GetCol());
	}

	void Movement::SetSquareBefore(int row, int col)
	{
		this->piece->SetPosition(row, col);
	}

	void Movement::SetSquareBefore(const Square& square)
	{
		this->piece->SetPosition(square);
	}

	//void Movement::SetPieceTypeToBall()
	//{
	//	if (this->piece->GetType() == TYPE_PLAYER)
	//	{
	//		int row = this->piece->GetRow();
	//		int col = this->piece->GetCol();
	//		delete this->piece;
	//		this->piece = new BallPiece(row, col);
	//	}
	//}

	//void Movement::SetPieceTypeToPlayer(ORIENTATION orientation, PLAYER_ID id)
	//{
	//	if (this->piece->GetType() == TYPE_BALL)
	//	{
	//		int row = this->piece->GetRow();
	//		int col = this->piece->GetCol();
	//		delete this->piece;
	//		this->piece = new PlayerPiece(orientation, id, row, col);
	//	}
	//	else // this->piece->GetType() == TYPE_PLAYER
	//	{
	//		PlayerPiece* playerPiece = ((PlayerPiece*)this->piece);
	//		playerPiece->SetOrientation(orientation);
	//		playerPiece->SetId(id);
	//	}
	//}
}
