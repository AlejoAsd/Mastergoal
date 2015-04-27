#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Pieces.h"
#include "Square.h"

namespace Common
{
	class Movement
	{
	public:
		virtual ~Movement();

		const Square* GetSquareAfter() const { return this->squareAfter; }

		const Square* GetSquareBefore() const { return this->piece->GetPosition(); }

		void SetSquareAfter(int row, int col);

		// Setea squareAfter a la posición indicada por position
		void SetSquareAfter(const Square& position);

		void SetSquareBefore(int row, int col);

		// Setea squareBefore a la posición indicada por position
		void SetSquareBefore(const Square& position);

		TYPEPIECE GetMovementType() const { return this->typeMovement; }

		//void SetPieceTypeToBall();

		//void SetPieceTypeToPlayer(ORIENTATION orientation, PLAYER_ID id);

		//virtual const Piece* GetPiece() const { return this->piece; }

	protected:
		Piece* piece;

		Square* squareAfter;

		TYPEPIECE typeMovement;

		// Constructores privado
		Movement() {}

		// Hace una copia de BallPiece
		Movement(const BallPiece& piece, int rowAfter, int colAfter);
		
		// Hace una copia de PlayerPiece
		Movement(const PlayerPiece& piece, int rowAfter, int colAfter);

		//Movement(const Movement& movement);
	};
}

#endif
