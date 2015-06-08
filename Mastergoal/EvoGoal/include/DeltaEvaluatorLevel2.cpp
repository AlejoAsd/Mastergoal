#include "stdafx.h"
#include "DeltaEvaluatorLevel2.h"
#include "RulesManager.h"

using namespace Common;
using namespace Utils;

namespace AI
{
	DeltaEvaluatorLevel2::DeltaEvaluatorLevel2() : DeltaEvaluator()
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = 1.0f;
		}
	}

	DeltaEvaluatorLevel2::DeltaEvaluatorLevel2(float weights[]) : DeltaEvaluator()
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = weights[i];
		}
	}

	// Se asume que genome tiene la cantidad exacta de pesos
	void DeltaEvaluatorLevel2::SetWeights(const float *weights)
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = weights[i];
		}
	}

	float DeltaEvaluatorLevel2::Evaluate(const State& before, const State& after, const Play& play)
	{
		// Se verifica si es gol del que acaba de jugar
		if (IsGoal(*after.GetBoard(), before.GetTurn()))
		{
			return EVOGOAL_INFINITY;
		}

		CalculateDeltas(before, after, play);
		float score = 0.0;
		for (int i = 0; i < N_FEATURES; i++)
		{
			score += this->weights[i] * this->features[i];
		}
		return score;
	}

	void DeltaEvaluatorLevel2::CalculateDeltas(const State& before, const State& after, const Play& play)
	{
		//MOVED_TOWARDS_BALL
		//// Distancias a la pelota antes y despues del primer movimiento
		//int distBefore = before.GetBoard()->GetBall()->GetPosition()->Distance(*play.GetFirstMovement()->GetSquareBefore());
		//int distAfter = before.GetBoard()->GetBall()->GetPosition()->Distance(*play.GetFirstMovement()->GetSquareAfter());
		// Distancia minima a la pelota antes de mover
		int distBefore = MinDistToBall(*before.GetBoard(), before.GetTurn());
		// Si pateo la pelota la distancia minima despues del movimiento seria 1
		int distAfter = (play.GetNMovements() > 1) ? 1 : MinDistToBall(*after.GetBoard(), before.GetTurn());
		features[MOVED_TOWARDS_BALL] = (float)(distBefore - distAfter);

		//KICKED_FORWARD
		features[KICKED_FORWARD] = (float)(after.GetBoard()->GetBall()->GetRow() - before.GetBoard()->GetBall()->GetRow());
		if (before.GetTurn() == LEFT) features[KICKED_FORWARD] *= -1.0f;

		//KICKED_AWAY_OP
		// Distancias a la pelota del oponente antes y despues
		distBefore = MinDistToBall(*before.GetBoard(), after.GetTurn());
		distAfter = MinDistToBall(*after.GetBoard(), after.GetTurn());
		features[KICKED_AWAY_OP] = (float)(distAfter - distBefore);

		//MOVED_FORWARD
		features[MOVED_FORWARD] = (float)(play.GetFirstMovement()->GetSquareAfter()->GetRow() - play.GetFirstMovement()->GetSquareBefore()->GetRow());
		if (before.GetTurn() == LEFT) features[MOVED_FORWARD] *= -1.0f;

		//// Jugador mas adelantado
		//const PlayerPiece* pl1 = before.GetBoard()->GetPlayer(before.GetTurn(), ID_N);
		//// Jugador mas atrasado
		//const PlayerPiece* pl2 = before.GetBoard()->GetPlayer(before.GetTurn(), ID_P);
		//if (before.GetTurn() == LEFT)
		//{
		//	// El mas adelantado es el de menor fila
		//	if (pl2->GetRow() < pl1->GetRow())
		//		SwapMacro(const PlayerPiece*, pl1, pl2);
		//}
		//else
		//{
		//	// El mas adelantado es el de mayor fila
		//	if (pl1->GetRow() < pl2->GetRow())
		//		SwapMacro(const PlayerPiece*, pl1, pl2);
		//}

		//features[MOVED_FORWARD_1] = (float)(after.GetBoard()->GetPlayer(pl1->GetOrientation(), pl1->GetId())->GetRow() - pl1->GetRow());
		//features[MOVED_FORWARD_2] = (float)(after.GetBoard()->GetPlayer(pl2->GetOrientation(), pl2->GetId())->GetRow() - pl2->GetRow());
		//if (before.GetTurn() == LEFT) 
		//{
		//	features[MOVED_FORWARD_1] *= -1.0f;
		//	features[MOVED_FORWARD_2] *= -1.0f;
		//}
	}

	int DeltaEvaluatorLevel2::ReadWeights(ifstream& file)
	{
		float w = 0;
		int i;
		for (i = 0; i < N_FEATURES; i++)
		{
			file >> w;
			if (file.fail())
				return -1;
			weights[i] = w;
		}
		// Si no leyó todos los pesos, o leyó demás
		if (i != N_FEATURES)
			return -1;

		return 0;
	}

	int DeltaEvaluatorLevel2::MinDistToBall(const Board& board, ORIENTATION team)
	{
		PlayerPiece** players = (team == RIGHT) ? board.GetRightPieces() : board.GetLeftPieces();
		int minDist = board.GetBall()->GetPosition()->Distance(*players[0]->GetPosition());

		for (int i = 1; i < board.GetNPieces(); i++)
		{
			minDist = MIN(board.GetBall()->GetPosition()->Distance(*players[i]->GetPosition()), minDist);
		}
		return minDist;
	}
}

