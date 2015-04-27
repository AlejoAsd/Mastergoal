#ifndef PLAYER_PIECE_H
#define PLAYER_PIECE_H

#include "Player.h"
#include "Piece.h"
#include "Square.h"

namespace Common
{
	class PlayerPiece : public Player, public Piece
	{
	public:
		/** Constructores **/
		PlayerPiece(ORIENTATION orientation, PLAYER_ID id, Square position);

		PlayerPiece(ORIENTATION orientation, PLAYER_ID id, int row, int col);

		PlayerPiece(const PlayerPiece& playerPiece);

		~PlayerPiece() {}

		/** Métodos **/
		// Retorna true si son iguales los id y orientation
		bool SamePlayer(const PlayerPiece& playerPiece) const;

	private:
		PlayerPiece() : Piece() {}
	};
}

#endif
