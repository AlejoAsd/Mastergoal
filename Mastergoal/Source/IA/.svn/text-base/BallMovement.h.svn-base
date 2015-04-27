#ifndef BALL_MOVEMENT_H
#define BALL_MOVEMENT_H

#include "Movement.h"

namespace Common
{
	class BallMovement : public Movement
	{
	public:
		BallMovement(const Common::BallPiece &ball, int rowAfter, int colAfter, const Player& playerWhoMoves);

		BallMovement(const BallMovement& ballMovement);

		~BallMovement();

		const Player* GetPlayerWhoMoves() const { return this->playerWhoMoves; }

		void SetPlayerWhoMoves(ORIENTATION orientation, PLAYER_ID id);

		const BallPiece* GetPiece() const { return ((BallPiece*)this->piece); }

	private:
		Player* playerWhoMoves;
	};
}

#endif
