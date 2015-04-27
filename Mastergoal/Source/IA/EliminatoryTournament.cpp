#include "stdafx.h"
#include "EliminatoryTournament.h"
#include "Definitions.h"
#include "Game.h"
#include <GA/garandom.h>

namespace AI
{
	EliminatoryTournament::EliminatoryTournament(GAPopulation* population) : Tournament()
	{
		int size = population->size();

		this->agentL = new Agent(GAME_LEVEL, LEFT, "agentL");
		this->agentR = new Agent(GAME_LEVEL, RIGHT, "agentR");

		// Se insertan los competidores a la llave principal
		for (int i = 0; i < size; i++)
		{
			this->winners.insert(&population->individual(i));
		}

		// Se sortean los contrincantes
		// winners[i] competirá con winners[i + 1]
		for (int i = 0; i < size; i++)
		{
			this->winners.swap(i, GARandomInt(0, size - 1));
		}
	}

	EliminatoryTournament::~EliminatoryTournament()
	{
		if (agentL) { delete agentL; agentL = NULL; }
		if (agentR) { delete agentR; agentR = NULL; }
	}

	void EliminatoryTournament::Play()
	{	
		/** Se juega la primera ronda **/
		// Los perdedores pasan a la rueda de perdedores
		this->WinnersRound();
		
		while (this->winners.size() > 1)
		{
			// En este punto hay la misma cantidad de jugadores en ambas ruedas
			this->LosersRound();
			// Ahora la cantidad de jugadores en la rueda de perdedores es la mitad que la de la rueda de ganadores

			// Se juega la rueda de ganadores y se insertan a los perdedores en la rueda de perdedores
			// La población de rueda de ganadores disminuye a la mitad, y la de perdedores se duplica
			this->WinnersRound();
			// Ahora la cantidad de jugadores en la rueda de ganadores es la mitad que la de la rueda de perdedores

			// Se juega la rueda de perdedores
			// La población de la rueda de perdedores disminuye a la mitad
			this->LosersRound();
			// Ahora hay la misma cantidad de jugadores en ambas ruedas
		}

		if (this->losers.size() == 1)
			this->Final();
	}

	/** Rueda de ganadores **/
	int EliminatoryTournament::WinnersRound()
	{
		int nGames = this->winners.size() / 2;

		// Si hay un solo competidor o menos en winners, no se puede competir
		if (nGames == 0)
			return 0;
		
		// Se inicializan los iteradores de la lista
		this->winners.head();
		this->losers.head();

		// Mientras haya elementos (se supone que hay una cantidad par de elementos, más aún, potencia de dos)
		for (int i = 0; i < nGames; i++)
		{
			SetAgentGenome(agentL, *this->winners.current());
			SetAgentGenome(agentR, *this->winners.next());
			Game game(GAME_LEVEL, LEFT, this->agentL, this->agentR);
			game.DoGame();

			// Tener en cuenta que winners.current() apunta a los genes de agentR
			// Ganó agentL, se lleva 3 puntos
			if (game.GetState()->GetGoalsLeft() == 1)
			{
				// Se mueve al perdedor a la rueda de perdedores
				// Se lo inserta luego del i-ésimo jugador, de manera a que juegue contra él
				this->losers.insert(*this->winners.current());
				// Se apunta al jugador i + 2 (i + 1 antes de la línea anterior)
				this->losers.next();
				GAGenome** tmpPtr = this->winners.remove();
				delete tmpPtr;

				// Se asigna el fintess por ganar (ahora current apunta a los genes de agentL)
				GAGenome* gen = (*this->winners.current());
				gen->score(gen->score() + 3.0f);
			}
			// Ganó agentR, se lleva 3 puntos
			else if (game.GetState()->GetGoalsRight() == 1)
			{
				// Se asigna el score por ganar
				GAGenome* gen = (*this->winners.current());
				gen->score(gen->score() + 3.0f);

				// Se mueve al perdedor a la rueda de perdedores
				this->losers.insert(*this->winners.prev());
				this->losers.next();
				GAGenome** tmpPtr = this->winners.remove();
				delete tmpPtr;
				this->winners.next();
			}
			// Empataron, ambos llevan 1 punto
			else
			{
				// Se elige el ganador al azar
				if (GARandomBit())
				{
					// Se asigna el fintess
					GAGenome* gen = (*this->winners.current());
					gen->score(gen->score() + 1.0f);

					// Se mueve al perdedor a la rueda de perdedores
					this->losers.insert(*this->winners.current());
					this->losers.next();
					GAGenome** tmpPtr = this->winners.remove();
					delete tmpPtr;

					// Se asigna el fintess (ahora current apunta a los genes de agentL)
					gen = (*this->winners.current());
					gen->score(gen->score() + 1.0f);
				}
				else
				{
					// Se asigna el score
					GAGenome* gen = (*this->winners.current());
					gen->score(gen->score() + 1.0f);

					// Se mueve al perdedor a la rueda de perdedores
					this->losers.insert(*this->winners.prev());
					this->losers.next();
					
					// Se asigna el fitness
					gen = (*this->winners.current());
					gen->score(gen->score() + 1.0f);

					GAGenome** tmpPtr = this->winners.remove();
					delete tmpPtr;
					this->winners.next();
				}
			}
			this->winners.next();
		}
		return 1;
	}

