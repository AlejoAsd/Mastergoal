#include "stdafx.h"
#include "DeltaEvaluatorLevel1.h"
#include "RulesManager.h"

using namespace Common;
using namespace Utils;

namespace AI
{
	DeltaEvaluatorLevel1::DeltaEvaluatorLevel1() : DeltaEvaluator()
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = 1.0f;
		}
	}

	DeltaEvaluatorLevel1::DeltaEvaluatorLevel1(float weights[]) : DeltaEvaluator()
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = weights[i];
		}
	}

	// Se asume que genome tiene la cantidad exacta de pesos
	void DeltaEvaluatorLevel1::SetWeights(const float *weights)
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = weights[i];
		}
	}

	float DeltaEvaluatorLevel1::Evaluate(const State& before, const State& after, const Play& play)
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

	void DeltaEvaluatorLevel1::CalculateDeltas(const State& before, const State& after, const Play& play)
	{
		//MOVED_TOWARDS_BALL
		// Distancias a la pelota antes y despues del primer movimiento
		int distBefore = before.GetBoard()->GetBall()->GetPosition()->Distance(*play.GetFirstMovement()->GetSquareBefore());
		int distAfter = before.GetBoard()->GetBall()->GetPosition()->Distance(*play.GetFirstMovement()->GetSquareAfter());
		features[MOVED_TOWARDS_BALL] = (float)(distBefore - distAfter);

		//KICKED_FORWARD
		features[KICKED_FORWARD] = (float)(after.GetBoard()->GetBall()->GetRow() - before.GetBoard()->GetBall()->GetRow());
		if (before.GetTurn() == LEFT) features[KICKED_FORWARD] *= -1.0f;

		//KICKED_AWAY_OP
		// Distancias a la pelota del oponente antes y despues
		distBefore = before.GetBoard()->GetBall()->GetPosition()->Distance(*before.GetBoard()->GetPlayer(after.GetTurn(), ID_N)->GetPosition());
		distAfter = after.GetBoard()->GetBall()->GetPosition()->Distance(*after.GetBoard()->GetPlayer(after.GetTurn(), ID_N)->GetPosition());
		features[KICKED_AWAY_OP] = (float)(distAfter - distBefore);

		//MOVED_FORWARD
		features[MOVED_FORWARD] = (float)(play.GetFirstMovement()->GetSquareAfter()->GetRow() - play.GetFirstMovement()->GetSquareBefore()->GetRow());
		if (before.GetTurn() == LEFT) features[MOVED_FORWARD] *= -1.0f;
	}

	int DeltaEvaluatorLevel1::ReadWeights(ifstream& file)
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
}

