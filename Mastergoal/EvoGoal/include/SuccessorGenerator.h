#ifndef SUCCESSOR_GENERATOR_H
#define SUCCESSOR_GENERATOR_H

#include <vector>
#include <list>
#include "Play.h"
#include "State.h"
#include "RulesManager.h"

using namespace std;
using namespace Common;
using namespace Utils;

namespace AI
{
	/** Esta clase contiene la función que genera y ordena los sucesores de un estado **/
	class SuccessorGenerator
	{
	private:
		static const int N_PLAYER_INCS = 16;
		static const int N_BALL_INCS = 32;
		static Square playerIncs[N_PLAYER_INCS];
		static Square ballIncs[N_BALL_INCS];

		struct SqrPass
		{
			bool visited;
			int nKicks;
		};
		// Indica a qué jugadores ya se hicieron pases y en qué casillas
		// sqrPasses[ID_JUGADOR][FILA][COLUMNA]
		SqrPass*** passesPositions;
		bool** finalBallPositions;

		// Genera los posibles movimientos del balón luego de haber movido una ficha,
		// nMovement == la cantidad de pases hechos hasta el momento (0 la primera vez que se llama al método)
		// state.playerWithBall no sebe ser NULL
		void SuccessorBallMovements(const State& initialState, State& currentState, Movement** playMovements, list<Play>& plays);

		inline bool AlreadyInSuccessors(const Square* ballPos)
		{
			if (!RulesManager::IsValidSquare(ballPos))
				return true;

			return this->finalBallPositions[ballPos->GetRow()][ballPos->GetCol()];
		}

		inline bool AlreadyBallPassed(const Square* toWhere, PLAYER_ID toWhom, int nKick)
		{
			return (this->passesPositions[toWhom - 1][toWhere->GetRow()][toWhere->GetCol()].visited == true
				&& this->passesPositions[toWhom - 1][toWhere->GetRow()][toWhere->GetCol()].nKicks <= nKick);
		}

		inline void MarkFinalBallPos(const Square* ballPos)
		{
			this->finalBallPositions[ballPos->GetRow()][ballPos->GetCol()] = true;
		}

		inline void MarkAsPassed(const Square* toWhere, PLAYER_ID toWhom, int nKick)
		{
			this->passesPositions[toWhom - 1][toWhere->GetRow()][toWhere->GetCol()].visited = true;
			this->passesPositions[toWhom - 1][toWhere->GetRow()][toWhere->GetCol()].nKicks = nKick;
		}

		inline void ResetPassesPositions()
		{
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					for (int k = 0; k < 11; k++)
					{
						this->passesPositions[i][j][k].visited = false;
						this->passesPositions[i][j][k].nKicks = 0;
					}
				}
			}
		}

		inline void ResetFinalBallPositions()
		{
			for (int i = 0; i < 15; i++)
			{
				for (int j = 0; j < 11; j++)
				{
					this->finalBallPositions[i][j] = false;
				}
			}
		}

		inline void ShufflePlayerIncs()
		{
			for (int i = 0; i < N_PLAYER_INCS; i++)
			{
				int randPos = rand() % N_PLAYER_INCS;
				int auxCol = playerIncs[i].GetCol();
				int auxRow = playerIncs[i].GetRow();
				playerIncs[i].SetCol(playerIncs[randPos].GetCol());
				playerIncs[i].SetRow(playerIncs[randPos].GetRow());
				playerIncs[randPos].SetCol(auxCol);
				playerIncs[randPos].SetRow(auxRow);
			}
		}

		inline void ShuffleBallIncs()
		{
			for (int i = 0; i < N_BALL_INCS; i++)
			{
				int randPos = rand() % N_BALL_INCS;
				int auxCol = ballIncs[i].GetCol();
				int auxRow = ballIncs[i].GetRow();
				ballIncs[i].SetCol(ballIncs[randPos].GetCol());
				ballIncs[i].SetRow(ballIncs[randPos].GetRow());
				ballIncs[randPos].SetCol(auxCol);
				ballIncs[randPos].SetRow(auxRow);
			}
		}

	public:
		/** Constructor **/
		SuccessorGenerator();

		~SuccessorGenerator();

		/** Métodos **/
		// Genera las jugadas posibles a partir de un estado
		void SuccessorPlays(const State& state, list<Play>& listSuccessorPlays);
	};
}

#endif
