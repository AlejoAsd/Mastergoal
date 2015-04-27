#ifndef NEGAMAX_SEARCHER_H
#define NEGAMAX_SEARCHER_H

#include "AbstractSearcher.h"
#include <list>

namespace AI
{
	// Buscador Negamax con poda alfa beta
	class NegamaxSearcher : public AbstractSearcher
	{
	public:
		NegamaxSearcher(Evaluator* evaluator, int maxDepth = DEFAULT_MAX_DEPTH, int quiesDepth = DEFAULT_QUIES_DEPTH);

		Play* FindPlayBySearch(const State& state);

		float Search(State& state, int depth, float alpha, float beta);	
	};
}


#endif
