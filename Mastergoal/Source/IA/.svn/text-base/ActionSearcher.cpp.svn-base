#include "stdafx.h"
#include "ActionSearcher.h"
#include "Play.h"
#include "RulesManager.h"
using namespace std;

using namespace Utils;

namespace AI
{
	ActionSearcher::ActionSearcher(Evaluator* evaluator, DeltaEvaluator* deltaEvaluator, int maxDepth, int quiesDepth) : AbstractSearcher(evaluator, maxDepth, quiesDepth)
	{
		this->deltaEvaluator = deltaEvaluator;
	}

	ActionSearcher::~ActionSearcher()
	{
	}

	Play* ActionSearcher::FindPlayBySearch(const State& state)
	{
		// Se asume que tiene sucesores
		list<Play> listSuccessorPlays;
		this->successorGen->SuccessorPlays(state, listSuccessorPlays);
		list<RankedPlay> rankedSuc;

		// Pelota ahogada
		if (listSuccessorPlays.empty())
			return NULL;

		float bestValue = -FLT_MAX;
		Play* bestPlay = new Play(listSuccessorPlays.front());

		State currentState(state);
		// Para optimizar se setea firstPly afuera del for y antes de hacer la jugada
		currentState.SetFirstPly(false);
		for(list<Play>::iterator it = listSuccessorPlays.begin(); it != listSuccessorPlays.end(); it++)
		{
			currentState.DoPlay(*it);
			float value = 0.0f;
			
			if (!SpecialSquare(currentState)
				// Si es special square pero nMovements del play es 1, quiere decir que no se
				// chutó la pelota en esa jugada, por lo que no se repite el turno
				|| it->GetNMovements() == 1)
			{
				currentState.SwitchTurn();
				value = deltaEvaluator->Evaluate(state, currentState, *it);
				currentState.SwitchTurn();
			}
			else
			{
				value = deltaEvaluator->Evaluate(state, currentState, *it);
			}
			currentState.UndoPlay(*it);
			
			if (value > bestValue)
			{
				bestValue = value;
				delete bestPlay;
				bestPlay = new Play(*it);
			}		
		}
		return bestPlay;
	}
}
