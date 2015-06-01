#ifndef BALL_PIECE_H
#define BALL_PIECE_H

#include "Piece.h"

namespace Common
{
	class BallPiece : public Piece
	{
	public:
		/** Constructores **/
		BallPiece(const Square& position) : Piece(TYPE_BALL, position) {}

		BallPiece(int row, int col) : Piece(TYPE_BALL, row, col) {}

		BallPiece(const BallPiece& ballPiece)
		{
			this->type = TYPE_BALL;
			this->position = new Square(*ballPiece.position);
		}

		~BallPiece() { }

	private:
		BallPiece() : Piece() {}
	};
}

#endif
