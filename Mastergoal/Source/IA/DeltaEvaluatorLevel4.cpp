#include "stdafx.h"
#include "DeltaEvaluatorLevel4.h"
#include "RulesManager.h"

using namespace Common;
using namespace Utils;

namespace AI
{
	DeltaEvaluatorLevel4::DeltaEvaluatorLevel4() : DeltaEvaluator()
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = 1.0f;
		}
	}

	DeltaEvaluatorLevel4::DeltaEvaluatorLevel4(float weights[]) : DeltaEvaluator()
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = weights[i];
		}
	}

	// Se asume que genome tiene la cantidad exacta de pesos
	void DeltaEvaluatorLevel4::SetWeights(const float *weights)
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = weights[i];
		}
	}

	float DeltaEvaluatorLevel4::Evaluate(const State& before, const State& after, const Play& play)
	{
		// Se verifica si es gol del que acaba de jugar
		if (IsGoal(*after.GetBoard(), before.GetTurn()))
		{
			return INFINITY;
		}

		CalculateDeltas(before, after, play);
		float score = 0.0;
		for (int i = 0; i < N_FEATURES; i++)
		{
			score += this->weights[i] * this->features[i];
		}
		return score;
	}

	void DeltaEvaluatorLevel4::CalculateDeltas(const State& before, const State& after, const Play& play)
	{
		//MOVED_TOWARDS_BALL
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
	}

	int DeltaEvaluatorLevel4::ReadWeights(ifstream& file)
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

	int DeltaEvaluatorLevel4::MinDistToBall(const Board& board, ORIENTATION team)
	{
		PlayerPiece** players = (team == RIGHT) ? board.GetRightPieces() : board.GetLeftPieces();
		int minDist = board.GetBall()->GetPosition()->Distance(*players[0]->GetPosition());

		for (int i = 1; i < board.GetNPieces(); i++)
		{
			minDist = min(board.GetBall()->GetPosition()->Distance(*players[i]->GetPosition()), minDist);
		}
		return minDist;
	}
}

