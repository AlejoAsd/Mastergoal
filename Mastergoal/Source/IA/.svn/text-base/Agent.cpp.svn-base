#include "stdafx.h"
#include "Agent.h"
#include "NegamaxSearcher.h"
#include "ActionSearcher.h"
#include "Evaluators.h"
using namespace Common;

namespace AI
{
	Agent::Agent(int level, ORIENTATION orientation, string name)
	{
		/*totalTime = 0;
		totalPlays = 0;*/
#ifndef EVAL_ACTIONS
		Evaluator* evaluator;
		if (level == 1)
		{
			evaluator = new EvaluatorLevel1();
		}
		else if (level == 2)
		{
			evaluator = new EvaluatorLevel2();
		}
		else if (level == 3)
		{
			evaluator = new EvaluatorLevel3();
		}
		else // level == 4
		{
			evaluator = new EvaluatorLevel4();	
		}
		this->searcher = new NegamaxSearcher(evaluator);
#else
		DeltaEvaluator* evaluator;
		switch (level)
		{
		case 1:
			evaluator = new DeltaEvaluatorLevel1();
			break;
		case 2:
			evaluator = new DeltaEvaluatorLevel2();
			break;
		case 3:
			evaluator = new DeltaEvaluatorLevel3();
			break;
		case 4:
			evaluator = new DeltaEvaluatorLevel4();
			break;
		default: // level == 4
			evaluator = NULL;
		}
		this->searcher = new ActionSearcher(NULL, evaluator);
#endif
		this->orientation = orientation;
		this->name = name;
	}

	Agent::Agent(int level, ORIENTATION orientation, int searchDepth, string name)
	{
		/*totalTime = 0;
		totalPlays = 0;*/
		Evaluator* evaluator;

		if (level == 1)
		{
			evaluator = new EvaluatorLevel1();
		}
		else if (level == 2)
		{
			evaluator = new EvaluatorLevel2();
		}
		else if (level == 3)
		{
			evaluator = new EvaluatorLevel3();
		}
		else // level == 4
		{
			evaluator = new EvaluatorLevel4();
		}

		this->searcher = new NegamaxSearcher(evaluator, searchDepth);		
		this->orientation = orientation;
		this->name = name;
	}

	Agent::Agent(AbstractSearcher* searcher, const ORIENTATION& orientation, const string name)
	{
		this->searcher = searcher;
		this->orientation = orientation;
		this->name = name;
	}

	Agent::~Agent()
	{
		if (this->searcher)
		{
			delete this->searcher;
			this->searcher = NULL;
		}
	}
}
