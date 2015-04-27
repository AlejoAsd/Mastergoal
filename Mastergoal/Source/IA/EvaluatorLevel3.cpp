#include "stdafx.h"
#include "EvaluatorLevel3.h"
#include "RulesManager.h"

using namespace Common;
using namespace Utils;

namespace AI
{
	EvaluatorLevel3::EvaluatorLevel3() : Evaluator()
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = 1.0f;
		}
	}

	EvaluatorLevel3::EvaluatorLevel3(float weights[]) : Evaluator()
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = weights[i];
		}
	}

	// Se asume que genome tiene la cantidad exacta de pesos
	void EvaluatorLevel3::SetWeights(const float *weights)
	{
		for (int i = 0; i < N_FEATURES; i++)
		{
			this->weights[i] = weights[i];
		}
	}

	float EvaluatorLevel3::Evaluate(const State& state)
	{
		// Se verifica si es gol del que acaba de jugar
		if (IsGoal(*state.GetBoard(), state.GetTurn() == LEFT ? RIGHT : LEFT))
		{
			/** Se calcula la menor distancia a la pelota **/
			PlayerPiece** players = state.GetTurn() == LEFT ? state.GetBoard()->GetLeftPieces() : state.GetBoard()->GetRightPieces();
			int minDistToBall = players[0]->GetPosition()->Distance(*state.GetBoard()->GetBall()->GetPosition());
			int aux = players[1]->GetPosition()->Distance(*state.GetBoard()->GetBall()->GetPosition());
			if (aux < minDistToBall)
				minDistToBall = aux;
			aux = players[2]->GetPosition()->Distance(*state.GetBoard()->GetBall()->GetPosition());
			if (aux < minDistToBall)
				minDistToBall = aux;
			aux = players[3]->GetPosition()->Distance(*state.GetBoard()->GetBall()->GetPosition());
			if (aux < minDistToBall)
				minDistToBall = aux;
			aux = players[4]->GetPosition()->Distance(*state.GetBoard()->GetBall()->GetPosition());
			if (aux < minDistToBall)
				minDistToBall = aux;
			/** ~Se calcula la menor distancia a la pelota **/

			// Se suma la menor distancia a la pelota de manera que cuando sea gol cantado 
			// del oponente, por lo menos el jugador se acerque a la pelota
			return -INFINITY - minDistToBall;
		}

		CalculateFeatures(state);
		float score = 0.0;
		for (int i = 0; i < N_FEATURES; i++)
		{
			score += this->weights[i] * this->features[i];
		}
		return score;
	}

	void EvaluatorLevel3::CalculateFeatures(const State& state)
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
		
		PlayerPiece** pls = state.GetTurn() == LEFT ? state.GetBoard()->GetLeftPieces() : state.GetBoard()->GetRightPieces();
		PlayerPiece** ops = state.GetTurn() == RIGHT ? state.GetBoard()->GetLeftPieces() : state.GetBoard()->GetRightPieces();
		// Distancias a la pelota
		int plsDist[5];
		int opsDist[5];
		int plsAdv[5];
		int opsAdv[5];
		for (int i = 0; i < 5; i++)
		{
			plsDist[i] = pls[i]->GetPosition()->Distance(*state.GetBoard()->GetBall()->GetPosition());
			opsDist[i] = ops[i]->GetPosition()->Distance(*state.GetBoard()->GetBall()->GetPosition());
			if (state.GetTurn() == LEFT)
			{
				plsAdv[i] = -(pls[0]->GetRow() - 7);
				opsAdv[i] = ops[0]->GetRow() - 7;
			}
			else
			{
				plsAdv[i] = pls[0]->GetRow() - 7;
				opsAdv[i] = -(ops[0]->GetRow() - 7);
			}
		}
		// Se ordena las distancias de menor a mayor y los avances de mayor a menor
		for (int i = 0; i < 5 - 1; i++)
		{
			for (int j = i + 1; j < 5; j++)
			{
				if (plsDist[j] < plsDist[i])
					SwapMacro(int, plsDist[i], plsDist[j]);
				if (opsDist[j] < opsDist[i])
					SwapMacro(int, opsDist[i], opsDist[j]);
				if (plsAdv[j] > plsAdv[i])
					SwapMacro(int, plsAdv[i], plsAdv[j]);
				if (opsAdv[j] > opsAdv[i])
					SwapMacro(int, opsAdv[i], opsAdv[j]);
			}
		}
		/** Players distance to ball **/
		features[PL_BALL_DIST_1] = (float)plsDist[0];
		features[PL_BALL_DIST_2] = (float)plsDist[1];
		features[PL_BALL_DIST_3] = (float)plsDist[2];
		/** ~Players distance to ball **/

		/** Opponents distance to ball **/		
		features[OP_BALL_DIST_1] = (float)opsDist[0];
		features[OP_BALL_DIST_2] = (float)opsDist[1];
		features[OP_BALL_DIST_3] = (float)opsDist[2];
		/** ~Opponents distance to ball **/

		/** Players advance **/
		features[PL_ADVANCE_1] = (float)plsAdv[0];
		features[PL_ADVANCE_2] = (float)plsAdv[1];
		features[PL_ADVANCE_3] = (float)plsAdv[2];
		/** ~Players advance **/

		/** Opponents advance **/
		features[OP_ADVANCE_1] = (float)opsAdv[0];
		features[OP_ADVANCE_2] = (float)opsAdv[1];
		features[OP_ADVANCE_3] = (float)opsAdv[2];
		/** ~Opponents advance **/

		/** Players & opponents too close to borders 
		    Players too close or too far 
		    Opponents too close or too far **/
		features[PL_TOO_CLOSE_BORDER] = 0.0f;
		features[OP_TOO_CLOSE_BORDER] = 0.0f;
		features[PLS_TOO_FAR] = 0.0f;
		features[PLS_TOO_CLOSE] = 0.0f;
		features[OPS_TOO_FAR] = 0.0f;
		features[OPS_TOO_CLOSE] = 0.0f;
		for (int i = 0; i < state.GetBoard()->GetNPieces(); i++)
		{
			if (pls[i]->GetCol() == 0 || pls[i]->GetCol() == 10)
				features[PL_TOO_CLOSE_BORDER] += 2;
			else if (pls[i]->GetCol() == 1 || pls[i]->GetCol() == 9)
				features[PL_TOO_CLOSE_BORDER] += 1;

			if (ops[i]->GetCol() == 0 || ops[i]->GetCol() == 10)
				features[OP_TOO_CLOSE_BORDER] += 2;
			else if (ops[i]->GetCol() == 1 || ops[i]->GetCol() == 9)
				features[OP_TOO_CLOSE_BORDER] += 1;

			/** Players too close or too far 
			    Opponents too close or too far **/
			if (i != state.GetBoard()->GetNPieces() - 1)
			{
				for (int j = i + 1; j < state.GetBoard()->GetNPieces(); j++)
				{
					int dist = pls[i]->GetPosition()->Distance(*pls[j]->GetPosition());
					if (dist > 5)
						features[PLS_TOO_FAR] += 1.0f;
					else if (dist < 3)
						features[PLS_TOO_CLOSE] += 1.0f;

					dist = ops[i]->GetPosition()->Distance(*ops[j]->GetPosition());
					if (dist > 5)
						features[OPS_TOO_FAR] += 1.0f;
					else if (dist < 3)
						features[OPS_TOO_CLOSE] += 1.0f;
				}
			}
			/** ~Players too close or too far 
			    ~Opponents too close or too far **/
		}
		/** ~Players & opponents too close to borders 
		    ~Players too close or too far 
		    ~Opponents too close or too far **/

		/** Keepers in area **/
		features[PL_KEEPER_IN_AREA] = 0.0;
		features[OP_KEEPER_IN_AREA] = 0.0;
		if (state.GetBoard()->GetPlayer(LEFT, ID_G)->GetRow() <= 4 &&
			(state.GetBoard()->GetPlayer(LEFT, ID_G)->GetCol() > 0 &&
			 state.GetBoard()->GetPlayer(LEFT, ID_G)->GetCol() < 10))
		{
			if (state.GetTurn() == LEFT)
				features[PL_KEEPER_IN_AREA] = 1.0;
			else
				features[OP_KEEPER_IN_AREA] = 1.0;
		}
		if (state.GetBoard()->GetPlayer(RIGHT, ID_G)->GetRow() >= 10 &&
			(state.GetBoard()->GetPlayer(RIGHT, ID_G)->GetCol() > 0 &&
			 state.GetBoard()->GetPlayer(RIGHT, ID_G)->GetCol() < 10))
		{
			if (state.GetTurn() == RIGHT)
				features[PL_KEEPER_IN_AREA] = 1.0;
			else
				features[OP_KEEPER_IN_AREA] = 1.0;
		}
		/** ~Keepers in area **/

		/** Closer to ball **/
		features[CLOSER_TO_BALL] = features[OP_BALL_DIST_1] - features[PL_BALL_DIST_1];
		/** ~Closer to ball **/

		/** Neutral square **/
		features[NEUTRAL_SQR] = RulesManager::IsNeutralSquare(state) ? 1.0f : 0.0f;
		/** ~Neutral square **/

		/** Special square **/
		features[SPECIAL_SQR] = RulesManager::IsSpecial(state.GetBoard()->GetBall()->GetCol(), state.GetBoard()->GetBall()->GetRow(), state.GetTurn()) ? 1.0f : 0.0f;
		/** ~Special square **/
	}

	int EvaluatorLevel3::ReadWeights(ifstream& file)
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
