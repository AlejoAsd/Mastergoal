#include "stdafx.h"
#include "AbstractSearcher.h"
using namespace Common;

namespace AI
{
	AbstractSearcher::AbstractSearcher(Evaluator* evaluator, int maxDepth, int quiesDepth)
	{
		this->evaluator = evaluator;
		this->successorGen = new SuccessorGenerator();
		this->maxDepth = maxDepth;
		this->quiescenceDepth = quiesDepth;
	}

	AbstractSearcher::~AbstractSearcher()
	{
		if (this->evaluator)
		{
			delete this->evaluator;
			this->evaluator = NULL;
		}
		if (this->successorGen)
		{
			delete this->successorGen;
			this->successorGen = NULL;
		}
	}
}