	/** Rueda de perdedores **/
	int EliminatoryTournament::LosersRound()
	{
		int nGames = this->losers.size() / 2;
		// Si queda un solo individuo o menos en losers
		if (nGames == 0)
			return 0;

		// Se inicializa el iterador de la lista
		this->losers.head();
		// Mientras haya elementos (se supone que hay una cantidad par de elementos, más aún, potencia de dos)
		for (int i = 0; i < nGames; i++)
		{
			SetAgentGenome(agentL, *this->losers.current());
			SetAgentGenome(agentR, *this->losers.next());
			Game game(GAME_LEVEL, LEFT, this->agentL, this->agentR);
			game.DoGame();

			// Tener en cuenta que losers.current() apunta a los genes de agentR
			// Ganó agentL, lleva 3 puntos
			if (game.GetState()->GetGoalsLeft() == 1)
			{
				// Se remueve al perdedor
				GAGenome** tmpPtr = this->losers.remove();
				delete tmpPtr;

				// Se asigna el score por ganar, (ahora current apunta a los genes de agentL)
				GAGenome* gen = (*this->losers.current());
				gen->score(gen->score() + 3.0f);
			}
			// Ganó agentR, lleva 3 puntos
			else if (game.GetState()->GetGoalsRight() == 1)
			{
				// Se asigna el score por ganar
				GAGenome* gen = (*this->losers.current());
				gen->score(gen->score() + 3.0f);

				// Se remueve al perdedor
				this->losers.prev();
				GAGenome** tmpPtr = this->losers.remove();
				delete tmpPtr;
				this->losers.next();
			}
			// Empataron, ambos llevan un punto
			else
			{
				// Se elige el ganador al azar
				if (GARandomBit())
				{
					// Se asigna el fintess
					GAGenome* gen = (*this->losers.current());
					gen->score(gen->score() + 1.0f);

					// Se remueve al perdedor
					GAGenome** tmpPtr = this->losers.remove();
					delete tmpPtr;

					// Se asigna el score, (ahora current apunta a los genes de agentL)
					gen = (*this->losers.current());
					gen->score(gen->score() + 1.0f);
				}
				else
				{
					// Se asigna el score
					GAGenome* gen = (*this->losers.current());
					gen->score(gen->score() + 1.0f);

					// Se apunta al perdedor
					this->losers.prev();

					// Se asigna el score
					gen = (*this->losers.current());
					gen->score(gen->score() + 1.0f);

					// Se remueve al perdedor
					GAGenome** tmpPtr = this->losers.remove();
					delete tmpPtr;
					this->losers.next();
				}
			}
			this->losers.next();
		}
		return 1;
	}

	void EliminatoryTournament::Final()
	{
		SetAgentGenome(agentL, *this->winners.head());
		SetAgentGenome(agentR, *this->losers.head());
		Game game(GAME_LEVEL, LEFT, this->agentL, this->agentR);
		game.DoGame();

		// Ganó agentL
		if (game.GetState()->GetGoalsLeft() == 1)
		{
			// Se asigna el score por ganar
			GAGenome* gen = (*this->winners.current());
			gen->score(gen->score() + 3.0f);

			GAGenome** tmpPtr = this->losers.remove();
			delete tmpPtr;
		}
		// Ganó agentR
		else if (game.GetState()->GetGoalsRight() == 1)
		{
			GAGenome** tmpPtr = this->winners.remove();
			delete tmpPtr;
			tmpPtr = this->losers.remove();
			this->winners.insert(*tmpPtr);
			delete tmpPtr;

			// Se asigna el score por ganar
			GAGenome* gen = (*this->winners.head());
			gen->score(gen->score() + 3.0f);
		}
		// Empataron
		else
		{
			// Se elige el ganador al azar
			if (GARandomBit())
			{
				// Se asigna el score
				GAGenome* gen = (*this->winners.current());
				gen->score(gen->score() + 1.0f);

				gen = (*this->losers.current());
				gen->score(gen->score() + 1.0f);

				GAGenome** tmpPtr = this->losers.remove();
				delete tmpPtr;
			}
			else
			{
				// Se asigna el score
				GAGenome* gen = (*this->winners.current());
				gen->score(gen->score() + 1.0f);

				gen = (*this->losers.current());
				gen->score(gen->score() + 1.0f);

				GAGenome** tmpPtr = this->winners.remove();
				delete tmpPtr;
				tmpPtr = this->losers.remove();
				this->winners.insert(*tmpPtr);
				delete tmpPtr;
			}
		}
	}
}
