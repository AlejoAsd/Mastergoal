#ifndef BOARD_H
#define BOARD_H

#include "Pieces.h"
#include "Movements.h"
#include "Play.h"

namespace Common
{
	class Board
	{
	public:
		// Constructores
		//Board(int level, BallPiece* ball, PlayerPiece** leftPlayers, PlayerPiece** rightPlayers);

		Board(int level);

		Board(const Board& board);

		~Board();

		// Getters y Setters
		inline const BallPiece* GetBall() const { return this->ball; }

		const PlayerPiece* GetPlayer(const ORIENTATION& orientation, PLAYER_ID id) const;

		void SetBallPosition(int row, int col);

		void SetPlayerPosition(const ORIENTATION& orientation, PLAYER_ID id, int row, int col);

		void SetBallPosition(const Square& position);

		void SetPlayerPosition(const ORIENTATION& orientation, PLAYER_ID id, const Square& position);

		void SetPiecePosition(const Piece* piece, int row, int col);

		// Retorna la cantidad de fichas por cada equipo
		inline int GetNPieces() const { return nPieces; }

		inline PlayerPiece** GetLeftPieces() const { return this->leftPieces; }

		inline PlayerPiece** GetRightPieces() const { return this->rightPieces; }

		/** Métodos **/
		// Copia el board, el usuario se hace responsable de liberar la memoria al finalizar
		Board* Clone();

		bool Equals(const Board& board) const;

		// Retorna true si la casilla está vacía
		inline bool IsEmpty(int row, int col) const
		{
			if (this->ball->GetRow() == row && this->ball->GetCol() == col)
				return false;

			for (int i = 0; i < this->nPieces; i++)
			{
				if (this->leftPieces[i]->GetRow() == row && this->leftPieces[i]->GetCol() == col)
					return false;
			}

			for (int i = 0; i < this->nPieces; i++)
			{
				if (this->rightPieces[i]->GetRow() == row && this->rightPieces[i]->GetCol() == col)
					return false;
			}
			return true;
		}

	private:
		// Número de piezas en cada equipo
		int nPieces;

		BallPiece* ball;

		PlayerPiece** leftPieces; // White

		PlayerPiece** rightPieces; // Red

		// Constructor privado
		Board() {}

		void Initialize();
	};
}

#endif
