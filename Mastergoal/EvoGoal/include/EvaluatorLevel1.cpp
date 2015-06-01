#include "stdafx.h"
#include "EvaluatorLevel1.h"
#include "RulesManager.h"

using namespace Common;
using namespace Utils;

namespace AI
{
	EvaluatorLevel1::EvaluatorLevel1() : Evaluator()
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = 1.0f;
		}
	}

	EvaluatorLevel1::EvaluatorLevel1(float weights[]) : Evaluator()
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = weights[i];
		}
	}

	// Se asume que genome tiene la cantidad exacta de pesos
	void EvaluatorLevel1::SetWeights(const float *weights)
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = weights[i];
		}
	}

	float EvaluatorLevel1::Evaluate(const State& state)
	{
		// Se verifica si es gol del que acaba de jugar
		if (IsGoal(*state.GetBoard(), state.GetTurn() == LEFT ? RIGHT : LEFT))
		{
			/** Se calcula la distancia a la pelota **/
			PlayerPiece** players = state.GetTurn() == LEFT ? state.GetBoard()->GetLeftPieces() : state.GetBoard()->GetRightPieces();
			int distToBall = players[0]->GetPosition()->Distance(*state.GetBoard()->GetBall()->GetPosition());			
			/** ~Se calcula la menor distancia a la pelota **/

			// Se suma la menor distancia a la pelota de manera que cuando sea gol cantado 
			// del oponente, por lo menos el jugador se acerque a la pelota
			return -INFINITY - distToBall;
		}

		CalculateFeatures(state);
		float score = 0.0;
		for (int i = 0; i < N_FEATURES; i++)
		{
			score += this->weights[i] * this->features[i];
		}
		return score;
	}

	void EvaluatorLevel1::CalculateFeatures(const State& state)
	{
		/** Ball advance **/
		features[BALL_ADVANCE] = (float)(state.GetBoard()->GetBall()->GetRow() - 7);
		if (state.GetTurn() == LEFT)
		{
			features[BALL_ADVANCE] *= -1.0;			
		}
		// Si la pelota está a una o dos filas del arco contrario, pero está en una casilla donde no puede llegar en un chute
		if ((features[BALL_ADVANCE] == 6.0 && (state.GetBoard()->GetBall()->GetCol() == 1 || state.GetBoard()->GetBall()->GetCol() == 2 || state.GetBoard()->GetBall()->GetCol() == 9 || state.GetBoard()->GetBall()->GetCol() == 10))
				|| (features[BALL_ADVANCE] == 5.0 && (state.GetBoard()->GetBall()->GetCol() == 1 || state.GetBoard()->GetBall()->GetCol() == 10)))
			features[BALL_ADVANCE]--;
		/** ~Ball advance **/

		/** Players distance to ball **/
		PlayerPiece** pls = state.GetTurn() == LEFT ? state.GetBoard()->GetLeftPieces() : state.GetBoard()->GetRightPieces();
		features[PL_BALL_DIST_1] = (float)pls[0]->GetPosition()->Distance(*state.GetBoard()->GetBall()->GetPosition());
		/** ~Players distance to ball **/

		/** Opponents distance to ball **/
		PlayerPiece** ops = state.GetTurn() == RIGHT ? state.GetBoard()->GetLeftPieces() : state.GetBoard()->GetRightPieces();
		features[OP_BALL_DIST_1] = (float)ops[0]->GetPosition()->Distance(*state.GetBoard()->GetBall()->GetPosition());
		/** ~Opponents distance to ball **/

		/** Players & opponents advance **/
		features[PL_ADVANCE_1] = (float)(pls[0]->GetRow() - 7);
		
		features[OP_ADVANCE_1] = (float)(ops[0]->GetRow() - 7);
		
		if (state.GetTurn() == LEFT)
		{
			features[PL_ADVANCE_1] *= -1.0;
		}
		else
		{
			features[OP_ADVANCE_1] *= -1.0;
		}
		/** ~Players & opponents advance **/

		/** Players & opponents distance to ball's column **/
		features[PL_BALL_COL_DIST] = (float)Abs(state.GetBoard()->GetBall()->GetCol() - pls[0]->GetCol());
		features[OP_BALL_COL_DIST] = (float)Abs(state.GetBoard()->GetBall()->GetCol() - ops[0]->GetCol());
		/** ~Players & opponents distance to ball's column **/

		/** Closer to ball **/
		features[CLOSER_TO_BALL] = features[OP_BALL_DIST_1] - features[PL_BALL_DIST_1];
		/** ~Closer to ball **/
	}

	int EvaluatorLevel1::ReadWeights(ifstream& file)
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

