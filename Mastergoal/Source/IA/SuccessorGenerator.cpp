#include "stdafx.h"
#include <vector>
#include <list>
#include "SuccessorGenerator.h"
#include "Movements.h"
#include "State.h"
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace Common;
using namespace Utils;

namespace AI
{
	Square SuccessorGenerator::playerIncs[N_PLAYER_INCS] = 
	{ 
		Square(-2, -2), Square(-2, 0), Square(-2, 2),
		Square(-1, -1), Square(-1, 0), Square(-1, 1),
		Square(0, -2), Square(0, -1), Square(0, 1), Square(0, 2),
		Square(1, -1), Square(1, 0), Square(1, 1),
		Square(2, -2), Square(2, 0), Square(2, 2)
	};

	Square SuccessorGenerator::ballIncs[N_BALL_INCS] = 
	{ 
		Square(-4, -4), Square(-4, 0), Square(-4, 4),
		Square(-3, -3), Square(-3, 0), Square(-3, 3),
		Square(-2, -2), Square(-2, 0), Square(-2, 2),
		Square(-1, -1), Square(-1, 0), Square(-1, 1),
		Square(0, -4), Square(0, -3), Square(0, -2), Square(0, -1), 
		Square(0, 1), Square(0, 2), Square(0, 3), Square(0, 4),
		Square(1, -1), Square(1, 0), Square(1, 1),
		Square(2, -2), Square(2, 0), Square(2, 2),
		Square(3, -3), Square(3, 0), Square(3, 3),
		Square(4, -4), Square(4, 0), Square(4, 4)
	};

	SuccessorGenerator::SuccessorGenerator()
	{
		srand( (unsigned)time( NULL ) );
		this->passesPositions = new SqrPass**[5];

		for (int i = 0; i < 5; i++)
		{
			this->passesPositions[i] = new SqrPass*[15];
			for (int j = 0; j < 15; j++)
			{
				this->passesPositions[i][j] = new SqrPass[11];
				for (int k = 0; k < 11; k++)
				{
					this->passesPositions[i][j][k].visited = false;
					this->passesPositions[i][j][k].nKicks = 0;
				}
			}
		}
		this->finalBallPositions = new bool*[15];
		for (int i = 0; i < 15; i++)
		{
			this->finalBallPositions[i] = new bool[11];
			for (int j = 0; j < 11; j++)
			{
				this->finalBallPositions[i][j] = false;
			}
		}
	}

	SuccessorGenerator::~SuccessorGenerator()
	{
		if (this->passesPositions)
		{
			delete[] this->passesPositions;
			this->passesPositions = NULL;
		}
		if (this->finalBallPositions)
		{
			delete[] this->finalBallPositions;
			this->finalBallPositions = NULL;
		}
	}

	void SuccessorGenerator::SuccessorPlays(const State& state, list<Play>& pieceSuccessorPlays)
	{
		Movement* movementSuccessors[5];
		State auxState(state);
		int ballRow = state.GetBoard()->GetBall()->GetRow();
		int ballCol = state.GetBoard()->GetBall()->GetCol();
		PlayerPiece** pieces = (state.GetTurn() == LEFT) ? state.GetBoard()->GetLeftPieces() : state.GetBoard()->GetRightPieces();
		int nPieces = state.GetBoard()->GetNPieces();

		PlayerMovement movement(*pieces[0], pieces[0]->GetRow(), pieces[0]->GetCol());
		ShufflePlayerIncs();
		// Para cada movida posible de jugador...		
		for (int inc = 0; inc < N_PLAYER_INCS; inc++)
		{
			// ...Para cada ficha del equipo que juega se buscan los movimientos posibles
			int i = rand() % nPieces;
			for (int i0 = 0; i0 < nPieces; i0++)
			{
				i = (i + i0) % nPieces;
				movement.SetPlayer(pieces[i]->GetOrientation(), pieces[i]->GetId());
				movement.SetSquareBefore(pieces[i]->GetRow(), pieces[i]->GetCol());
				movement.SetSquareAfter(pieces[i]->GetRow()+playerIncs[inc].GetRow(), pieces[i]->GetCol()+playerIncs[inc].GetCol() );

				if (RulesManager::IsValidPlayerMovePC(state, &movement))
				{
					movementSuccessors[0] = new PlayerMovement(movement);					

					// Aplicar el posible movimiento a auxState
					auxState.DoMove(&movement);					
					vector<Player> posiblePlayersWithBall = RulesManager::GetPossiblePlayersWithBall(auxState);

					if (posiblePlayersWithBall.size() > 0)
					{
						for(vector<Player>::const_iterator it = posiblePlayersWithBall.begin(); it != posiblePlayersWithBall.end(); it++)
						{
							ResetPassesPositions();
							ResetFinalBallPositions();
							auxState.SetPlayerWithBall((Player*)&(*it));
							SuccessorBallMovements(state, auxState, movementSuccessors, pieceSuccessorPlays);
							auxState.SetPlayerWithBall(NULL);
						}
					}
					else
					{						
						Play playToInsert(movementSuccessors, 1);
						pieceSuccessorPlays.push_back(playToInsert);
					}							

					// Deshacer el movimiento de la ficha
					auxState.UndoMove(&movement);
					delete movementSuccessors[0];
					movementSuccessors[0] = NULL;
				}
			}
		}
	}

