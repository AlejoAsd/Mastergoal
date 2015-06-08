#ifndef MOVEMENT_ITERATOR_H
#define MOVEMENT_ITERATOR_H

#include "Movement.h"
#include "PlayerMovement.h"
#include "BallMovement.h"

namespace Common
{
	/** Dos formas de iterar: 
		1. con Next() y Value()
		2. con FirstMovement() y NextBallMovement() **/
	class MovementIterator
	{
	public:
		MovementIterator() {}
		
		MovementIterator(Movement** movements, int nMovements)
		{
			this->index = -1;
			this->nMovements = nMovements;
			this->movements = movements;
		}

		MovementIterator(const MovementIterator& movIt)
		{
			this->movements = movIt.movements;
			this->nMovements = movIt.nMovements;
			this->index = movIt.index;
		}

		const Movement* Value() const
		{ 
			if (this->index < 0 || this->index >= this->nMovements)
				return NULL;

			return this->movements[index]; 
		}		

		bool Next()
		{
			if (this->index < this->nMovements - 1)
			{
				index++;
				return true;
			}
			return false;
		}

		// Retorna el primer movimiento del Play (PlayerMovement por lo tanto)
		// y setea index al segundo movimiento
		const PlayerMovement* FirstMovement()
		{
			this->index = 0;
			return (PlayerMovement*)this->movements[this->index++];
		}

		// Si index apunta a algún movimiento que no sea el primero (el del jugador),
		// lo retorna y apunta a siguiente; si no, retorna NULL
		const BallMovement* NextBallMovement()
		{
			if (this->index <= 0 || this->index >= this->nMovements)
				return NULL;

			return (BallMovement*)this->movements[this->index++];
		}

		// Se lo llama luego de NextBallMovement(), retorna un puntero al jugador
		// que recibe el balón si lo hay
		const Player* NextPlayerWithBall() const
		{ 
			if (this->index <= 0 || this->index >= this->nMovements)
				return NULL;

			return ((BallMovement*)this->movements[index])->GetPlayerWhoMoves(); 
		}		

		// Retorna el último movimiento de balón del Play si lo hay
		// y setea index al penúltimo movimiento
		const BallMovement* LastBallMovement()
		{
			this->index = nMovements - 1;
		
			if (index <= 0) // No hay movimientos de balón
				return NULL;

			return (BallMovement*)this->movements[this->index--];
		}

		// Si index apunta a algún movimiento que no sea el primero (el del jugador),
		// lo retorna y apunta al previo; si no, retorna NULL
		const BallMovement* PrevBallMovement()
		{
			if (this->index <= 0 || this->index >= this->nMovements)
				return NULL;

			return (BallMovement*)this->movements[this->index--];
		}

	private:
		Movement** movements;
		int nMovements;
		int index;
	};
}

#endif
