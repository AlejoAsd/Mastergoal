#ifndef PLAY_H
#define PLAY_H

#include "Movements.h"
#include "Player.h"
using namespace std;

namespace Common
{
	class Play
	{
	public:
		/** Constructor **/
		Play(Movement** movements, int nMovements);

		Play(const Play& play);

		~Play();

		Play& operator = (const Play& play);

		inline const PlayerMovement* GetFirstMovement() const { return (PlayerMovement*)this->movements[0]; }

		inline const Movement* GetLastMovement() const { return this->movements[this->nMovements - 1]; }

		inline MovementIterator GetMovementsIterator() const { return MovementIterator(this->movements, this->nMovements); }

		inline const Player* GetPlayer() const { return ((PlayerMovement*)this->movements[0])->GetPiece(); }

		inline const int GetNMovements() const { return this->nMovements; }

	private:
		Movement** movements;

		int nMovements;

		// Constructor privado
		Play() {}
	};
}

#endif