	// Genera los posibles movimientos del balón luego de haber movido una ficha
	// state.playerWithBall no sebe ser NULL
	void SuccessorGenerator::SuccessorBallMovements(const State& initialState, State& currentState, Movement** playMovements, list<Play>& plays)
	{
		int nMovement = currentState.GetNKicks();
		// Si ya se hicieron 4 pases se retorna o el estado ya fue examinado
		if (nMovement >= 4 || AlreadyBallPassed(currentState.GetBoard()->GetBall()->GetPosition(), currentState.PlayerWithBall()->GetId(), nMovement))
			return;

		// Se agrega el estado a la lista de examinados para no volver 
		// a generar sus sucesores
		MarkAsPassed(currentState.GetBoard()->GetBall()->GetPosition(), currentState.PlayerWithBall()->GetId(), nMovement);
		
		const BallPiece* piece = currentState.GetBoard()->GetBall();

		// Pieza que efectúa el movimiento
		Player playerWithBall(*currentState.PlayerWithBall());

		// Se setea la pieza que se mueve
		BallMovement movement(*piece, piece->GetRow(), piece->GetCol(), playerWithBall);
		ShuffleBallIncs();
		for (int inc = 0; inc < N_BALL_INCS; inc++)
		{
			// Se setea la posición del movimiento
			movement.SetSquareAfter(piece->GetRow() + ballIncs[inc].GetRow(), piece->GetCol() + ballIncs[inc].GetCol());
			
			if (!AlreadyInSuccessors(movement.GetSquareAfter()))
			{
				if (RulesManager::IsValidBallMovePC(currentState, &movement))
				{
					// Se agrega el movimiento a los movimientos de la jugada
					playMovements[nMovement + 1] = new BallMovement(movement);

					// Se obtienen los jugadores del equipo que juega que están 
					// alrededor de la pelota, el método no incluye al que actualmente 
					// posee el balón
					vector<Player> playersAroundBall = RulesManager::GetPossiblePlayersWithBall(currentState, &movement);

					// Si no hay candidatos a la posesión del balón la jugada termina
					if (playersAroundBall.empty())
					{
						Play playToInsert(playMovements, nMovement + 2);
						
						plays.push_back(playToInsert);
						MarkFinalBallPos(playToInsert.GetLastMovement()->GetSquareAfter());
					}
					else
					// Se itera con cada cadidato a la posesión del balón
					{
						for (vector<Player>::const_iterator it = playersAroundBall.begin(); it != playersAroundBall.end(); it++)
						{
							Player withBall = *it;
							// Se realiza el movimiento en el tablero
							// El balón queda en posesión de una copia de withBall
							currentState.DoMove(&movement, &withBall);
							currentState.SetNKicks(currentState.GetNKicks() + 1);

							// Se buscan los siguientes movimientos de balón
							SuccessorBallMovements(initialState, currentState, playMovements, plays);

							// Se retorna el estado a su estado anterior
							currentState.UndoMove(&movement);
							currentState.SetNKicks(currentState.GetNKicks() - 1);
							// (Hace una copia de playerWithBall para asociarlo al estado)
							currentState.SetPlayerWithBall(&playerWithBall);
						}
					}
					// Se quita el movimiento y jugador de  los movimientos de la jugada
					delete playMovements[nMovement + 1];
					playMovements[nMovement + 1] = NULL;
				}
			}
		}
	}
}
