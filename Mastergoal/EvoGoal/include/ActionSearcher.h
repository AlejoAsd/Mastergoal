#ifndef ACTION_SEARCHER_H
#define ACTION_SEARCHER_H

#include "AbstractSearcher.h"
#include "Evaluators.h"

namespace AI
{
	// Buscador que utiliza DeltaEvaluator
	class ActionSearcher : public AbstractSearcher
	{
	public:
		ActionSearcher(Evaluator* evaluator, DeltaEvaluator* deltaEvaluator, int maxDepth = DEFAULT_MAX_DEPTH, int quiesDepth = DEFAULT_QUIES_DEPTH);

		~ActionSearcher();

		Play* FindPlayBySearch(const State& state);

		inline DeltaEvaluator* GetDeltaEvaluator() const { return deltaEvaluator; }
	protected:
		DeltaEvaluator* deltaEvaluator;
	};
}


#endif
