#ifndef PLAYER_MOVEMENT_H
#define PLAYER_MOVEMENT_H

#include "Movement.h"

namespace Common
{
	class PlayerMovement : public Movement
	{
	public:
		/** Constructores **/
		PlayerMovement(const PlayerPiece& piece, int rowAfter, int colAfter);

		PlayerMovement(const PlayerMovement& playerMovement);

		const PlayerPiece* GetPiece() const { return ((PlayerPiece*)this->piece); }

		void SetPlayer(ORIENTATION orientation, PLAYER_ID id);
	};
}

#endif
