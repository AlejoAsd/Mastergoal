#include "stdafx.h"
#include "Tournament.h"
#ifdef EVAL_ACTIONS
#include "ActionSearcher.h"
#endif

namespace AI
{
	Tournament::Tournament()
	{
	}

	Tournament::Tournament(const Tournament& tournament)
	{
	}

	Tournament::~Tournament()
	{
	}

	void Tournament::RealAgentSetter(AI::Agent *agent, GAGenome *genome)
	{
		float *weights = new float[((GARealGenome*)genome)->size()];
		for (int i = 0; i < ((GARealGenome*)genome)->size(); i++)
		{
			weights[i] = ((GARealGenome*)genome)->gene(i);
		}
	#ifndef EVAL_ACTIONS
		agent->GetEvaluator()->SetWeights(weights);
	#else
		((ActionSearcher*)agent->GetSearcher())->GetDeltaEvaluator()->SetWeights(weights);
	#endif

		delete[] weights;
		weights = NULL;
	}
}
