#include "stdafx.h"
#include "PlayerPiece.h"

namespace Common
{
	PlayerPiece::PlayerPiece(ORIENTATION orientation, PLAYER_ID id, Square position) : Player(orientation, id), Piece(TYPE_PLAYER, position)
	{
	}

	PlayerPiece::PlayerPiece(ORIENTATION orientation, PLAYER_ID id, int row, int col) : Player(orientation, id), Piece(TYPE_PLAYER, row, col)
	{
	}

	PlayerPiece::PlayerPiece(const PlayerPiece& playerPiece)
	{
		this->type = TYPE_PLAYER;
		this->orientation = playerPiece.orientation;
		this->id = playerPiece.id;
		this->position = new Square(*playerPiece.position);
	}

	// Retorna true si son iguales los id y orientation
	bool PlayerPiece::SamePlayer(const PlayerPiece& playerPiece) const
	{
		if (playerPiece.orientation == this->orientation
			&& playerPiece.id == this->id)
			return true;

		return false;
	}
}
