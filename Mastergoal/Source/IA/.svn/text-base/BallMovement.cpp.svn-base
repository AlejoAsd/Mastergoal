#include "stdafx.h"
#include "BallMovement.h"

namespace Common
{
	BallMovement::BallMovement(const Common::BallPiece &ball, int rowAfter, int colAfter, const Player& playerWhoMoves) : Movement(ball, rowAfter, colAfter)
	{
		this->playerWhoMoves = new Player(playerWhoMoves);
		this->typeMovement = TYPE_BALL;
	}

	BallMovement::BallMovement(const BallMovement& ballMovement)
	{
		// TODO: controlar si copia bien
		this->piece = new BallPiece(*(BallPiece*)ballMovement.piece);

		this->squareAfter = new Square(ballMovement.squareAfter->GetRow(), ballMovement.squareAfter->GetCol());

		// TODO: sera que le llama al constructor de copias de Movement?
		this->playerWhoMoves = new Player(*ballMovement.playerWhoMoves);

		this->typeMovement = TYPE_BALL;
	}

	BallMovement::~BallMovement()
	{
		if (this->playerWhoMoves)
		{
			delete this->playerWhoMoves;
			this->playerWhoMoves = NULL;
		}
	}

	void BallMovement::SetPlayerWhoMoves(ORIENTATION orientation, PLAYER_ID id)
	{
		this->playerWhoMoves->SetOrientation(orientation);
		this->playerWhoMoves->SetId(id);
	}
}
