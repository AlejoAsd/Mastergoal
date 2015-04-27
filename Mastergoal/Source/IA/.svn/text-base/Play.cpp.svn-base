#include "stdafx.h"
#include "Play.h"
#include "Movements.h"
#include "Player.h"
using namespace std;

namespace Common
{
	Play::Play(Common::Movement **movements, int nMovements)
	{
		this->nMovements = nMovements;
		this->movements = new Movement*[nMovements];

		this->movements[0] = new PlayerMovement(*(PlayerMovement*)movements[0]);
		for (int i = 1; i < nMovements; i++)
		{
			this->movements[i] = new BallMovement(*(BallMovement*)movements[i]);
		}
	}

	Play::Play(const Play& play)
	{
		this->nMovements = play.nMovements;
		this->movements = new Movement*[this->nMovements];

		this->movements[0] = new PlayerMovement(*(PlayerMovement*)play.movements[0]);
		for (int i = 1; i < this->nMovements; i++)
		{
			this->movements[i] = new BallMovement(*(BallMovement*)play.movements[i]);
		}
	}

	Play::~Play()
	{
		if (this->movements)
		{
			for (int i = 0; i < this->nMovements; i++)
			{
				if (this->movements[i])
				{
					delete this->movements[i];
					this->movements[i] = NULL;
				}
			}
			delete[] this->movements;
			this->movements = NULL;
		}
	}

	Play& Play::operator =(const Common::Play &play)
	{
		if (this->nMovements > 0)
		{
			// Se borran todos los moviemientos
			for (int i = 0; i < this->nMovements; i++)
			{
				if (this->movements[i])
				{
					delete this->movements[i];
					this->movements[i] = NULL;
				}
			}
			// Si el size de los vectores de movs no coinciden se debe crear uno nuevo
			if (this->nMovements != play.nMovements)
			{
				this->nMovements = play.nMovements;
				delete[] this->movements;
				this->movements = new Movement*[this->nMovements];
			}
		}
		else // Si no había movimientos
		{
			this->nMovements = play.nMovements;
			this->movements = new Movement*[this->nMovements];
		}
	
		this->movements[0] = new PlayerMovement(*(PlayerMovement*)play.movements[0]);
		for (int i = 1; i < this->nMovements; i++)
		{
			this->movements[i] = new BallMovement(*(BallMovement*)play.movements[i]);
		}
		return *this;
	}
}
