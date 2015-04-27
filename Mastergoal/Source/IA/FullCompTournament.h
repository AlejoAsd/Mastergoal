#ifndef FULL_COMP_TOUR
#define FULL_COMP_TOUR

#include <ga/GAPopulation.h>
#include "Tournament.h"
#include "SampleTourData.h"

namespace AI
{
	class FullCompTournament : public Tournament
	{
	public:
		FullCompTournament(GAPopulation *pop);

		FullCompTournament(const FullCompTournament& tournament);

		virtual ~FullCompTournament();

		virtual void Play();

	protected:
		GAPopulation *pop;
	};
}

#endif
