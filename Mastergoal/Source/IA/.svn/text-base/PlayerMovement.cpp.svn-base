#include "stdafx.h"
#include "PlayerMovement.h"
#include "Pieces.h"

namespace Common
{
	PlayerMovement::PlayerMovement(const PlayerPiece& piece, int rowAfter, int colAfter) : Movement(piece, rowAfter, colAfter) 
	{
		this->typeMovement = TYPE_PLAYER;
	}

	PlayerMovement::PlayerMovement(const PlayerMovement& playerMovement)
	{
		// TODO: sera que le llama al constructor de copias de Movement?
		// TODO: controlar si copia bien
		this->piece = new PlayerPiece(*(PlayerPiece*)playerMovement.piece);

		this->squareAfter = new Square(playerMovement.squareAfter->GetRow(), playerMovement.squareAfter->GetCol());

		this->typeMovement = TYPE_PLAYER;
	}

	void PlayerMovement::SetPlayer(ORIENTATION orientation, PLAYER_ID id)
	{
		((PlayerPiece*)piece)->SetOrientation(orientation);
		((PlayerPiece*)piece)->SetId(id);
	}
}
