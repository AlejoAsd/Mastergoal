#include "stdafx.h"
#include "NegamaxSearcher.h"
#include "Play.h"
#include "RulesManager.h"
using namespace std;

using namespace Utils;

namespace AI
{
	NegamaxSearcher::NegamaxSearcher(Evaluator* evaluator, int maxDepth, int quiesDepth) : AbstractSearcher(evaluator, maxDepth, quiesDepth)
	{
	}

	Play* NegamaxSearcher::FindPlayBySearch(const State& state)
	{
		// Se asume que tiene sucesores
		list<Play> listSuccessorPlays;
		this->successorGen->SuccessorPlays(state, listSuccessorPlays);

		// Pelota ahogada
		if (listSuccessorPlays.empty())
			return NULL;

		float bestValue = -FLT_MAX;
		float value = 0.0f;
		Play* bestPlay = new Play(listSuccessorPlays.front());
		float alpha = -FLT_MAX, beta = FLT_MAX;

		State currentState(state);
		// Para optimizar se setea firstPly afuera del for y antes de hacer la jugada
		currentState.SetFirstPly(false);
		for(list<Play>::iterator it = listSuccessorPlays.begin(); it != listSuccessorPlays.end(); it++)
		{	
			// Si es gol no hay nada más que analizar
			if (IsGoal(&(*it)))
			{
				if (bestPlay)
					delete bestPlay;
				bestPlay = new Play(*it);
				return bestPlay;
			}

			currentState.DoPlay(*it);
			
			if (!SpecialSquare(currentState)
				// Si es special square pero nMovements del play es 1, quiere decir que no se
				// chutó la pelota en esa jugada, por lo que no se repite el turno
				|| it->GetNMovements() == 1)
			{
				currentState.SwitchTurn();
				value = -Search(currentState, maxDepth - 1, -beta, -alpha);
				currentState.SwitchTurn();
			}
			else
			{
				value = Search(currentState, maxDepth - 1, alpha, beta);
			}
			currentState.UndoPlay(*it);
			
			if (value > bestValue)
			{
				bestValue = value;

				delete bestPlay;
				bestPlay = new Play(*it);
			}

			if (bestValue >= beta)
			{
				return bestPlay;
			}

			if (alpha < bestValue)
				alpha = bestValue;			
		}
		return bestPlay;
	}

	float NegamaxSearcher::Search(State& state, int depth, float alpha, float beta)
	{
		if (depth <= 0 || IsGoal(state.GetBoard()))
		{
			// TODO: QSearch
			return evaluator->Evaluate(state);
		}

		float bestValue = -FLT_MAX;

		// Como no es hoja se asume que tiene sucesores
		list<Play> listSuccessorPlays;
		this->successorGen->SuccessorPlays(state, listSuccessorPlays);

		// Si es pelota ahogada
		if (listSuccessorPlays.empty())
		{
			return EvaluateDrawnBall(state);
		}

		for(list<Play>::iterator it = listSuccessorPlays.begin(); it != listSuccessorPlays.end(); it++)
		{	
			state.DoPlay(*it);
			if (!SpecialSquare(state)
				// Si es special square pero nMovements del play es 1, quiere decir que no se
				// chutó la pelota en esa jugada, por lo que no se repite el turno
				|| it->GetNMovements() == 1)
			{
				state.SwitchTurn();
				bestValue = MAX(-Search(state, depth - 1, -beta, -alpha), bestValue);
				state.SwitchTurn();
			}
			else
			{
				bestValue = MAX(Search(state, depth - 1, alpha, beta), bestValue);
			}

			state.UndoPlay(*it);

			if (bestValue >= beta)
			{
				return bestValue;
			}

			if (alpha < bestValue)
				alpha = bestValue;
		}
		return bestValue;
	}
}
